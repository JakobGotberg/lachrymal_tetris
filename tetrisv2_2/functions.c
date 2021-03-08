/* mipslabfunc.c
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header_file.h"  /* Declatations for these labs */


/* Declare a helper function which is local to this file */
static void num32asc( char * s, int ); 

#define DISPLAY_CHANGE_TO_COMMAND_MODE (PORTFCLR = 0x10)
#define DISPLAY_CHANGE_TO_DATA_MODE (PORTFSET = 0x10)

#define DISPLAY_ACTIVATE_RESET (PORTGCLR = 0x200)
#define DISPLAY_DO_NOT_RESET (PORTGSET = 0x200)

#define DISPLAY_ACTIVATE_VDD (PORTFCLR = 0x40)
#define DISPLAY_ACTIVATE_VBAT (PORTFCLR = 0x20)

#define DISPLAY_TURN_OFF_VDD (PORTFSET = 0x40)
#define DISPLAY_TURN_OFF_VBAT (PORTFSET = 0x20)

/* quicksleep:
   A simple function to create a small delay.
   Very inefficient use of computing resources,
   but very handy in some special cases. */
//written 2015 by F Lundevall
void quicksleep(int cyc) {
	int i;
	for(i = cyc; i > 0; i--);
}

int getbtns(void){

int returnValue = PORTD & 0xe0;
/*
  volatile int* ptr = (volatile int*) PORTD;
  returnValue = *ptr & 0xe0; 
  */
	returnValue = returnValue >> 5;

	return returnValue;
}
//written 2015 by F Lundevall
uint8_t spi_send_recv(uint8_t data) {
	while(!(SPI2STAT & 0x08)); // while no underrun condition
	SPI2BUF = data;
	while(!(SPI2STAT & 1));    // while buffer is not full
	return SPI2BUF;
}

//written 2015 by F Lundevall
void display_update(void) {

  int i;
  for(i = 0; i < 4; i++) {
    DISPLAY_CHANGE_TO_COMMAND_MODE;
    spi_send_recv(0x22);
    spi_send_recv(i);
    
    spi_send_recv(0x0);
    spi_send_recv(0x10);
    
    DISPLAY_CHANGE_TO_DATA_MODE;
    
  }
}

//written 2015 by F Lundevall
void display_init(void) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;
	quicksleep(10);
	DISPLAY_ACTIVATE_VDD;
	quicksleep(1000000);
	
	spi_send_recv(0xAE);
	DISPLAY_ACTIVATE_RESET;
	quicksleep(10);
	DISPLAY_DO_NOT_RESET;
	quicksleep(10);
	
	spi_send_recv(0x8D);
	spi_send_recv(0x14);
	
	spi_send_recv(0xD9);
	spi_send_recv(0xF1);
	
	DISPLAY_ACTIVATE_VBAT;
	quicksleep(10000000);
	
	spi_send_recv(0xA1);
	spi_send_recv(0xC8);
	
	spi_send_recv(0xDA);
	spi_send_recv(0x20);
	
	spi_send_recv(0xAF);
}

//prints the game field
void display_frame()
{
  int x = 66; 
  int y = 0;

while( y < 31 ){
  pixel_set(x, y);
  y++;
}

  while( x < 127 ){
    pixel_set(x, y);
    x++;
  }

while( y > 0 ){
  pixel_set(x, y);
  y--;
}

while( x > 65 ){
  pixel_set(x, y);
  x--;
}
}



// prints out entire array of pixels
void display_print() {
    int i, j;
    int t = 0;

    for(i = 0; i < 4; i++) {
        DISPLAY_CHANGE_TO_COMMAND_MODE;
        spi_send_recv(0x22);
        spi_send_recv(i);

        spi_send_recv(0 & 0xF);
        spi_send_recv(0x10 | ((0 >> 4) & 0xF));
        DISPLAY_CHANGE_TO_DATA_MODE;

        for(j = t; j <((i+1) * 128); j++){
            spi_send_recv(*(dis_ptr + j));
        }
        t +=128;
    }
}

// display the score on "dis_array" when the game is over
void display_score()
{

if(score != 0)
  logic_to_pixel_set(1,10);



    uint8_t digit_1 = score & 1;
    display_digit2(digit_1, 0);
    digit_1 = score & 2;
    display_digit2(digit_1, 4);
    digit_1 = score & 4;
    display_digit2(digit_1, 8);
    digit_1 = score & 8;
    display_digit2(digit_1, 12);

    digit_1 = score & 16;
    display_digit2(digit_1, 16);
    digit_1 = score & 32;
    display_digit2(digit_1, 20);
    digit_1 = score & 64;
    display_digit2(digit_1, 24);
    digit_1 = score & 128;
    display_digit2(digit_1, 28);

}

// uses pixel_set to set the correct pixel
void display_digit2(uint8_t digit, uint8_t offset)
{
  
  if(!digit)
  {
  
    pixel_set(60, offset);
    pixel_set(61, offset);
    pixel_set(62, offset);
    pixel_set(63, offset);

    pixel_set(60, offset+1);
    pixel_set(60, offset+2);

    pixel_set(61, offset+2);
    pixel_set(62, offset+2);
    pixel_set(63, offset+2);

    pixel_set(63, offset+1);
    pixel_set(63, offset+2);
  }
  else
  {
    pixel_set(60, offset);
    pixel_set(61, offset);
    pixel_set(62, offset);
    pixel_set(63, offset);
    pixel_set(60,offset+1);  
  }
    
    
}


// turns on pixel at given location
void pixel_set(unsigned int x, uint8_t y)
{

if(y < 8)
  x = x;

else if(y > 7 && y < 16){
  x = x + 128;
  y = y - 8;
}

else if(y > 15 && y < 24 ){
  x =  x + 256;
  y = y - 16;
}

else if(y > 23){
  x =  x + 384;
  y = y - 24;
}

switch(y)
{
    case 0:
    *(dis_ptr + x) |= 0x01;
    break;

    case 1:
    *(dis_ptr + x) |= 0x02;
    break;

    case 2:
    *(dis_ptr + x) |= 0x04;
    break;

    case 3:
    *(dis_ptr + x) |= 0x08;
    break;

    case 4:
    *(dis_ptr + x) |= 0x10;
    break;

    case 5:
    *(dis_ptr + x) |= 0x20;
    break;

    case 6:
    *(dis_ptr + x) |= 0x40;
    break;

    case 7:
    *(dis_ptr + x) |= 0x80;
    break;
}

}
//clears pixel att given location
void pixel_clr(unsigned int x,uint8_t y)
{

if(y < 8)
  x = x;

else if(y > 7 && y < 16){
  x = x + 128;
  y = y - 8;
}

else if(y > 15 && y < 24 ){
  x =  x + 256;
  y = y - 16;
}

else if(y > 23){
  x =  x + 384;
  y = y - 24;
}

switch(y)
{
    case 0:
    *(dis_ptr + x) &= ~(0x01);
    break;

    case 1:
    *(dis_ptr + x) &= ~(0x02);
    break;

    case 2:
    *(dis_ptr + x) &= ~(0x04);
    break;

    case 3:
    *(dis_ptr + x) &= ~(0x08);
    break;

    case 4:
    *(dis_ptr + x) &= ~(0x10);
    break;

    case 5:
    *(dis_ptr + x) &= ~(0x20);
    break;

    case 6:
    *(dis_ptr + x) &= ~(0x40);
    break;

    case 7:
    *(dis_ptr + x) &= ~(0x80);
    break;
}

}


