

#include <stdint.h>   /* Declarations of uint_32 and the like */
#include <pic32mx.h>  /* Declarations of system-specific addresses etc */
#include "header_file.h"  /* Declatations for these labs */

// game logic


// sets cube on gamefiled (logic_array) to 1, a cube is 3x3 pixels
// also sets corresponding cube in collision_array to 1
void logic_to_pixel_set( uint8_t row, uint8_t block )
{
	uint8_t pixel_row = row + 66 + ((row -1 ) * 2);
	uint8_t pixel_block = block * 3 -2;
	uint8_t collision_row = row-1;
	uint8_t collision_block = 10 - block;

	pixel_set(pixel_row, pixel_block);
	pixel_set(pixel_row, pixel_block + 1);
	pixel_set(pixel_row, pixel_block + 2);

	pixel_set(pixel_row + 1, pixel_block);
	pixel_set(pixel_row + 1, pixel_block + 1);
	pixel_set(pixel_row + 1, pixel_block + 2);

	pixel_set(pixel_row + 2, pixel_block);
	pixel_set(pixel_row + 2, pixel_block + 1);
	pixel_set(pixel_row + 2, pixel_block + 2);

	collision_array[collision_row][collision_block] = 1;

}

// sets cube on gamefiled (logic_array) to 0, a cube is 3x3 pixels
// also sets corresponding cube in collision_array to 0
void logic_to_pixel_clr( uint8_t row, uint8_t block )
{
	uint8_t pixel_row = row + 66 + ((row -1 ) * 2);
	uint8_t pixel_block = block * 3 -2;
	uint8_t collision_row = row-1;
	uint8_t collision_block = 10 - block;

	pixel_clr(pixel_row, pixel_block);
	pixel_clr(pixel_row, pixel_block + 1);
	pixel_clr(pixel_row, pixel_block + 2);

	pixel_clr(pixel_row + 1, pixel_block);
	pixel_clr(pixel_row + 1, pixel_block + 1);
	pixel_clr(pixel_row + 1, pixel_block + 2);

	pixel_clr(pixel_row + 2, pixel_block);
	pixel_clr(pixel_row + 2, pixel_block + 1);
	pixel_clr(pixel_row + 2, pixel_block + 2);

	collision_array[collision_row][collision_block] = 0;

}


//displays the current shape to the gamefiled
void display_shape()
{
	logic_to_pixel_set(obj.r1, obj.b1);
	logic_to_pixel_set(obj.r2, obj.b2);
	logic_to_pixel_set(obj.r3, obj.b3);
	logic_to_pixel_set(obj.r4, obj.b4);
}
// removes current shape from gamefiled
void delete_shape()
{
	logic_to_pixel_clr(obj.r1, obj.b1);
	logic_to_pixel_clr(obj.r2, obj.b2);
	logic_to_pixel_clr(obj.r3, obj.b3);
	logic_to_pixel_clr(obj.r4, obj.b4);
}

// checks inputs from buttons, PORTD
void btn_check()
{
	

	if(getbtns() & 1) 
	{
		go_left_flag = 1;			
	}

	if(getbtns() & 2)
	{
		go_right_flag = 1;			
	}

	if(getbtns() & 4)
	{
		rotate_flag = 1;		
	}
	

}


// moves current shape: left, right and rotate
void animation()
{



		if(go_left_flag)
		{
			go_left();

			go_right_flag = 0;

		}

		else if(go_right_flag)
		{
			go_right();
			go_left_flag = 0;
		}
		
		// && !rotate_lock 
		else if(rotate_flag)
		{
			delete_shape();
			rotation_control();
			rotate_flag = 0;
			display_shape();
			rotate_lock = 1;
		}
	rotate_flag = 0;



}


// checks collision with other blocks, using the collision_array
uint8_t collision_check(uint8_t row, uint8_t block)
{
	int collision_row = row - 1;
	int collision_block = 10 - block;

	return collision_array[collision_row][collision_block];
}

