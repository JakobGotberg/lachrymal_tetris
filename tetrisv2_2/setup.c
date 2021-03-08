#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header_file.h" 

uint8_t timeout_counter = 0;
uint8_t fall_speed = 5;	//lower fall_speeds -> faster fall
uint8_t index = 0;
int a = 0;
int b = 1;
uint8_t i = 0;


void timer_setup( void )
{
TMR2 = 0;
T2CON |= 0x70;  // preset with 256
PR2 = 31250;     // re is (80 * 10^6)/256/10
T2CONSET = 0x8000; //starts timer

IEC(0) = 0x100;               // start interrupt with flag 8
IPC(2) = 0x00000014;         // sets priorety to 5
enable_interrupt();

  return;
}


void user_isr( void )
{
// stops timer
T2CONCLR = 0x8000; 
// resets timer, no need for it really
TMR2 = 0;         

// points to IFC(0)	
volatile int* ifs_ptr = (volatile int*) 0xbf881030;  

// checks input from buttons
btn_check();


// a and b are locks, to prevent "dubble clicking", the buttons are very sensitive.
if(rotate_flag || go_left_flag || go_right_flag)
{
	if(!a && b)
	{
	animation();
	quicksleep(10000);
	}
	else if(a && !b)
	{
		a = 0;
		b = 1;
	}
}

// timer 2 interrupt flag
if((*ifs_ptr & 0x100) != 0)
{ 
timeout_counter++;	
i++;
// clears flag    0x18000108 -> 0x18000008	            
*ifs_ptr &= ~(0x100);    
}


// again, a way to prevent dubble clicking
if(index == 3)
{
	rotate_lock = 0;
	index = 0;
}




// falls down, if shape hits floor or other shape; new shape falls from roof 
if( timeout_counter >= fall_speed ){
	fall_down();
	timeout_counter = 0;
}

if(i == 250)
{
	if(fall_speed > 1)
	{
		fall_speed--;
	}

i = 0;
}

T2CONSET = 0x8000;   // start timer
  return;

}
