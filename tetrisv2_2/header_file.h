/* mipslab.h
   Header file for all labs.
   This file written 2015 by F Lundevall
   Some parts are original code written by Axel Isaksson

   Latest update 2015-08-28 by F Lundevall

   For copyright and licensing, see file COPYING */

/* Declare display-related functions from mipslabfunc.c */
//void display_image(int x, const uint8_t *data);
void display_init(void);
//void display_string(int line, char *s);
void display_update(void);
uint8_t spi_send_recv(uint8_t data);

/* Declare lab-related functions from mipslabfunc.c */
//char * itoaconv( int num );
//void labwork(void);
//int nextprime( int inval );
void quicksleep(int cyc);
//void tick( unsigned int * timep );

/* Declare display_debug - a function to help debugging.

   After calling display_debug,
   the two middle lines of the display show
   an address and its current contents.

   There's one parameter: the address to read and display.

   Note: When you use this function, you should comment out any
   repeated calls to display_image; display_image overwrites
   about half of the digits shown by display_debug.
*/
//void display_debug( volatile int * const addr );

/* Declare bitmap array containing font */
//extern const uint8_t const font[128*8];
/* Declare bitmap array containing icon */
//extern const uint8_t const icon[128];
/* Declare text buffer for display output */
//extern char textbuffer[4][16];

/* Declare functions written by students.
   Note: Since we declare these functions here,
   students must define their functions with the exact types
   specified in the laboratory instructions. */
/* Written as part of asm lab: delay, time2string */
//void delay(int);
//void time2string( char *, int );
/* Written as part of i/o lab: getbtns, getsw, enable_interrupt */
int getbtns(void);
//int getsw(void);
void enable_interrupt(void);

/********************************/
//project functions, global variables och arrays
/********************************/

typedef struct shape{

   //ignore certian cubes in the shape when checking collision
   uint8_t ignore_down_cube1;
   uint8_t ignore_down_cube2;
   uint8_t ignore_down_cube3;
   uint8_t ignore_down_cube4;

   uint8_t ignore_right_cube1;
   uint8_t ignore_right_cube2;
   uint8_t ignore_right_cube3;
   uint8_t ignore_right_cube4;

   uint8_t ignore_left_cube1;
   uint8_t ignore_left_cube2;
   uint8_t ignore_left_cube3;
   uint8_t ignore_left_cube4;

   //cube 1
   uint8_t r1;
   uint8_t b1;
   //cube 2
   uint8_t r2;
   uint8_t b2;
   //cube 3
   uint8_t r3;
   uint8_t b3;
   //cube 4
   uint8_t r4;
   uint8_t b4;

   //ex when bottom_cube == 20, it can't fall down anymore
   uint8_t bottom_cube;
   uint8_t left_cube;
   uint8_t right_cube;


   //every shape has an index. A total of 19 shapes
   uint8_t shape_index;

}shape;

void shape_init(uint8_t r, uint8_t b);
shape shape_handler(uint8_t s);
shape rotation_handler(void);

extern shape obj;
extern shape T0;
extern shape T1;
extern shape T2;
extern shape T3;
extern shape O;
extern shape L0;
extern shape L1;
extern shape L2;
extern shape L3;
extern shape J0;
extern shape J1;
extern shape J2;
extern shape J3;
extern shape S0;
extern shape S1;
extern shape Z0;
extern shape Z1;
extern shape I0;
extern shape I1;

extern shape temp;

extern uint8_t new_shape_flag;


extern uint8_t obj_row;
extern uint8_t obj_block;
extern uint8_t go_left_flag;
extern uint8_t go_right_flag;
extern uint8_t rotate_flag;

extern uint8_t rotate_lock;

extern uint8_t init_rotation;
extern uint8_t game;
extern uint8_t score;
extern uint8_t rng_global_value;
extern uint8_t rng_counter;

extern uint8_t dis_arr[512];
extern uint8_t* dis_ptr;
extern uint8_t logic_array[200];
extern uint8_t* log_ptr;
extern uint8_t collision_array[20][10];

void display_frame(void);
void display_print(void);

void screen_fill(void);
void row_fill(uint8_t _row);


void display_digit2(uint8_t digit, uint8_t offset);
void display_score(void);

void timer_setup( void );

void fall_down(void);
void go_right(void);
void go_left(void);

void btn_check(void);

void animation(void);

void pixel_set(unsigned int x, uint8_t y);
void pixel_clr(unsigned int x, uint8_t y);

void logic_to_pixel_set( uint8_t row, uint8_t block );
void logic_to_pixel_clr( uint8_t row, uint8_t block );

void display_shape(void);
void delete_shape(void);

void display_number(uint8_t number, uint8_t row, uint8_t col);
void display_digit(uint8_t number, uint8_t row, uint8_t col);


void row_clear(uint8_t _row);
void move_rows_down(uint8_t clearedRow);
void row_check(void);


uint8_t collision_check(uint8_t row, uint8_t block);

uint8_t collision_check_down(void);
uint8_t collision_check_rigth(void);
uint8_t collision_check_left(void);
void rotation_control(void);


