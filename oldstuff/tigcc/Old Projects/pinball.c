// C Source File
// Created 25/Nov/03; 22:12:42

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files
#include "extgraph.h"         // Include the extgraph library
#include "JudeCN.h"						// Include Jude Nelson's Header File

/*============== Global Defines ===============*/
// put any #define stuff here or enumerations
enum keys {UP,DOWN,RIGHT,LEFT,ESC};

/*=============== Global Types ================*/
// space for typedefs
typedef struct {
	int x_pos;
	int y_pos;
	int past_x;
	int past_y;
	int x_velocity;
	int y_velocity;
} BALL_DATA;

/*============= Global Variables ==============*/
BALL_DATA ball_data = {0,0,0,0,0,0};
int i=0;
int gravity = 1;
unsigned short temp_sprite_l[] = {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
																0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};
unsigned short temp_sprite_d[] = {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
																0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};
/*unsigned short temp_sprite_f[] = {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,
																0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000};*/
static INT_HANDLER interrupt1, interrupt5;
// more variables

/*=========== Function Prototypes =============*/
void reaction(void);
void collide(void);
void setup(void);
void drawBall(void);
void closing(void);
void degrade(short);
void delay(short);
void testCollide(void);
short testKeys(void);
// more protoypes

/*========= The Interface Routines ===========*/
// routines for drawing menus, hi-scores, help, etc
void degrade(short d)
{
  short a;
  for(a=0 ; a<7 ; a++) {
		switch (d) {
		  case 1:
				OSContrastUp();
				delay(2000);	
			  break;
			case 2:
				OSContrastDn();
				delay(2000);	
				break;
		}	  	
  }
}
void delay(short l)
{
   short a, b;
  for(a=0 ; a<=l; a++)
  	for(b=0 ; b<=40 ; b++); 	
}

short testKeys(void) {
	if (_rowread(0x01)&0x01 && ball_data.y_pos > 0) return UP;
	if (_rowread(0x01)&0x02 && ball_data.x_pos > 0) return LEFT;
	if (_rowread(0x01)&0x04 && ball_data.y_pos < 100) return DOWN;
	if (_rowread(0x01)&0x08 && ball_data.x_pos < 160) return RIGHT;
	if (_rowread(0x40)&0x01) return ESC;
	return -1;
}

/*======= Setup and Closing Routines =========*/
//initialize and deinitialize grayscale, the interrupts, etc
void setup(void) {
	interrupt1 = GetIntVec(AUTO_INT_1);
	interrupt5 = GetIntVec(AUTO_INT_5);
	SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
	SetIntVec(AUTO_INT_5,DUMMY_HANDLER);
	clrscr();
	GrayOn();
}

void closing(void) {
	GrayOff();
	//clrscr();
	SetIntVec(AUTO_INT_1,interrupt1);
	SetIntVec(AUTO_INT_5,interrupt5);
}

/*========== The Sprite Routines =============*/
// guess what goes here! YOURE RIGHT!!! the sprite drawing functions
void drawBall(void) {
	unsigned short ball_sprite_l[] = {0x0000,0x0000,0x0000,0x0000,0x03C0,0x07E0,0x0FF0,0x0FF0,
																		0x0FF0,0x0FF0,0x07E0,0x03C0,0x0000,0x0000,0x0000,0x0000};
	unsigned short ball_sprite_d[] = {0x0000,0x0000,0x0000,0x0000,0x03C0,0x07E0,0x0FF0,0x0FF0,
																		0x0FF0,0x0FF0,0x07E0,0x03C0,0x0000,0x0000,0x0000,0x0000};
	GraySprite16_XOR(ball_data.x_pos,ball_data.y_pos,16,ball_sprite_l,ball_sprite_d,GetPlane(LIGHT_PLANE),GetPlane(DARK_PLANE));
}

/*============ The Ball Physics ==============*/
// these will be everything about the physics of the pinball game
void testCollide(void) {
	// the basic premise of this function is to test whether the ball has hit anything or not
	// the way i will accomplish this is by collecting a 16x16 sprite at the ball's location, sans the ball
	// then i will test collision between the ball and that sprite
	// if the two hit, then i will run a routine that bounces the ball
	drawBall();
	unsigned short ball_sprite_f[] = {0x0000,0x0000,0x0000,0x0000,0x03C0,0x07E0,0x0FF0,0x0FF0,
																	0x0FF0,0x0FF0,0x07E0,0x03C0,0x0000,0x0000,0x0000,0x0000};
	Sprite16Get(ball_data.x_pos,ball_data.y_pos,16,GetPlane(LIGHT_PLANE),temp_sprite_l);
	Sprite16Get(ball_data.x_pos,ball_data.y_pos,16,GetPlane(DARK_PLANE),temp_sprite_d);
	drawBall();
	//temp_sprite_f = temp_sprite_l | temp_sprite_d;
	if (TestCollide16(ball_data.x_pos,ball_data.y_pos,ball_data.x_pos,ball_data.y_pos,16,ball_sprite_f,temp_sprite_l) ||
			TestCollide16(ball_data.x_pos,ball_data.y_pos,ball_data.x_pos,ball_data.y_pos,16,ball_sprite_f,temp_sprite_d)) {
		DrawStr(0,0,"They Hit!",A_REVERSE);
		// of course this would have later a lot more, where i would make the ball bounce
	}	
}

/*=========== THE MAIN FUNCTION!!! ===========*/
void _main(void)
{
// first this will intialize a few locals
// then it should run the opening routines
// draw the menu
// go into the game
// run the closing routines
// thats it
	
	setup();
	degrade(1);
	drawBall();
	//drawWalls();
	degrade(2);
	while (1) {
		// the game loop should add the gravity constant to the ball's y change
		// then it should draw(erase) the ball
		// then it should move the ball
		// then it test for collisions
		// then it should redraw the ball
		// then it should check for keys pressed
		// then it should delay the game
		// then redraw the score
		// also check if the ball is in the dead area and kill it
		// check the ball's position and scroll the screen to match it
		addGravity();
		drawBall();
		if (testCollide()) bounce();
		drawBall();
		// key check spot
		delay(game_speed);
		drawScore();
	}
	/*closing();
	ngetchx();*/
}