// checks collision with blocks to the right of current shape
uint8_t block_right(uint8_t row, uint8_t block)
{
	block--;
	int collision_row = row - 1;
	int collision_block = 10 - block;

	return collision_array[collision_row][collision_block];
}
// checks collision with blocks to the left of current shape
uint8_t block_left(uint8_t row, uint8_t block)
{
	block++;
	int collision_row = row - 1;
	int collision_block = 10 - block;

	return collision_array[collision_row][collision_block];
}
// checks collision with blocks under current shape
uint8_t block_down(uint8_t row, uint8_t block)
{
	row++;
	int collision_row = row - 1;
	int collision_block = 10 - block;

	return collision_array[collision_row][collision_block];
}

// checks collision with blocks to the left of current shape
uint8_t collision_check_left()
{	
	
	if(!obj.ignore_left_cube1 && block_left(obj.r1,obj.b1))
	{
		//logic_to_pixel_set(1,1);
			return 1;
	}
	
	if( !obj.ignore_left_cube2 && block_left(obj.r2,obj.b2) )
	{
		//logic_to_pixel_set(10,3);
			return 1;
	}
	

	if( !obj.ignore_left_cube3 && block_left(obj.r3,obj.b3))
	{
		//logic_to_pixel_set(20,1);
			return 1;
	}
	

	if( !obj.ignore_left_cube4 && block_left(obj.r4,obj.b4))
	{
		//logic_to_pixel_set(20,10);
			return 1;
	}
	
return 0;

}

// checks collision with blocks to the right of current shape
uint8_t collision_check_right()
{

	if( !obj.ignore_right_cube1 && block_right(obj.r1,obj.b1))
	{
		//logic_to_pixel_set(1,1);
			return 1;
	}
	
	if( !obj.ignore_right_cube2 && block_right(obj.r2,obj.b2) )
	{
		//logic_to_pixel_set(10,3);
			return 1;
	}
	

	if( !obj.ignore_right_cube3 && block_right(obj.r3,obj.b3))
	{
		//logic_to_pixel_set(20,1);
			return 1;
	}
	

	if( !obj.ignore_right_cube4 && block_right(obj.r4,obj.b4))
	{
		//logic_to_pixel_set(20,10);
			return 1;
	}

	return 0;
}

// checks collision with blocks under current shape
uint8_t collision_check_down()
{


	if( !obj.ignore_down_cube1 && block_down(obj.r1,obj.b1))
	{
			return 1;
	}
	
	if( !obj.ignore_down_cube2 && block_down(obj.r2,obj.b2) )
	{
			return 1;
	}
	

	if( !obj.ignore_down_cube3 && block_down(obj.r3,obj.b3))
	{
			return 1;
	}
	

	if( !obj.ignore_down_cube4 && block_down(obj.r4,obj.b4))
	{
			return 1;
	}


	return 0;
}




// goes left, once per btn-press, as long as there is no collision
void go_left()
{
	
//&& !( collision_check_left() )
	if((obj.left_cube != 10) && !( collision_check_left())) {
		delete_shape();
		obj.b1++;
		obj.b2++;
		obj.b3++;
		obj.b4++;
		obj.left_cube++;
		obj.right_cube++;
		display_shape();
	}
	
	go_left_flag = 0;

	
}


// goes rigth, once per btn-press, as long as there is no collision
void go_right()
{
	//rightCollision = collision_check_right();

	if(obj.right_cube != 1 && !( collision_check_right() )) {
		delete_shape();
		obj.b1--;
		obj.b2--;
		obj.b3--;
		obj.b4--;
		obj.right_cube--;
		obj.left_cube--;
		display_shape();
		go_right_flag = 0;
	}
	
}


