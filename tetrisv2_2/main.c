/* mipslabmain.c

   This file written 2015 by Axel Isaksson,
   modified 2015, 2017 by F Lundevall

   Latest update 2017-04-21 by F Lundevall

   For copyright and licensing, see file COPYING */

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header_file.h"  /* Declatations for these labs */


int main(void) {
        /*
	  This will set the peripheral bus clock to the same frequency
	  as the sysclock. That means 80 MHz, when the microcontroller
	  is running at 80 MHz. Changed 2017, as recommended by Axel.
	*/
	SYSKEY = 0xAA996655;  /* Unlock OSCCON, step 1 */
	SYSKEY = 0x556699AA;  /* Unlock OSCCON, step 2 */
	while(OSCCON & (1 << 21)); /* Wait until PBDIV ready */
	OSCCONCLR = 0x180000; /* clear PBDIV bit <0,1> */
	while(OSCCON & (1 << 21));  /* Wait until PBDIV ready */
	SYSKEY = 0x0;  /* Lock OSCCON */
	
	/* Set up output pins */
	AD1PCFG = 0xFFFF;
	ODCE = 0x0;
	TRISECLR = 0xFF;
	PORTE = 0x0;
	
	/* Output pins for display signals */
	PORTF = 0xFFFF;
	PORTG = (1 << 9);
	ODCF = 0x0;
	ODCG = 0x0;
	TRISFCLR = 0x70;
	TRISGCLR = 0x200;
	
	/* Set up input pins */
	TRISDSET = (1 << 8);
	TRISFSET = (1 << 1);
	
	/* Set up SPI as master */
	SPI2CON = 0;				// clears spicon


	SPI2BRG = 4;				// sets clock
	/* SPI2STAT bit SPIROV = 0; */
	SPI2STATCLR = 0x40;			// overflow flag cleared
	/* SPI2CON bit CKP = 1; */
        SPI2CONSET = 0x40;		// clock polairty set to 0?
	/* SPI2CON bit MSTEN = 1; */
	SPI2CONSET = 0x20;			// master mode on
	/* SPI2CON bit ON = 1; */
	SPI2CONSET = 0x8000;		// bit 15, SPI Peripheral is enabled

	//set btns as inputs
	PORTDSET = 0x00e0;
	
	// starts and updates the OLED-screen
	display_init();
	display_update();

	//index for the next shape
	uint8_t index_for_shape = 2;
	//gives correct data to the shapes
	shape_init(0,0);
	// gives "dis_array" a frame around the gamefiled
	display_frame();
	//"obj" is the placeholder for the current shape
	obj = T0;
	// distpalys the current shape on the gamefiled
	display_shape();
	//stesup timer and interrupt
	timer_setup();
	
	while(game)		
	{
	
		// if current shape can't move down, a new shape is displayed on the gamefiled
		if(new_shape_flag == 1){
			go_left_flag = 0;
			go_right_flag = 0;
			rotate_flag = 0;
			obj = shape_handler(index_for_shape);
			new_shape_flag = 0;
			display_shape();
			index_for_shape++;
			if(index_for_shape > 7)
				index_for_shape = 1;

			// if the new shape can't move down directly after it has been displayed, the game is over.
			if( collision_check_down() )
			{ game = 0; }

		}
		
		// prints out "dis_array" to the OLED-display
		display_print();

		
	}
	
	//when game = 0, prints score and fills gamefiled with white blocks
	screen_fill();
	display_score();
	display_print();
	

	return 0;
}