//falls down, faster and faster. only falls if the 
void fall_down()
{
	
	if(obj.bottom_cube != 20 && !collision_check_down()){
		delete_shape();
		obj.r1++;
		obj.r2++;
		obj.r3++;
		obj.r4++;
		obj.bottom_cube++;
		display_shape();
		
	}
	else{
		row_check();
		new_shape_flag = 1;
	}

}
/*

//in lack of a RNG (random number generator) we resorted to using a simpel list. 
// This function returns the next shape to enter the gamefield.
shape shape_handler(uint8_t s)
{
	switch(s)
	{
		case 1:
			return O;
			break;

		case 2:
			return L1;
			break;

		case 3:
			return I1;
			break;

		case 4:
			return J3;
			break;

		case 5:
			return T0;
			break;

		case 6:
			return Z0;
			break;

		case 7:
			return S0;
			break;
	}
}
*/
shape shape_handler(uint8_t s)
{

	if(s == 1)
		return O;
 
	else if(s == 2)
		return L1;

	 else if(s == 3)
		return I1;
	
	else if(s == 4)
		return J3;

	else if(s == 5)
		return T0;

	 else if(s == 6)
		return Z0;

	 else if(s == 7)
		return S0;
	
}


// checks if rotation is possible without collision. If it is, it changes current shape to another one.
void rotation_control()
{

shape_init(obj.r1, obj.b1);

	switch(obj.shape_index)
	{
		case 1:
		shape_init(0,0);
			return;

		case 2: //L0 
			temp = L1;
		break;

		case 3: //L1
			temp = L2;
		break;

		case 4:
			temp = L3;
		break;

		case 5:
			temp = L0;
		break;

		case 6:
			temp = I1;
		break;

		case 7:
			temp = I0;
		break;

		case 8:
			temp = J1;
		break;

		case 9:
			temp = J2;
		break;

		case 10:
			temp = J3;
		break;

		case 11:
			temp = J0;
		break;

		case 12:
			temp = T1;
		break;

		case 13:
			temp = T2;
		break;

		case 14:
			temp = T3;
		break;

		case 15:
			temp = T0;
		break;

		case 16:
			temp = Z1;
		break;

		case 17:
			temp = Z0;
		break;

		case 18:
			temp = S1;
		break;

		case 19:
			temp = S0;
		break;

	}

	
		if(!collision_check(temp.r2,temp.b2) && !collision_check(temp.r3,temp.b3) && !collision_check(temp.r4,temp.b4) 
			&& (temp.b2 > 0) && (temp.b3 > 0) && (temp.b4 > 0) && (temp.b2 < 11) && (temp.b3 < 11) && (temp.b4 < 11))
		{
			obj = temp;
		}

		shape_init(0,0);
		
}

//checks if a row is full
void row_check()
{
	uint8_t row = obj.r1;
	int clear = 0;
	int i;
	int p;
	for(i = row; i < row + 4; i++){
		for(p = 1; p < 11; p++){
			clear += collision_check(i, p);
		}
		if(clear == 10){
			row_clear(i);
			score++;
		}
		clear = 0;
	}	
}

//clears the row and 
void row_clear(uint8_t _row)
{
	int i;
	for(i = 1; i < 11; i++){
		logic_to_pixel_clr(_row, i);
	}
	
	move_rows_down(_row);
}

void move_rows_down(uint8_t clearedRow){
	int i;
	int p;
	for(i = clearedRow; i > 0; i--){
		for(p = 1; p < 11; p++){
			int moveDown = collision_check(i-1, p);
			if(moveDown == 1){
				logic_to_pixel_clr(i-1, p);
				logic_to_pixel_set(i, p);
			}	
		}
	}
}

void row_fill(uint8_t _row)
{
	int i;
	for(i = 1; i < 11; i++){
		logic_to_pixel_set(_row, i);
	}
}

void screen_fill()
{
	int i;
	for(i = 1; i < 21; i++){
		row_fill(i);
	}
}

