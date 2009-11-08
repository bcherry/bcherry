// C Source File
// Written by Jude Nelson
// Created 5/20/2003; 9:56:35 PM *****this not to be altered*****
// Contact:  judecn@aol.com
// Enjoy my pinball game!

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS					// Compile for TI-92 Plus
#define USE_V200							// Compile for Voyage 200

#define ENABLE_ERROR_RETURN   // Enable Returning Errors to AMS

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define MIN_AMS 205           // Compile for AMS 2.05 or higher

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All TIGCC Header Files
#include "extgraph.h"					// Include ExtGraph functions

#define BigBumper 1
#define RectBumper 2
#define SmallBumper 3
#define BigLeftDeltoid 4
#define BigRightDeltoid 5
#define SmallLeftDeltoid 6
#define SmallRightDeltoid 7
#define Teleporter 8
#define Flag 9
#define BallEater 10
#define Accelerator 11
#define BinaryBumper 12
#define LeftFlipper 1
#define RightFlipper -1

//global variables
static int tablepart = 1;
static int grayflag = 1;
static int initilized = 0;
static char *title, *str1, *str2, *str3, *str4, *str5, *str6, *str7, *str8, *str9, *str0;
static int machine = 0;
static float gravity = 5;
static int horizontalgravity = 0;
static int rightflipper = 0, leftflipper = 0, use_flags = 0, lkeypress = 0, rkeypress = 0;
char *doublescreen=0, *lightdoublescreen=0, *darkdoublescreen=0;
char cname[8], tname[20]; 

//handle vectors
INT_HANDLER save_vec_auto_int_1;

//function prototypes
void game(void);
void logo(void);
void detectcollision(short (*)[], short (*)[], short (*)[], short (*)[], float (*)[], float (*)[], float (*)[], float (*)[], float (*)[], float (*)[], short (*)[], short (*)[], short (*)[], unsigned long *[], unsigned short *[], unsigned char *[], short *, int *, int *);
void moveball(short (*)[], float (*)[], float (*)[], float (*)[], float (*)[], float (*)[], float (*)[], unsigned char *[], short *);
void leftflipperup(short (*)[], short (*)[], short (*)[], unsigned long *[]);
void leftflipperdown(short (*)[], short (*)[], short (*)[], unsigned long *[]);
void rightflipperup(short (*)[], short (*)[], short (*)[], unsigned long *[]);
void rightflipperdown(short (*)[], short (*)[], short (*)[], unsigned long *[]);
void moveplunger(short (*)[], float *, unsigned char *[]);
void getkey(short (*)[]);
void gamemenu(void);
void menu(short, short, short, short (*)[], short *, ...);
void drawbox(short, short, short, short, short);
void startup(void);
void cleanup(void);
void nullstring(char (*)[], unsigned long);
void draw(short (*)[], short (*)[], short (*)[], short (*)[], float (*)[], float (*)[], float (*)[], float (*)[], float (*)[], float (*)[], short (*)[], short (*)[], short (*)[], unsigned long *[], unsigned short *[], unsigned char *[]);
void GraySprite8 (short, short, short, char *, char *, short);
void GraySprite16 (short, short, short, short *, char *, short);
void GraySprite32 (short, short, short, long *, char *, short);

void fixstring(char (*string)[])
{
	unsigned int len = strlen((*string)), i, endflag = 0;
	for(i=0; i<len+1; i++) {
		if((*string)[i] == '\n') {(*string)[i] = 0; endflag = 1;}
		if(endflag==1) (*string)[i] = 0;
	}
}

void make_menu_box(short height, short width, short x, short y) {
	int i;
	for(i=x; i<=width+6; i++)
	{
		DrawLine(i,y,i,y+height,A_REVERSE);
	}
	drawbox(x,y,width,height,0);	
	drawbox(x+1,y+1,width-2,height-2,0);
	drawbox(x+3,y+3,width-6,height-6,0);
}

void clearfullscreen(void)
{
	ClrScr();
	if(grayflag == 1 && initilized == 1) {
		PortSet(lightdoublescreen, 239, 127+128);
		ClrScr();
		PortSet(darkdoublescreen, 239, 127+128);
		ClrScr();
		PortSet(LCD_MEM, 239, 127);
	}
}

//this function creates a machine that is dependent on the value of 
//the machine variable, which is set in the gamemenu() function
void getmachine(short (*objectflags)[], short (*objectx)[], short (*objecty)[], short (*ballflags)[], float (*ballx)[], float (*bally)[], float (*ballmovex)[], float (*ballmovey)[], float (*moveshiftx)[], float (*moveshifty)[], short (*launcherxy)[], short (*plungerxy)[], short (*cgravity)[], int machine)
{
	//Traditional machine
	if(machine == 1)
	{
		use_flags = 1;
		
		(*objectflags)[0] = 0;
		(*objectx)[0] = 0;
		(*objecty)[0] = 0;
	
		(*objectflags)[1] = 0;
		(*objectx)[1] = 0;
		(*objecty)[1] = 0;
	
		(*objectflags)[2] = BigBumper;
		(*objectx)[2] = 34;
		(*objecty)[2] = -80+100;

		(*objectflags)[3] = BigBumper;
		(*objectx)[3] = 64;
		(*objecty)[3] = -90+100;

		(*objectflags)[4] = BigBumper;
		(*objectx)[4] = 90;
		(*objecty)[4] = -84+100;

		(*objectflags)[5] = 0;
		(*objectx)[5] = 0;
		(*objecty)[5] = 0;

		(*objectflags)[6] = 0;
		(*objectx)[6] = 0;
		(*objecty)[6] = 0;

		(*objectflags)[7] = 0;
		(*objectx)[7] = 0;
		(*objecty)[7] = 0;
		
		(*objectflags)[8] = SmallBumper;
		(*objectx)[8] = 140-8;
		(*objecty)[8] = 0+100;
				
		(*objectflags)[9] = SmallBumper;
		(*objectx)[9] = 140-8;
		(*objecty)[9] = 8+100;
				
		(*objectflags)[10] = SmallBumper;
		(*objectx)[10] = 140-8;
		(*objecty)[10] = 16+100;
		
		(*objectflags)[11] = SmallBumper;
		(*objectx)[11] = 140-8;
		(*objecty)[11] = 24+100;

		(*objectflags)[12] = 0;
		(*objectx)[12] = 0;
		(*objecty)[12] = 0;

		(*objectflags)[13] = SmallBumper;
		(*objectx)[13] = 8;
		(*objecty)[13] = -16+100;

		(*objectflags)[14] = SmallBumper;
		(*objectx)[14] = 140-8;
		(*objecty)[14] = -8+100;

		(*objectflags)[15] = SmallBumper;
		(*objectx)[15] = 140-8;
		(*objecty)[15] = -16+100;

		(*objectflags)[16] = Accelerator;
		(*objectx)[16] = 160-16;
		(*objecty)[16] = -20+100;

		(*objectflags)[17] = 0;
		
		(*objectflags)[18] = 0;

		(*objectflags)[19] = 0;
		
		(*objectflags)[20] = 0;
			
		(*objectflags)[21] = 0;
	
		(*objectflags)[22] = 0;
		
		(*objectflags)[23] = 0;
		
		(*objectflags)[24] = BigLeftDeltoid;
		(*objectx)[24] = 0;
		(*objecty)[24] = 70-32+100;
	
		(*objectflags)[25] = BigRightDeltoid;
		(*objectx)[25] = 140-32;
		(*objecty)[25] = 70-32+100;
		
		(*objectflags)[26] = BigLeftDeltoid;
		(*objectx)[26] = 0;
		(*objecty)[26] = -(100-47)+100;
	
		(*objectflags)[27] = BigRightDeltoid;
		(*objectx)[27] = 140-32;
		(*objecty)[27] = -(100-47)+100;
	
		(*objectflags)[28] = SmallRightDeltoid;
		(*objectx)[28] = 160-16;
		(*objecty)[28] = -100+100;
		
		(*objectflags)[29] = SmallLeftDeltoid;
		(*objectx)[29] = 0;
		(*objecty)[29] = -100+100;
	
		(*objectflags)[30] = 0;
		
		(*objectflags)[31] = BallEater;
		(*objectx)[31] = 20;
		(*objecty)[31] = 100-12+100;
				
		(*objectflags)[32] = BallEater;
		(*objectx)[32] = 140-20-16;
		(*objecty)[32] = 100-12+100;
			
		(*objectflags)[33] = 0;
			
		(*objectflags)[34] = BallEater;
		(*objectx)[34] = 140-16;
		(*objecty)[34] = 100-30+100;
			
		(*objectflags)[35] = BallEater;
		(*objectx)[35] = 0;
		(*objecty)[35] = 100-30+100;
		
		(*objectflags)[36] = BallEater;
		(*objectx)[36] = 70-8;
		(*objecty)[36] = 100-12+100;
	
		(*objectflags)[37] = Teleporter;
		(*objectflags)[38] = Teleporter;
		(*objectx)[37] = 20;
		(*objecty)[37] = 20+100;
		(*objectx)[38] = 140-12-8-20;
		(*objecty)[38] = 20+100;
	
		(*objectflags)[39] = Flag;
		(*objectx)[39] = 32;
		(*objecty)[39] = -60+100;
			
		(*objectflags)[40] = Flag;
		(*objectx)[40] = 140-8-8-8-8-8-16;
		(*objecty)[40] = -60+100;
				
		(*objectflags)[41] = Flag;
		(*objectx)[41] = 34+20;
		(*objecty)[41] = -90+100;
		
		(*objectflags)[42] = Flag;
		(*objectx)[42] = 70-4;
		(*objecty)[42] = 100-12-8+100;
	
		(*objectflags)[43] = Flag;
		(*objectx)[43] = 64+4;
		(*objecty)[43] = -50+100;
		
		(*objectflags)[44] = Flag;
		(*objectx)[44] = 70-4;
		(*objecty)[44] = 60+100;
	
		(*objectflags)[65] = LeftFlipper;
		(*objectx)[65] = 30;
		(*objecty)[65] = 70-4+100;
		
		(*objectflags)[66] = RightFlipper;
		(*objectx)[66] = 102-24;
		(*objecty)[66] = 70-4+100;
		
		(*objectflags)[67] = LeftFlipper;
		(*objectx)[67] = 20;
		(*objecty)[67] = -20+100;
		
		(*objectflags)[68] = RightFlipper;
		(*objectx)[68] = 110-24;
		(*objecty)[68] = -20+100;
	
		(*launcherxy)[0] = 160-16;
		(*launcherxy)[1] = 60+100;
		(*cgravity)[0] = 0;
		(*cgravity)[1] = 70-8;
		(*cgravity)[2] = 50-8+100;
	}
	//detached machine
	if(machine == 2)
	{
		use_flags = 1;

		(*objectflags)[0] = BigBumper;
		(*objectx)[0] = 0;
		(*objecty)[0] = 10+100;
	
		(*objectflags)[1] = BigBumper;
		(*objectx)[1] = 160-16;
		(*objecty)[1] = 10+100;

		(*objectflags)[2] = BigBumper;
		(*objectx)[2] = 10;
		(*objecty)[2] = -80+100;

		(*objectflags)[3] = BigBumper;
		(*objectx)[3] = 150-16;
		(*objecty)[3] = -80+100;

		(*objectflags)[4] = 0;
		(*objectx)[4] = 0;
		(*objecty)[4] = -20;

		(*objectflags)[5] = 0;
		(*objectx)[5] = 160-16;
		(*objecty)[5] = -20;

		(*objectflags)[6] = 0;

		(*objectflags)[7] = 0;

		(*objectflags)[8] = 0;

		(*objectflags)[9] = 0;

		(*objectflags)[10] = SmallBumper;
		(*objectx)[10] = 0;
		(*objecty)[10] = 30+100;

		(*objectflags)[11] = SmallBumper;
		(*objectx)[11] = 160-8;
		(*objecty)[11] = 30+100;

		(*objectflags)[12] = SmallBumper;
		(*objectx)[12] = 0;
		(*objecty)[12] = 90+100;

		(*objectflags)[13] = SmallBumper;
		(*objectx)[13] = 160-8;
		(*objecty)[13] = 90+100;

		(*objectflags)[14] = SmallBumper;
		(*objectx)[14] = 0;
		(*objecty)[14] = -60+100;

		(*objectflags)[15] = SmallBumper;
		(*objectx)[15] = 160-8;
		(*objecty)[15] = -60+100;

		(*objectflags)[16] = 0;
		(*objectx)[16] = 80-8;
		(*objecty)[16] = -20;

		(*objectflags)[17] = 0;

		(*objectflags)[18] = 0;

		(*objectflags)[19] = 0;

		(*objectflags)[20] = 0;

		(*objectflags)[21] = 0;

		(*objectflags)[22] = 0;

		(*objectflags)[23] = 0;

		(*objectflags)[24] = BigLeftDeltoid;
		(*objectx)[24] = 88;
		(*objecty)[24] = 80-32+100;

		(*objectflags)[25] = BigRightDeltoid;
		(*objectx)[25] = 72-32;
		(*objecty)[25] = 80-32+100;

		(*objectflags)[26] = BigLeftDeltoid;
		(*objectx)[26] = 0;
		(*objecty)[26] = -(33)+100;

		(*objectflags)[27] = BigRightDeltoid;
		(*objectx)[27] = 160-32;
		(*objecty)[27] = -(33)+100;

		(*objectflags)[28] = SmallRightDeltoid;
		(*objectx)[28] = 80-8;
		(*objecty)[28] = -100+100;

		(*objectflags)[29] = 0;

		(*objectflags)[30] = 0;

		(*objectflags)[31] = 0;
	
		(*objectflags)[32] = BinaryBumper;
		(*objectx)[32] = 80-16;
		(*objecty)[32] = -50+100;
	
		(*objectflags)[33] = Flag;
		(*objectx)[33] = 30;
		(*objecty)[33] = 70+100;

		(*objectflags)[34] = Flag;
		(*objectx)[34] = 160-8-30;
		(*objecty)[34] = 70+100;

		(*objectflags)[35] = Flag;
		(*objectx)[35] = 30;
		(*objecty)[35] = 40+100;
	
		(*objectflags)[36] = Flag;
		(*objectx)[36] = 160-8-30;
		(*objecty)[36] = 40+100;

		(*objectflags)[37] = Flag;
		(*objectx)[37] = 30;
		(*objecty)[37] = -40+100;
	
		(*objectflags)[38] = Flag;
		(*objectx)[38] = 160-8-30;
		(*objecty)[38] = -40+100;

		(*cgravity)[0] = 0;
		(*cgravity)[1] = 80-8;
		(*cgravity)[2] = 10+100;
	
		(*objectflags)[39] = BallEater;
		(*objectx)[39] = 80-20-16;
		(*objecty)[39] = 100-16+100;

		(*objectflags)[40] = BallEater;
		(*objectx)[40] = 80+20;
		(*objecty)[40] = 100-16+100;

		(*objectflags)[41] = BallEater;
		(*objectx)[41] = 0;
		(*objecty)[41] = -100+100;

		(*objectflags)[42] = BallEater;
		(*objectx)[42] = 160-16;
		(*objecty)[42] = -100+100;

		(*objectflags)[43] = 0;

		(*objectflags)[44] = 0;
	
		(*objectflags)[45] = Teleporter;
		(*objectflags)[46] = Teleporter;
		(*objectx)[45] = 80-6;
		(*objecty)[45] = 40+100;
		(*objectx)[46] = 80-6;
		(*objecty)[46] = -50-12+100;

		(*objectflags)[65] = RightFlipper;
		(*objectx)[65] = 160-8-24;
		(*objecty)[65] = 80+100;
	
		(*objectflags)[66] = LeftFlipper;
		(*objectx)[66] = 0;
		(*objecty)[66] = 80+100;

		(*objectflags)[67] = RightFlipper;
		(*objectx)[67] = 80-8-24;
		(*objecty)[67] = -20+100;

		(*objectflags)[68] = LeftFlipper;
		(*objectx)[68] = 80;
		(*objecty)[68] = -20+100;

		(*launcherxy)[0] = 80-8;
		(*launcherxy)[1] = 60+100;

	}
	//symmetrical machine
	if(machine == 3)
	{
		use_flags = 1;
		
		(*objectflags)[0] = BigBumper;
		(*objectx)[0] = 60-16;
		(*objecty)[0] = -100+32+16+100;

		(*objectflags)[1] = BigBumper;
		(*objectx)[1] = 0;
		(*objecty)[1] = -100+32+100;

		(*objectflags)[2] = BigBumper;
		(*objectx)[2] = 160-16;
		(*objecty)[2] = -100+32+100;

		(*objectflags)[3] = BigBumper;
		(*objectx)[3] = 100;
		(*objecty)[3] = -100+32+16+100;

		(*objectflags)[4] = 0;

		(*objectflags)[5] = BigBumper;
		(*objectx)[5] = 60-16;
		(*objecty)[5] = 0+100;

		(*objectflags)[6] = BigBumper;
		(*objectx)[6] = 100;
		(*objecty)[6] = 0+100;

		(*objectflags)[7] = 0;

		(*objectflags)[8] = SmallBumper;
		(*objectx)[8] = 60-8;
		(*objecty)[8] = 100-8+100;

		(*objectflags)[9] = SmallBumper;
		(*objectx)[9] = 100;
		(*objecty)[9] = 100-8+100;

		(*objectflags)[10] = SmallBumper;
		(*objectx)[10] = 60-8;
		(*objecty)[10] = 100-8-16-8+100;

		(*objectflags)[11] = SmallBumper;
		(*objectx)[11] = 100;
		(*objecty)[11] = 100-8-16-8+100;

		(*objectflags)[12] = SmallBumper;
		(*objectx)[12] = 60-8;
		(*objecty)[12] = 100-8-16-8-16-8+100;

		(*objectflags)[13] = SmallBumper;
		(*objectx)[13] = 100;
		(*objecty)[13] = 100-8-16-8-16-8+100;

		(*objectflags)[14] = SmallBumper;
		(*objectx)[14] = 60-8;
		(*objecty)[14] = 100-8-16-8-16-8-16-8+100;

		(*objectflags)[15] = SmallBumper;
		(*objectx)[15] = 100;
		(*objecty)[15] = 100-8-16-8-16-8-16-8+100;

		(*objectflags)[16] = Accelerator;
		(*objectx)[16] = 0;
		(*objecty)[16] = -20+100;

		(*objectflags)[17] = RectBumper;
		(*objectx)[17] = 60-8;
		(*objecty)[17] = 100-16-8+100;

		(*objectflags)[18] = RectBumper;
		(*objectx)[18] = 100;
		(*objecty)[18] = 100-16-8+100;

		(*objectflags)[19] = RectBumper;
		(*objectx)[19] = 60-8;
		(*objecty)[19] = 100-16-8-16-8+100;

		(*objectflags)[20] = RectBumper;
		(*objectx)[20] = 100;
		(*objecty)[20] = 100-16-8-16-8+100;

		(*objectflags)[21] = RectBumper;
		(*objectx)[21] = 60-8;
		(*objecty)[21] = 100-16-8-16-8-16-8+100;

		(*objectflags)[22] = RectBumper;
		(*objectx)[22] = 100;
		(*objecty)[22] = 100-16-8-16-8-16-8+100;

		(*objectflags)[23] = Accelerator;
		(*objectx)[23] = 160-16;
		(*objecty)[23] = -20+100;

		(*objectflags)[24] = Teleporter;
		(*objectflags)[25] = Teleporter;
		(*objectx)[24] = 80-6;
		(*objecty)[24] = 20+100;
		(*objectx)[25] = 80-6;
		(*objecty)[25] = -20+100;
	
		(*objectflags)[26] = BigLeftDeltoid;
		(*objectx)[26] = 0;
		(*objecty)[26] = -100+100;

		(*objectflags)[27] = BigRightDeltoid;
		(*objectx)[27] = 160-32;
		(*objecty)[27] = -100+100;

		(*objectflags)[28] = BigRightDeltoid;
		(*objectx)[28] = 80-8-32;
		(*objecty)[28] = -32+100;

		(*objectflags)[29] = BigLeftDeltoid;
		(*objectx)[29] = 80+8;
		(*objecty)[29] = -32+100;

		(*objectflags)[30] = BinaryBumper;
		(*objectx)[30] = 80-16;
		(*objecty)[30] = -100+100;
	
		(*objectflags)[31] = Flag;
		(*objectx)[31] = 60-8-8-8-8-8-8;
		(*objecty)[31] = 52+100;

		(*objectflags)[32] = Flag;
		(*objectx)[32] = 60-8-8-8-8-8;
		(*objecty)[32] = 52+100;

		(*objectflags)[33] = Flag;
		(*objectx)[33] = 60-8-8-8-8;
		(*objecty)[33] = 52+100;

		(*objectflags)[34] = Flag;
		(*objectx)[34] = 100+8+8+8;
		(*objecty)[34] = 52+100;

		(*objectflags)[35] = Flag;
		(*objectx)[35] = 100+8+8+8+8;
		(*objecty)[35] = 52+100;

		(*objectflags)[36] = Flag;
		(*objectx)[36] = 100+8+8+8+8+8;
		(*objecty)[36] = 52+100;
	
		(*cgravity)[0] = 0;
		(*cgravity)[1] = 80-8;
		(*cgravity)[2] = 35+100;
	
		(*objectflags)[37] = BallEater;
		(*objectx)[37] = 0;
		(*objecty)[37] = 100-12+100;

		(*objectflags)[38] = BallEater;
		(*objectx)[38] = 160-16;
		(*objecty)[38] = 100-12+100;

		(*objectflags)[65] = LeftFlipper;
		(*objectx)[65] = 0;
		(*objecty)[65] = 70 - 4+100;

		(*objectflags)[66] = RightFlipper;
		(*objectx)[66] = 160-8-24;
		(*objecty)[66] = 70 - 4+100;

		(*objectflags)[67] = LeftFlipper;
		(*objectx)[67] = 100+8;
		(*objecty)[67] = 30-4+100;

		(*objectflags)[68] = RightFlipper;
		(*objectx)[68] = 60-8-8-24;
		(*objecty)[68] = 30-4+100;

		(*launcherxy)[0] = 80-8;
		(*launcherxy)[1] = 70+100;
	}
	//chambered machine
	if(machine == 4)
	{
		use_flags = 1;
		
		(*objectflags)[0] = BigBumper;
		(*objectx)[0] = 2;
		(*objecty)[0] = -16+100;
		
		(*objectflags)[1] = BigBumper;
		(*objectx)[1] = 40;
		(*objecty)[1] = -16+100;
				
		(*objectflags)[2] = BigBumper;
		(*objectx)[2] = 56;
		(*objecty)[2] = -16+100;
				
		(*objectflags)[3] = BigBumper;
		(*objectx)[3] = 88;
		(*objecty)[3] = -16+100;
				
		(*objectflags)[4] = BigBumper;
		(*objectx)[4] = 104;
		(*objecty)[4] = -16+100;
		
		(*objectflags)[5] = BigBumper;
		(*objectx)[5] = 142;
		(*objecty)[5] = -16+100;
						
		(*objectflags)[6] = SmallBumper;
		(*objectx)[6] = 0;
		(*objecty)[6] = -72-8+100;
				
		(*objectflags)[7] = SmallBumper;
		(*objectx)[7] = 8;
		(*objecty)[7] = -64-8+100;
				
		(*objectflags)[8] = SmallBumper;
		(*objectx)[8] = 16;
		(*objecty)[8] = -56-8+100;
				
		(*objectflags)[9] = SmallBumper;
		(*objectx)[9] = 24;
		(*objecty)[9] = -48-8+100;
				
		(*objectflags)[10] = SmallBumper;
		(*objectx)[10] = 160-32;
		(*objecty)[10] = -48-8+100;
				
		(*objectflags)[11] = SmallBumper;
		(*objectx)[11] = 160-24;
		(*objecty)[11] = -56-8+100;
				
		(*objectflags)[12] = SmallBumper;
		(*objectx)[12] = 160-16;
		(*objecty)[12] = -64-8+100;
				
		(*objectflags)[13] = SmallBumper;
		(*objectx)[13] = 160-8;
		(*objecty)[13] = -72-8+100;
				
		(*objectflags)[14] = Accelerator;
		(*objectx)[14] = 8;
		(*objecty)[14] = 100-20+100;
				
		(*objectflags)[15] = Accelerator;
		(*objectx)[15] = 40;
		(*objecty)[15] = 100-20+100;
				
		(*objectflags)[16] = Accelerator;
		(*objectx)[16] = 72;
		(*objecty)[16] = 100-20+100;
				
		(*objectflags)[17] = Accelerator;
		(*objectx)[17] = 104;
		(*objecty)[17] = 100-20+100;
				
		(*objectflags)[18] = Accelerator;
		(*objectx)[18] = 136;
		(*objecty)[18] = 100-20+100;
		
		(*objectflags)[19] = Teleporter;
		(*objectflags)[20] = Teleporter;
		(*objectx)[19] = 80-6;
		(*objecty)[19] = 40+100;
		(*objectx)[20] = 80-6;
		(*objecty)[20] = -12+100;
		
		(*objectflags)[21] = SmallRightDeltoid;
		(*objectx)[21] = 160-16;
		(*objecty)[21] = -100+100;
				
		(*objectflags)[22] = SmallLeftDeltoid;
		(*objectx)[22] = 0;
		(*objecty)[22] = -100+100;
				
		(*objectflags)[23] = SmallRightDeltoid;
		(*objectx)[23] = 80-16-16;
		(*objecty)[23] = -100+100;
				
		(*objectflags)[24] = SmallLeftDeltoid;
		(*objectx)[24] = 80+16;
		(*objecty)[24] = -100+100;
		
		(*objectflags)[25] = BinaryBumper;
		(*objectx)[25] = 80-16;
		(*objecty)[25] = -100+100;
		
		(*objectflags)[26] = Flag;
		(*objectx)[26] = 60-8-8-8-8-8;
		(*objecty)[26] = -99+100;
				
		(*objectflags)[27] = Flag;
		(*objectx)[27] = 60-8-8-8-8;
		(*objecty)[27] = -99+100;
			
		(*objectflags)[28] = Flag;
		(*objectx)[28] = 60-8-8-8;
		(*objecty)[28] = -99+100;
				
		(*objectflags)[29] = Flag;
		(*objectx)[29] = 100+8+8;
		(*objecty)[29] = -99+100;
				
		(*objectflags)[30] = Flag;
		(*objectx)[30] = 100+8+8+8;
		(*objecty)[30] = -99+100;
				
		(*objectflags)[31] = Flag;
		(*objectx)[31] = 100+8+8+8+8;
		(*objecty)[31] = -99+100;
		
		(*cgravity)[0] = 0;
		(*cgravity)[1] = 80-8;
		(*cgravity)[2] = -75+100;
		
		(*objectflags)[32] = BallEater;
		(*objectx)[32] = 0;
		(*objecty)[32] = 0+100;
				
		(*objectflags)[33] = BallEater;
		(*objectx)[33] = 160-16;
		(*objecty)[33] = 0+100;
			
		(*objectflags)[34] = BallEater;
		(*objectx)[34] = 60-8-16;
		(*objecty)[34] = 0+100;
				
		(*objectflags)[35] = BallEater;
		(*objectx)[35] = 100+8;
		(*objecty)[35] = 0+100;
				
		(*objectflags)[36] = BallEater;
		(*objectx)[36] = 0;
		(*objecty)[36] = -30+100;
				
		(*objectflags)[37] = BallEater;
		(*objectx)[37] = 160-16;
		(*objecty)[37] = -30+100;

		(*objectflags)[65] = LeftFlipper;
		(*objectx)[65] = 32;
		(*objecty)[65] = -40-8+100;
				
		(*objectflags)[66] = RightFlipper;
		(*objectx)[66] = 160-40-24;
		(*objecty)[66] = -40-8+100;

		(*launcherxy)[0] = 80-8;
		(*launcherxy)[1] = 60+100;
	}
	//Custom Machine
	if(machine == 10)
	{
		use_flags = 1;
		FILE *name = fopen(cname, "r+t");
		char buf[100];
		int i;
		while(strncmp(buf,"Table=",6) != 0) fgets(buf, 100, name);
		strncpy(tname, buf+6, 20);
		fixstring(&tname);
		while(strncmp(buf,"Password=",9) != 0) fgets(buf, 100, name);
		for(i=0; i<75; i++) {
			fgets(buf, 100, name);
			if(strncmp("objectflag=",buf,11) == 0) (*objectflags)[i] = atoi(&buf[11]);
			fgets(buf, 100, name);
			if(strncmp("objectx=",buf,8) == 0) (*objectx)[i] = atoi(&buf[8]);
			fgets(buf, 100, name);
			if(strncmp("objecty=",buf,8) == 0) (*objecty)[i] = atoi(&buf[8]);
		}
		fgets(buf, 100, name);
		if(strncmp("launcherx=",buf,10) == 0) (*launcherxy)[0] = atoi(&buf[10]);
		fgets(buf, 100, name);
		if(strncmp("launchery=",buf,10) == 0) (*launcherxy)[1] = atoi(&buf[10]);
		fgets(buf, 100, name);
		if(strncmp("cgravityx=",buf,10) == 0) (*cgravity)[1] = atoi(&buf[10]);
		fgets(buf, 100, name);
		if(strncmp("cgravityy=",buf,10) == 0) (*cgravity)[2] = atoi(&buf[10]);
		for(i=0; i<75; i++) {
			if((*objectflags)[i] == 100 || (*objectflags)[i] == -100) (*objectflags)[i] /= 100;
		}
		fclose(name);
	}
	//machine starting constants
	(*moveshiftx)[0] = 0; //horizontal gravity increment, modifies (*ballmovex)[0]
	(*moveshifty)[0] = 0.25; //vertical gravity increment, modifies (*ballmovey)[0]
	(*plungerxy)[0] = (*launcherxy)[0] + 4;
	(*plungerxy)[1] = (*launcherxy)[1] + 8;
	(*ballflags)[0] = 1;
	(*ballx)[0] = (*launcherxy)[0] + 4; //ball's initial horizontal position (dependent on location of the launcher)
	(*bally)[0] = (*launcherxy)[1] - 4; //ball's initial vertical position (dependent on location of the launcher)
	(*ballmovex)[0] = 0; //horizontal ball vector, modifies (*ballx)[0]
	(*ballmovey)[0] = 0; //vertical ball vector, modifies (*bally)[0]
}

//originally from cblaster
void inputStr(char *str,short x,short y,short maxlen) {
	short i=0,key=0,len=maxlen;
	while(key != KEY_ENTER) {
		str[i]='_'; str[i+1]=' '; str[i+2]=0;
		DrawStr(x,y,str,A_NORMAL);
		key=ngetchx();
		if(key == KEY_ESC) break;
		if(key>=' '&&key<='~'&&i<len) {str[i] = ' ';DrawStr(x,y,str,A_REPLACE);str[i++]=key;};
		if(key==257&&i) {str[i] = ' ';DrawStr(x,y,str,A_REPLACE);i--;str[i] = ' ';DrawStr(x,y,str,A_REPLACE);};
		if(key=='\\'&&i) len=strlen(str)+7;
	}
	str[i]=0;
}

//main function, creates game loop
void _main(void)
{
	clearfullscreen();
	//OSSetSR(0x0400); //disable some (not all) interrupts
	logo();
//	startup();
play_again:
	game();
	//OSSetSR(0x0000); //enable interrupts
	goto play_again;
}

void startup(void)
{
	if(initilized == 0) {
		save_vec_auto_int_1 = GetIntVec(AUTO_INT_1);
		SetIntVec(AUTO_INT_1, DUMMY_HANDLER);
		if(grayflag == 1)
		{
			GrayOn();
			if(!GrayCheckRunning()) {
				FontSetSys(F_8x10);
				printf_xy(0,0,"Not Enough Free Memory");
				while(!_keytest (RR_ENTER)) {}
				SetIntVec(AUTO_INT_1, save_vec_auto_int_1);
				exit(0);
			}
		}
		doublescreen = malloc(2*3840);
		if(!doublescreen)
		{
			clrscr();
			GrayOff();
			FontSetSys(F_8x10);
			printf_xy(0,0,"Not Enough Free Memory");
			while(!_keytest (RR_ENTER)) {}
			SetIntVec(AUTO_INT_1, save_vec_auto_int_1);
			exit(0);
		}
		lightdoublescreen = malloc(2*3840);
		if(!lightdoublescreen)
		{
			clrscr();
			GrayOff();
			FontSetSys(F_8x10);
			printf_xy(0,0,"Not Enough Free Memory");
			while(!_keytest (RR_ENTER)) {}
			SetIntVec(AUTO_INT_1, save_vec_auto_int_1);
			free(doublescreen);
			exit(0);
		}
		darkdoublescreen = malloc(2*3840);
		if(!darkdoublescreen)
		{
			clrscr();
			GrayOff();
			FontSetSys(F_8x10);
			printf_xy(0,0,"Not Enough Free Memory");
			while(!_keytest (RR_ENTER)) {}
			SetIntVec(AUTO_INT_1, save_vec_auto_int_1);
			free(doublescreen);
			free(lightdoublescreen);
			exit(0);
		}
		memset(doublescreen,0,2*3840);
		memset(lightdoublescreen,0,2*3840);
		memset(darkdoublescreen,0,2*3840);
		initilized = 1;
	}
}

void cleanup(void)
{
	if(initilized == 1) {
		GrayOff();
		SetIntVec(AUTO_INT_1, save_vec_auto_int_1);
		free(doublescreen);
		free(lightdoublescreen);
		free(darkdoublescreen);
		initilized = 0;
	}
}

//this is the primary driver function, as it controls all of the other functions
//in the game.  All of the primary game variables are defined and declared here,
//and are passed to the other functions.
void game(void)
{
	//game sprites
	//the ball sprite
	unsigned char ball[] = {
		0b00111100,
		0b01000010,
		0b10110001,
		0b10100001,
		0b10000001,
		0b10000001,
		0b01000010,
		0b00111100,
		0b00000000,
		0b00111100,
		0b01001110,
		0b01011110,
		0b01111110,
		0b01111110,
		0b00111100,
		0b00000000
	};
	//the pivot point for the flippers
	unsigned char pivot[] = {
		0b00000000,
		0b00000000
	};
	unsigned long lflipperdown[] = {
		0b00111111100000000000000000000000,
		0b01011010011000000000000000000000,
		0b10111101000110000000000000000000,
		0b11100111000001000000000000000000,
		0b11100111000000110000000000000000,
		0b10111101000000001100000000000000,
		0b01011010000000000010000000000000,
		0b00111100000000000001100000000000,
		0b00000011110000000000011000000000,
		0b00000000001111110000000100000000,
		0b00000000000000001111000011000000,
		0b00000000000000000000111111000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00100101100000000000000000000000,
		0b01000000111000000000000000000000,
		0b00011000111110000000000000000000,
		0b00011000111111000000000000000000,
		0b01000010111111110000000000000000,
		0b00100101111111111100000000000000,
		0b00000011111111111110000000000000,
		0b00000000001111111111100000000000,
		0b00000000000000001111111000000000,
		0b00000000000000000000111100000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000
	};
	unsigned long lflipperup[] = {
		0b00111111111111111111111111000000,
		0b01011010000000000000000111000000,
		0b10111101000000000000111000000000,
		0b11100111000000000111000000000000,
		0b11100111000000111000000000000000,
		0b10111101000111000000000000000000,
		0b01011010111000000000000000000000,
		0b00111111000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00100101111111111111111100000000,
		0b01000010111111111111100000000000,
		0b00011000111111111100000000000000,
		0b00011000111111100000000000000000,
		0b01000010111100000000000000000000,
		0b00100101000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000
	};
	unsigned long rflipperdown[] = {
		0b00000000000000000000000111111100,
		0b00000000000000000000011001011010,
		0b00000000000000000001100010111101,
		0b00000000000000000010000011100111,
		0b00000000000000001100000011100111,
		0b00000000000000110000000010111101,
		0b00000000000001000000000001011010,
		0b00000000000110000000000000111100,
		0b00000000011000000000001111000000,
		0b00000000100000001111110000000000,
		0b00000011000011110000000000000000,
		0b00000011111100000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000110100100,
		0b00000000000000000000011101000010,
		0b00000000000000000001111100011000,
		0b00000000000000000011111100011000,
		0b00000000000000001111111101000010,
		0b00000000000000111111111110100100,
		0b00000000000001111111111111000000,
		0b00000000000111111111110000000000,
		0b00000000011111110000000000000000,
		0b00000000111100000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
	};
	unsigned long rflipperup[] = {
		0b00000011111111111111111111111100,
		0b00000011100000000000000001011010,
		0b00000000011100000000000010111101,
		0b00000000000011100000000011100111,
		0b00000000000000011100000011100111,
		0b00000000000000000011100010111101,
		0b00000000000000000000011101011010,
		0b00000000000000000000000011111100,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000011111111111111110100100,
		0b00000000000011111111111101000010,
		0b00000000000000011111111100011000,
		0b00000000000000000011111100011000,
		0b00000000000000000000011101000010,
		0b00000000000000000000000010100100,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000
	};
	//the "cup" in which the ball is launched from
	unsigned short launcher[] = {
		0b0000000000000000,
		0b0000000000000000,
		0b1000000000000001,
		0b1000000000000001,
		0b1000000000000001,
		0b1000000000000001,
		0b1000000000000001,
		0b1111111111111111,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0100000000000010,
		0b0100000000000010,
		0b0111111111111110,
		0b0000000000000000
	};
	//the part of the launcher that moves back to kick the ball into the table
	unsigned char plunger[] = {
		0b01111110,
		0b00111100,
		0b00100100,
		0b00100100,
		0b00100100,
		0b00100100,
		0b00100100,
		0b00100100,
		0b00100100,
		0b00100100,
		0b00100100,
		0b00100100,
		0b00100100,
		0b00100100,
		0b00100100,
		0b00011000,
		0b00000000,
		0b00000000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00011000,
		0b00000000
	};
	//big bumpers (kicks ball in direction dependent on ball vectors, increases ball speed by 1.5)
	unsigned short largebumper[] = {
		0b0000111111110000,
		0b0001111111111000,
		0b0011000000001100,
		0b0110000000000110,
		0b1100000000000011,
		0b1100000000000011,
		0b1100000110000011,
		0b1100001111000011,
		0b1100001111000011,
		0b1100000110000011,
		0b1100000000000011,
		0b1100000000000011,
		0b0110000000000110,
		0b0011000000001100,
		0b0001111111111000,
		0b0000111111110000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000111111110000,
		0b0001111111111000,
		0b0011100000011100,
		0b0011001111001100,
		0b0011011001101100,
		0b0011010000101100,
		0b0011010000101100,
		0b0011011001101100,
		0b0011001111001100,
		0b0011100000011100,
		0b0001111111111000,
		0b0000111111110000,
		0b0000000000000000,
		0b0000000000000000
	}; 
	//small bumpers (kicks ball in direction dependent on ball vectors, no speed change)
	unsigned char smallbumper[] = {
		0b01111110,
		0b11111111,
		0b11000011,
		0b11011011,
		0b11011011,
		0b11000011,
		0b11111111,
		0b01111110,
		0b00000000,
		0b00000000,
		0b00111100,
		0b00100100,
		0b00100100,
		0b00111100,
		0b00000000,
		0b00000000
	};
	//elongated "rectangular" bumpers (kicks ball in direction dependent on ball vectors, increases ball speed by 1.1)
	unsigned char rectbumper[] = {
		0b01111110,
		0b11111111,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11011011,
		0b11011011,
		0b11011011,
		0b11011011,
		0b11011011,
		0b11011011,
		0b11000011,
		0b11000011,
		0b11000011,
		0b11111111,
		0b01111110,
		0b00000000,
		0b00000000,
		0b00111100,
		0b00111100,
		0b00111100,
		0b00100100,
		0b00100100,
		0b00100100,
		0b00100100,
		0b00100100,
		0b00100100,
		0b00111100,
		0b00111100,
		0b00111100,
		0b00000000,
		0b00000000
	};
	//binary bumper (kicks ball in random direction)
	unsigned long binary[] = {
		0b01111111111111111111111111111110,
		0b10000000000000000000000000000001,
		0b10111000010001000000011100000001,
		0b10100101011001001110010010100101,
		0b10111001010101010001011100100101,
		0b10100101010011011111010100011001,
		0b10111001010001010001010010010001,
		0b10000001000000010001000000100001,
		0b10000000000000000000000000000001,
		0b01111111111111111111111111111110,
		0b00000000000000000000000000000000,
		0b01111000110011100000011100000000,
		0b01000111101010101110100011111110,
		0b01011010100110110001101101011010,
		0b01000110101010101110100011011010,
		0b01011010101100100000101010100100,
		0b01000110101010101110101101101000,
		0b01111010111001101010111011010000,
		0b00000011100000111011100001110000,
		0b00000000000000000000000000000000
	};
	//this is never used
	unsigned char zero[] = {
		0b01111110,
		0b10000001,
		0b10011001,
		0b10100101,
		0b10100101,
		0b10011001,
		0b10000001,
		0b01111110
	};
	//this is never used
	unsigned char one[] = {
		0b01111110,
		0b10000001,
		0b10111001,
		0b10011001,
		0b10011001,
		0b10111101,
		0b10000001,
		0b01111110
	};
	//the "teeth" that eat your ball on the table
	unsigned short balleater[] = {
		0b0001111111111000,
		0b0111111111111110,
		0b1100100110010011,
		0b1100110110110011,
		0b1100111111110011,
		0b1110111111110111,
		0b1111111111111111,
		0b1110111111110111,
		0b1100110110110011,
		0b1100100110010011,
		0b0111111111111110,
		0b0001111111111000,
		0b0000000000000000,
		0b0000000000000000,
		0b0011011001101100,
		0b0011001001001100,
		0b0011000000001100,
		0b0001000000001000,
		0b0000000000000000,
		0b0001000000001000,
		0b0011001001001100,
		0b0011011001101100,
		0b0000000000000000,
		0b0000000000000000
	};
	//kicks ball in perpendicular direction when it hits the hypotenuse
	unsigned long leftdeltabumper[] = {
		0b00000000000000000000000000000000,
		0b01111100000000000000000000000000,
		0b11111110000000000000000000000000,
		0b11110011000000000000000000000000,
		0b11111001100000000000000000000000,
		0b11111100110000000000000000000000,
		0b11111110011000000000000000000000,
		0b11111111001100000000000000000000,
		0b11111111100110000000000000000000,
		0b11100011110011000000000000000000,
		0b11100001111001100000000000000000,
		0b11100000111100110000000000000000,
		0b11100000011110011000000000000000,
		0b11100000001111001100000000000000,
		0b11100000000111100110000000000000,
		0b11100000000011110011000000000000,
		0b11100000000001111001100000000000,
		0b11100000000000111100110000000000,
		0b11100000000000011110011000000000,
		0b11100000000000001111001100000000,
		0b11100000000000000111100110000000,
		0b11100000000000000011110011000000,
		0b11100000000000000001111001100000,
		0b11100000000000000000111100110000,
		0b11100000000000000000011110011000,
		0b11100000000000000000001111001100,
		0b11100000000000000000000111100110,
		0b11100000000000000000000111111111,
		0b01110000000000000000000111111110,
		0b01111111111111111111111111111110,
		0b00111111111111111111111111111100,
		0b00001111111111111111111111110000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00011100000000000000000000000000,
		0b00011110000000000000000000000000,
		0b00011111000000000000000000000000,
		0b00011111100000000000000000000000,
		0b00011101110000000000000000000000,
		0b00011100111000000000000000000000,
		0b00011100011100000000000000000000,
		0b00011100001110000000000000000000,
		0b00011100000111000000000000000000,
		0b00011100000011100000000000000000,
		0b00011100000001110000000000000000,
		0b00011100000000111000000000000000,
		0b00011100000000011100000000000000,
		0b00011100000000001110000000000000,
		0b00011100000000000111000000000000,
		0b00011100000000000011100000000000,
		0b00011100000000000001110000000000,
		0b00011111111111111111111000000000,
		0b00011111111111111111111000000000,
		0b00001111111111111111111000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000
	};
	//same as above
	unsigned long rightdeltabumper[] = {
		0b00000000000000000000000000000000,
		0b00000000000000000000000000111110,
		0b00000000000000000000000001111111,
		0b00000000000000000000000011001111,
		0b00000000000000000000000110011111,
		0b00000000000000000000001100111111,
		0b00000000000000000000011001111111,
		0b00000000000000000000110011111111,
		0b00000000000000000001100111111111,
		0b00000000000000000011001111000111,
		0b00000000000000000110011110000111,
		0b00000000000000001100111100000111,
		0b00000000000000011001111000000111,
		0b00000000000000110011110000000111,
		0b00000000000001100111100000000111,
		0b00000000000011001111000000000111,
		0b00000000000110011110000000000111,
		0b00000000001100111100000000000111,
		0b00000000011001111000000000000111,
		0b00000000110011110000000000000111,
		0b00000001100111100000000000000111,
		0b00000011001111000000000000000111,
		0b00000110011110000000000000000111,
		0b00001100111100000000000000000111,
		0b00011001111000000000000000000111,
		0b00110011110000000000000000000111,
		0b01100111100000000000000000000111,
		0b11111111100000000000000000000111,
		0b01111111100000000000000000001110,
		0b01111111111111111111111111111110,
		0b00111111111111111111111111111100,
		0b00001111111111111111111111110000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000111000,
		0b00000000000000000000000001111000,
		0b00000000000000000000000011111000,
		0b00000000000000000000000111111000,
		0b00000000000000000000001110111000,
		0b00000000000000000000011100111000,
		0b00000000000000000000111000111000,
		0b00000000000000000001110000111000,
		0b00000000000000000011100000111000,
		0b00000000000000000111000000111000,
		0b00000000000000001110000000111000,
		0b00000000000000011100000000111000,
		0b00000000000000111000000000111000,
		0b00000000000001110000000000111000,
		0b00000000000011100000000000111000,
		0b00000000000111000000000000111000,
		0b00000000001110000000000000111000,
		0b00000000011111111111111111111000,
		0b00000000011111111111111111111000,
		0b00000000011111111111111111110000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000
	};
	//there are only two of these on a table, if at all.  When a ball
	//lands in one, it comes out the other w/o altering the vectors
	unsigned short teleporter[] = {
		0b0011111111000000,
		0b0111000011100000,
		0b1111111111110000,
		0b1110000001110000,
		0b1010000001010000,
		0b1010000001010000,
		0b1010000001010000,
		0b1010000001010000,
		0b1110000001110000,
		0b1111111111110000,
		0b0111000011100000,
		0b0011111111000000,
		0b0000000000000000,
		0b0000111100000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0100000000100000,
		0b0100011000100000,
		0b0100011000100000,
		0b0100000000100000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000111100000000,
		0b0000000000000000
	};
	//kicks ball in opposite vertical vector upon impact, disappears when hit,
	//and reappears once ball lands in black hole.  Once all of these (if any)
	//are knocked down on a table, the black hole appears.
	unsigned char flag[] = {
		0b01111110,
		0b01000010,
		0b01000010,
		0b01000010,
		0b01000010,
		0b01000010,
		0b11111111,
		0b11111111,
		0b00000000,
		0b00111100,
		0b00100100,
		0b00100100,
		0b00100100,
		0b00100100,
		0b00000000,
		0b00000000
	};
	//the black hole.  This is the only object on a table that has gravity of its own.
	//when the ball strays near the hole, it is sucked toward it by slightly altering
	//the ball's vectors.  This only appears when all of the flags, if any, are knocked down.
	//if the ball lands inside the hole, then the user gains an extra life, all of the flags
	//are reactivated, and the hole disappears until the flags are knocked down again.
	unsigned short centralgravity[] = {
		0b0001111111111000,
		0b0011111111111100,
		0b0111100011111110,
		0b1111011100001111,
		0b1110110001110111,
		0b1110100011011011,
		0b1110110110001011,
		0b1110011111001011,
		0b1101001111100111,
		0b1101000110110111,
		0b1101101100010111,
		0b1110111000110111,
		0b1111000011101111,
		0b0111111100011110,
		0b0011111111111100,
		0b0001111111111000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000011100000000,
		0b0000100011110000,
		0b0001001110001000,
		0b0001011100100100,
		0b0001001001110100,
		0b0001100000110100,
		0b0010110000011000,
		0b0010111001001000,
		0b0010010011101000,
		0b0001000111001000,
		0b0000111100010000,
		0b0000000011100000,
		0b0000000000000000,
		0b0000000000000000
	};
	//when the ball makes contact with this object, it is shot upwards at a speed
	//dependent on the strength of the table's gravity
	unsigned short accelerator[] = {
		0b0110000000000110,
		0b1101000000001011,
		0b1011000000001101,
		0b0110000000000110,
		0b0000000000000000,
		0b0110000000000110,
		0b1011000110001101,
		0b1101001001001011,
		0b0110010000100110,
		0b0000000110000000,
		0b0110000110000110,
		0b1101000110001011,
		0b1011000110001101,
		0b0110000000000110,
		0b0000000000000000,
		0b0110000000000110,
		0b1011000000001101,
		0b1101000000001011,
		0b0110000000000110,
		0b0000000000000000,
		0b0000000000000000,
		0b0010000000000100,
		0b0100000000000010,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0100000000000010,
		0b0010000110000100,
		0b0000001111000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0010000000000100,
		0b0100000000000010,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0100000000000010,
		0b0010000000000100,
		0b0000000000000000,
		0b0000000000000000
	};
	//does the same as triangular bumpers, but just smaller
	unsigned short litedeltabumperleft[] = {
		0b0111111111111110,
		0b1111111111111111,
		0b1111111111111111,
		0b0100111100000111,
		0b0010011100000111,
		0b0001001110000111,
		0b0000100111000111,
		0b0000010011100111,
		0b0000001001111111,
		0b0000000100111111,
		0b0000000010011111,
		0b0000000001001111,
		0b0000000000100111,
		0b0000000000010111,
		0b0000000000001111,
		0b0000000000000110,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000011111000,
		0b0000000011111000,
		0b0000000001111000,
		0b0000000000111000,
		0b0000000000011000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000
	};
	//this is the mirror image of the one above, but this particular
	//object is not yet implemented
	unsigned short litedeltabumperright[] = {
		0b0111111111111110,
		0b1111111111111111,
		0b1111111111111111,
		0b1110000011110010,
		0b1110000011100100,
		0b1110000111001000,
		0b1110001110010000,
		0b1110011100100000,
		0b1111111001000000,
		0b1111110010000000,
		0b1111100100000000,
		0b1111001000000000,
		0b1110010000000000,
		0b1110100000000000,
		0b1111000000000000,
		0b0110000000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0001111100000000,
		0b0001111100000000,
		0b0001111000000000,
		0b0001110000000000,
		0b0001100000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000,
		0b0000000000000000
	};
	//this appears in the lower corner of the table when horizontal gravity is activated
	unsigned char gravityon[] = {
		0b01111110,
		0b10000000,
		0b10011110,
		0b10000001,
		0b01111110,
		0b00000000,
		0b01111110,
		0b01100000,
		0b01111110,
		0b00000000
	};
	//sprite test
	//GraySprite8 (0,0,8,ball,LCD_MEM,SPRT_XOR);
	//GraySprite8 (8,8,8,pivot,LCD_MEM,SPRT_XOR);
	//GraySprite16 (8, 20, 8, launcher, LCD_MEM, SPRT_XOR);
	//GraySprite8 (12, 28, 16, plunger, LCD_MEM, SPRT_XOR);
	//GraySprite16 (30,20,16,largebumper,LCD_MEM,SPRT_XOR);
	//GraySprite8 (30,36,8,smallbumper,LCD_MEM,SPRT_XOR);
	//GraySprite8 (40,40,16,rectbumper,LCD_MEM,SPRT_XOR);
	//GraySprite32 (50,50,10,binary,LCD_MEM,SPRT_XOR);
	//GraySprite8 (50,60,8,zero,LCD_MEM,SPRT_XOR);
	//GraySprite8 (50,70,8,one,LCD_MEM,SPRT_XOR);
	//GraySprite16(70,70,12,balleater,LCD_MEM,SPRT_XOR);
	//GraySprite32(100,0,32,leftdeltabumper,LCD_MEM,SPRT_XOR);
	//GraySprite32(100,50,32,rightdeltabumper,LCD_MEM,SPRT_XOR);
	//GraySprite16(144,25,12,ballcatch,LCD_MEM,SPRT_XOR);
	//ngetchx();
	//all large objects are encapsulated into a long array for easier parameter passing
	unsigned long *bigobjects[] = {
		binary,
		leftdeltabumper,
		rightdeltabumper,
		lflipperup,
		lflipperdown,
		rflipperup,
		rflipperdown
	};
	//all medium objects are encapsulated into a short array for easier parameter passing
	unsigned short *medobjects[] = {
		teleporter,
		balleater,
		launcher,
		largebumper,
		accelerator,
		litedeltabumperleft,
		centralgravity,
		litedeltabumperright
	};
	//all small objects are encapsulated into a char array for easier parameter passing
	unsigned char *smallobjects[] = {
		ball,
		zero,
		one,
		pivot,
		smallbumper,
		rectbumper,
		plunger,
		gravityon,
		flag
	};
	//primary game variables
	//these arrays control the ball, even though only the first term is used
	short ballflags[] = {0};
	float ballx[] = {0};
	float bally[] = {0};
	float ballmovex[] = {0};
	float ballmovey[] = {0};
	float moveshiftx[] = {0};
	float moveshifty[] = {0};
	short objectflags[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	short objectx[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	short objecty[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	short cgravity[] = {0,0,0};
	short launcherxy[] = {0,0};
	short plungerxy[] = {0,0,0};
	short score = 0;
	int trueflag = 1;
	int numballs = 2;
	gamemenu(); //start the menu
	getmachine(&objectflags, &objectx, &objecty, &ballflags, &ballx, &bally, &ballmovex, &ballmovey, &moveshiftx, &moveshifty, &launcherxy, &plungerxy, &cgravity, machine); //create a machine based on the value of the machine variable
	draw(&objectflags, &objectx, &objecty, &ballflags, &ballx, &bally, &ballmovex, &ballmovey, &moveshiftx, &moveshifty, &launcherxy, &plungerxy, &cgravity, bigobjects, medobjects, smallobjects); //draw the sprites
	float plungerpower = 0; //controls the power stored in the retractable part of the launcher
	short counter = 0; //delay before ball explodes if the ball stops moving
	int i;
	short updateflag;
	while(trueflag == 1)
	{
		getkey(&objectflags); //initilize the upward force of the flippers if keys F1 or F5 (F8 for 92+ and V200) are being pressed
		if(_keytest (RR_F1) && leftflipper == 0) {leftflipperup(&objectflags, &objectx, &objecty, bigobjects); leftflipper = 1;} //update left flipper image so it is up
		if(!_keytest (RR_F1) && leftflipper == 1) {leftflipperdown(&objectflags, &objectx, &objecty, bigobjects); leftflipper = 0;} //update left flipper image so it is down
		if((_keytest (RR_F5) || _keytest (RR_F8)) && rightflipper == 0) {rightflipperup(&objectflags, &objectx, &objecty, bigobjects); rightflipper = 1;} //update right flipper image so it is up
		if((!_keytest (RR_F5) && !_keytest (RR_F8)) && rightflipper == 1) {rightflipperdown(&objectflags, &objectx, &objecty, bigobjects); rightflipper = 0;} //update right flipper image so it is down
		if(_keytest (RR_PLUS)) OSContrastUp();
		if(_keytest (RR_MINUS)) OSContrastDn();
		if(!_keytest (RR_F1)) {
			lkeypress = 0;
			for(i=65; i<75; i++)
			{
				if(objectflags[i] > 0) objectflags[i] = 1;
			}
		}
		else {
			lkeypress = 1;
		}
		if(!_keytest (RR_F5) && !_keytest (RR_F8)) {
			rkeypress = 0;
			for(i=65; i<75; i++)
			{
				if(objectflags[i] < 0) objectflags[i] = -1;
			}
		}
		else {
			rkeypress = 1;
		}
		if(_keytest (RR_CLEAR)) trueflag = 0; //if you pressed clear, the game stops and calls up the menu
		if(_keytest (RR_DOWN)) moveplunger(&plungerxy, &plungerpower, smallobjects); //retract plunger if you're holding down the down arrow
		if(_keytest (RR_F3) && horizontalgravity == 0) {horizontalgravity = 1; GraySprite8 (160-8, 200 - 5, 5, gravityon, doublescreen, SPRT_XOR);}
		if(_keytest (RR_MODE))
		{
			PortSet(LCD_MEM, 239, 127);
			FontSetSys(F_8x10);
			clrscr();
			GrayOff();
			printf_xy(60,40,"PAUSE");
			FontSetSys(F_4x6);
			printf_xy(40,55,"Balls: %d", numballs);
			printf_xy(90,55,"Score: %d", score);
			while(!_keytest (RR_ENTER)) {}
			clrscr();
			if(grayflag == 1) GrayOn();
			PortSet(doublescreen,239,127+128);
		}
		if(!_keytest (RR_DOWN) && plungerpower > 0 && (ballx[0] + 8 >= launcherxy[0] && ballx[0] <= launcherxy[0] + 16) && (bally[0] - 8 >= launcherxy[1] - 8 && bally[0] <= launcherxy[1] + 16))
		{
			ballmovey[0] = -ceil(plungerpower) - gravity;
			GraySprite8 (ballx[0], bally[0], 8, smallobjects[0], doublescreen, SPRT_XOR);
			bally[0] = ballmovey[0] + bally[0];
			GraySprite8 (ballx[0], bally[0], 8, smallobjects[0], doublescreen, SPRT_XOR);
			GraySprite8 (plungerxy[0], plungerxy[1], 16, smallobjects[6], doublescreen, SPRT_XOR);
			plungerxy[1] = plungerxy[1] - floor(plungerpower);
			GraySprite8 (plungerxy[0], plungerxy[1], 16, smallobjects[6], doublescreen, SPRT_XOR);
			plungerpower = 0;
			ballmovex[0] = 0;
			counter = 1;
		}
		int i;
		for(i=65; i<75; i++)
		{
			if(objectflags[i] > 1)
			{
				objectflags[i] = objectflags[i] - 1;
				if(objectflags[i] < 2) objectflags[i] = 2;
			}
			if(objectflags[i] < -1)
			{
				objectflags[i] = objectflags[i] + 1;
				if(objectflags[i] > -2) objectflags[i] = -2;
			} 
		}
		if((ballx[0] + 8 <= launcherxy[0] || ballx[0] >= launcherxy[0] + 16) || (bally[0] - 8 <= launcherxy[1] - 8 || bally[0] >= launcherxy[1] + 16)) moveball(&ballflags, &ballx, &bally, &ballmovex, &ballmovey, &moveshiftx, &moveshifty, smallobjects, &updateflag);
		else
		{
			GraySprite8 (ballx[0], bally[0], 8, ball, doublescreen, SPRT_XOR);
			ballx[0] = launcherxy[0] + 4;
			bally[0] = launcherxy[1] - 1;
			GraySprite8 (ballx[0], bally[0], 8, ball, doublescreen, SPRT_XOR);
			ballmovex[0] = 0;
			ballmovey[0] = 0;
			counter = 0;
		}
		if(_keytest (RR_F3)) {if(horizontalgravity==1) horizontalgravity = 0; else horizontalgravity = 1; if(grayflag == 1) {Sprite8 (160-8, LCD_HEIGHT - 5, 5, gravityon, GetPlane(LIGHT_PLANE), SPRT_XOR); Sprite8(160-8, LCD_HEIGHT-5, 5, gravityon, GetPlane(DARK_PLANE), SPRT_XOR); Sprite8(160-8, LCD_HEIGHT-5, 5, gravityon+5, GetPlane(LIGHT_PLANE), SPRT_XOR);} else Sprite8(160-8, LCD_HEIGHT-5, 5, gravityon, LCD_MEM, SPRT_XOR);}
		detectcollision(&objectflags, &objectx, &objecty, &ballflags, &ballx, &bally, &ballmovex, &ballmovey, &moveshiftx, &moveshifty, &launcherxy, &plungerxy, &cgravity, bigobjects, medobjects, smallobjects, &score, &numballs, &trueflag);
		if((ballmovex[0] == 0 && ballmovey[0] == 0 && counter == 1) || _keytest (RR_2ND))
		{
			counter = 0;
			int radius = 1;
			GraySprite8(ballx[0], bally[0], 8, smallobjects[0], doublescreen, SPRT_XOR);
			PortSet(doublescreen,239,127+128);
			DrawClipEllipse(ballx[0] + 4, bally[0] + 4, radius, radius, &(SCR_RECT){{0,0,159,199}}, A_XOR);
			while (radius < 15)
			{
				DrawClipEllipse(ballx[0] + 4, bally[0] + 4, radius, radius, &(SCR_RECT){{0,0,159,199}}, A_XOR);
				radius++;
				if(radius >= 15) break;
				DrawClipEllipse(ballx[0] + 4, bally[0] + 4, radius, radius, &(SCR_RECT){{0,0,159,199}}, A_XOR);
			}
			PortSet(LCD_MEM,239,127);
			numballs--;
			if(numballs == -1) trueflag = 0;
			ballx[0] = launcherxy[0] + 4;
			bally[0] = launcherxy[1] - 4;
			ballmovex[0] = 0;
			ballmovey[0] = 0;
			GraySprite8(ballx[0], bally[0], 8, smallobjects[0], doublescreen, SPRT_XOR);
		}
		if(ballx[0] != launcherxy[0] + 4 && bally[0] != launcherxy[1] - 4 && ballmovex[0] == 0 && ballmovey[0] != 0)
		{
			if(ballx[0] < 80) ballmovex[0] = 0.25;
			if(ballx[0] >= 80) ballmovex[0] = -0.25;
		}
		int k = 0;
		for(i=0; i<65; i++)
		{
			 if(objectflags[i] == Flag) k = 1;
		}
		if(k == 0 && use_flags == 1 && cgravity[0] == 0)
		{
			GraySprite16 (cgravity[1], cgravity[2], 16, centralgravity, doublescreen, SPRT_XOR);
			cgravity[0] = 1;
		}
		k = 1;
		if(cgravity[0] == 1 && ballx[0] + 4 >= cgravity[1] - 32 && ballx[0] <= cgravity[1] + 16 + 32 && bally[0] + 8 >= cgravity[2] - 32 && bally[0] <= cgravity[2] + 16 + 32)
		{
			if(ballx[0] + 4 <= cgravity[1] + 8) ballmovex[0] += 0.25;
			else ballmovex[0] -= 0.25;
			if(bally[0] + 4 <= cgravity[2] + 8) ballmovey[0] += 0.25;
			else ballmovey[0] -= 0.25;
		}
		if(cgravity[0] == 1 && ballx[0] + 8 >= cgravity[1] && ballx[0] <= cgravity[1] + 16 && bally[0] + 8 >= cgravity[2] && bally[0] <= cgravity[2] + 16)
		{
			numballs++;
			GraySprite8(ballx[0], bally[0], 8, ball, doublescreen, SPRT_XOR);
			ballx[0] = launcherxy[0] + 4;
			bally[0] = launcherxy[1] - 4;
			GraySprite8(ballx[0], bally[0], 8, ball, doublescreen, SPRT_XOR);
			int j;
			for(j=0; j<65; j++)
			{
				if(objectflags[j] == -Flag) objectflags[j] = Flag;
			}
			cgravity[0] = 0;
			score += 20;
			PortSet(doublescreen,239,127+128);
			clearfullscreen();
			draw(&objectflags, &objectx, &objecty, &ballflags, &ballx, &bally, &ballmovex, &ballmovey, &moveshiftx, &moveshifty, &launcherxy, &plungerxy, &cgravity, bigobjects, medobjects, smallobjects); //draw the sprites
			PortSet(LCD_MEM,239,127);
		}
		short y = (short)bally[0];
		short spriteson = 0;
		char *address = doublescreen + 30*(y-(LCD_HEIGHT/2));
		char *lightaddress = lightdoublescreen + 30*(y-(LCD_HEIGHT)/2);
		char *darkaddress = darkdoublescreen + 30*(y-(LCD_HEIGHT)/2);
		if(bally[0]+(LCD_HEIGHT/2) < 200 && bally[0]-(LCD_HEIGHT/2) > 0)
		{
			if(grayflag == 0) FastCopyScreen(address,LCD_MEM);
			else if(grayflag == 1) {
				FastCopyScreen(lightaddress, GetPlane(LIGHT_PLANE));
				FastCopyScreen(darkaddress, GetPlane(DARK_PLANE));
			}
			spriteson = 1;
		}
		else if(bally[0]<100 && spriteson == 0)
		{
			if(grayflag == 0) FastCopyScreen(doublescreen,LCD_MEM);
			else if(grayflag == 1) {
				FastCopyScreen(lightdoublescreen, GetPlane(LIGHT_PLANE));
				FastCopyScreen(darkdoublescreen, GetPlane(DARK_PLANE));
			}
			spriteson = 1;
		}
		else if(bally[0]>100 && spriteson == 0)
		{
			if(grayflag == 0) FastCopyScreen(doublescreen+200*30-(30*LCD_HEIGHT),LCD_MEM);
			else if(grayflag == 1) {
				FastCopyScreen(lightdoublescreen+200*30-(30*LCD_HEIGHT),GetPlane(LIGHT_PLANE));
				FastCopyScreen(darkdoublescreen+200*30-(30*LCD_HEIGHT),GetPlane(DARK_PLANE));
			}
			spriteson = 1;
		}	
		FontSetSys(F_4x6);
	}
	PortSet(LCD_MEM, 239, 127);
	FontSetSys(F_8x10);
	GrayOff();
	printf_xy(40,40,"Game Over");
	FontSetSys(F_4x6);
	printf_xy(60,60,"Score: %d", score);
	while (!_keytest (RR_ENTER)) {}
	horizontalgravity = 0;
	leftflipper = 0;
	rightflipper = 0;
	SetIntVec(AUTO_INT_1,save_vec_auto_int_1);
	SYM_ENTRY *sym = SymFindPtr (SYMSTR ("pinhsc"),0);
	if(!sym)
	{
		FILE *file = fopen("pinhsc","wb");
		fprintf(file, "blank\n");
		fprintf(file, "Rank=1\n");
		fprintf(file, "Person=Mr. Nobody\n");
		fprintf(file, "Machine=Traditional         \n");
		fprintf(file, "Score=0\n");
		fprintf(file, "Rank=2\n");
		fprintf(file, "Person=Ms. Nobody\n");
		fprintf(file, "Machine=Detached            \n");
		fprintf(file, "Score=0\n");
		fprintf(file, "Rank=3\n");
		fprintf(file, "Person=Mrs. Nobody\n");
		fprintf(file, "Machine=Symmetrical         \n");
		fprintf(file, "Score=0\n");
		fprintf(file, "Rank=4\n");
		fprintf(file, "Person=Dr. Nobody\n");
		fprintf(file, "Machine=Chambered           \n");
		fprintf(file, "Score=0\n");
		fprintf(file, "Rank=5\n");
		fprintf(file, "Person=Sr. Nobody\n");
		fprintf(file, "Machine=Traditional         \n");
		fprintf(file, "Score=0\n");
		fprintf(file, "Rank=6\n");
		fprintf(file, "Person=Jr. Nobody\n");
		fprintf(file, "Machine=Detached            \n");
		fprintf(file, "Score=0\n");
		fprintf(file, "Rank=7\n");
		fprintf(file, "Person=Fr. Nobody\n");
		fprintf(file, "Machine=Symmetrical         \n");
		fprintf(file, "Score=0\n");
		fprintf(file, "Rank=8\n");
		fprintf(file, "Person=Mr. Nobody\n");
		fprintf(file, "Machine=Chambered           \n");
		fprintf(file, "Score=0\n");
		fprintf(file, "Rank=9\n");
		fprintf(file, "Person=Mrs. Nobody\n");
		fprintf(file, "Machine=Traditional         \n");
		fprintf(file, "Score=0\n");
		fprintf(file, "Rank=10\n");
		fprintf(file, "Person=Ms. Nobody\n");
		fprintf(file, "Machine=Detached            \n");
		fprintf(file, "Score=0\n");
		fputc(0,file);
		fputs("pHSC",file);
		fputc(0,file);
		fputc(OTH_TAG,file);
		fclose(file);
	}
	EM_moveSymFromExtMem (SYMSTR("pinhsc"), HS_NULL); 
	FILE *hsc = fopen ("pinhsc","rt");
	sym = SymFindPtr(SYMSTR("pinhsc"),0);
	char buffer[30];
	short scores[10];
	short rank[10];
	char person[20];
	char machinetype[20];
	char persons[20*10];
	char machines[20*10];
	short error = 1;
	FontSetSys(F_4x6);
	PortSet(LCD_MEM, 239, 127);
	ClrScr();
	for(i=0; i<10; i++)
	{
		nullstring(&buffer, 30);
		error = 1;
		while(!(strncmp(buffer,"Rank=",5)==0)) fgets(buffer, sizeof(buffer), hsc);
		if(strncmp(buffer, "Rank=", 5) == 0)
		{
			rank[i] = atoi(&buffer[5]);
			error = 0;
		}
		nullstring(&buffer, 30);
		fgets(buffer, sizeof(buffer), hsc);
		if(strncmp(buffer, "Person=", 7) == 0)
		{
			strncpy(person, buffer+7, strlen(buffer+7)-1);
			error = 0;
			strncpy(persons+20*i, person, 20);
			nullstring(&person, 20);
		}
		nullstring(&buffer, 30);
		fgets(buffer, sizeof(buffer), hsc);
		if(strncmp(buffer, "Machine=", 8) == 0)
		{
			strncpy(machinetype, buffer+8, strlen(buffer+8)-1);
			error = 0;
			strncpy(machines+20*i, machinetype, 20);
			nullstring(&machinetype, 20);
		}
		nullstring(&buffer, 30);
		fgets(buffer, sizeof(buffer), hsc);
		if(strncmp(buffer, "Score=", 6) == 0)
		{
			scores[i] = atoi(&buffer[6]);
			error = 0; 
		}
		nullstring(&buffer, 30);
		if(error == 1)
		{
			while(!_keytest (RR_CLEAR)) {}
			cleanup();
			exit(0);
		}
	}
	for(i=0; i<10; i++)
	{
		if(score >= scores[i])
		{
			clrscr();
			FontSetSys(F_8x10);
			printf_xy(LCD_WIDTH/2-(11*8)/2,0,"High Score!");
			FontSetSys(F_6x8);
			printf_xy(0,12,"Enter Your Name:");
			char name[20];
			inputStr(name,12,24,20);
			int j;
			for(j=9; j>i-1; j--)
			{
				scores[j+1] = scores[j];
				strncpy(persons+20*(j+1),persons+20*(j),20);
				strncpy(machines+20*(j+1),machines+20*(j),20);
			}
			scores[i] = score;
			strncpy(persons+20*i, name, 20);
			char currmachine[20];
			if(machine == 1) strncpy(currmachine,"Traditional         ",20);
			if(machine == 2) strncpy(currmachine,"Detached            ",20);
			if(machine == 3) strncpy(currmachine,"Symmetrical         ",20);
			if(machine == 4) strncpy(currmachine,"Chambered           ",20);
			if(machine == 10) strncpy(currmachine,tname,20);
			strncpy(machines+20*i, currmachine, 20);
			break;
		}
	}
	clrscr();
	FontSetSys(F_8x10);
	printf_xy(LCD_WIDTH/2-(12*8)/2,0,"High Scores");
	FontSetSys(F_4x6);
	for(i=0; i<10; i++)
	{
		printf_xy(LCD_WIDTH/2-80,8*i+20,"%d",rank[i]);
		strncpy(person, persons+20*i, 20);
		printf_xy(LCD_WIDTH/2-60,8*i+20,"%s",person);
		nullstring(&person, 20);
		strncpy(machinetype, machines+20*i, 20);
		printf_xy(LCD_WIDTH/2,8*i+20,"%s",machinetype);
		nullstring(&machinetype, 20);
		printf_xy(LCD_WIDTH/2+60,8*i+20,"%d",scores[i]);
	}
	SymDel(SYMSTR("pinhsc"));
	FILE *file = fopen("pinhsc","wb");
	rewind(file);
	fprintf(file, "blank\n");
	for(i=0; i<10; i++) {
		fprintf(file, "Rank=%d\n", rank[i]);
		fprintf(file, "Person=%s\n", persons+20*i);
		fprintf(file, "Machine=%s\n", machines+20*i);
		fprintf(file, "Score=%d\n", scores[i]);
	}
	fputc(0,file);
	fputs("pHSC",file);
	fputc(0,file);
	fputc(OTH_TAG,file);
	fclose(file);
	ngetchx();
	SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
}

void nullstring(char (*string)[], unsigned long length) {
	unsigned int i;
	for(i=0; i<length; i++)
		(*string)[i] = 0;
}

void detectcollision(short (*objectflags)[], short (*objectx)[], short (*objecty)[], short (*ballflags)[], float (*ballx)[], float (*bally)[], float (*ballmovex)[], float (*ballmovey)[], float (*moveshiftx)[], float (*moveshifty)[], short (*launcherxy)[], short (*plungerxy)[], short (*cgravity)[], unsigned long *bigobjects[], unsigned short *medobjects[], unsigned char *smallobjects[], short *score, int *numballs, int *trueflag)
{
	int i;
	for(i=0; i<65; i++)
	{
		if((*objectflags)[i] == BigBumper && (*ballx)[0] + 8 >= (*objectx)[i] && (*ballx)[0] <= (*objectx)[i] + 16 && (*bally)[0] + 8 >= (*objecty)[i] && (*bally)[0] <= (*objecty)[i] + 16)
		{
			if((*ballx)[0] + 4 <= (*objectx)[i] + 8) (*ballmovex)[0] = -1.5*abs((*ballmovex)[0]);
			else (*ballmovex)[0] = 1.5*abs((*ballmovex)[0]);
			if((*bally)[0] + 4 <= (*objecty)[i] + 8) (*ballmovey)[0] = -1.5*abs((*ballmovey)[0]);
			else (*ballmovey)[0] = 1.5*abs((*ballmovey)[0]);
			*score = *score + 3;
		}		
		if((*objectflags)[i] == RectBumper && (*ballx)[0] + 8 >= (*objectx)[i] && (*ballx)[0] <= (*objectx)[i] + 8 && (*bally)[0] + 8 >= (*objecty)[i] && (*bally)[0] <= (*objecty)[i] + 16)
		{
			if((*ballx)[0] + 4 <= (*objectx)[i] + 4) (*ballmovex)[0] = -1.1*abs((*ballmovex)[0]);
			else (*ballmovex)[0] = 1.1*abs((*ballmovex)[0]);
			if((*bally)[0] + 4 <= (*objecty)[i] + 8) (*ballmovey)[0] = -1.1*abs((*ballmovey)[0]);
			else (*ballmovey)[0] = 1.1*abs((*ballmovey)[0]);
			*score = *score + 3;
		}
		if((*objectflags)[i] == Accelerator && (*ballx)[0] + 8 >= (*objectx)[i] && (*ballx)[0] <= (*objectx)[i] + 16 && (*bally)[0] + 8 >= (*objecty)[i] && (*bally)[0] <= (*objecty)[i] + 16)
		{
			(*ballmovey)[0] = -8;
			//(*ballmovex)[0] = 0;
		}
		if((*objectflags)[i] == SmallBumper && (*ballx)[0] + 8 >= (*objectx)[i] && (*ballx)[0] <= (*objectx)[i] + 8 && (*bally)[0] + 8 >= (*objecty)[i] && (*bally)[0] <= (*objecty)[i] + 8)
		{
			if((*ballx)[0] + 4 <= (*objectx)[i] + 4) (*ballmovex)[0] = -1.0*abs((*ballmovex)[0]);
			else (*ballmovex)[0] = 1.0*abs((*ballmovex)[0]);
			if((*bally)[0] + 4 <= (*objecty)[i] + 4) (*ballmovey)[0] = -1.0*abs((*ballmovey)[0]);
			else (*ballmovey)[0] = 1.0*abs((*ballmovey)[0]);
			*score = *score + 3;
		}
		if((*objectflags)[i] == BigRightDeltoid && (*ballx)[0] + 8 >= (*objectx)[i] && (*ballx)[0] <= (*objectx)[i] + 32 && (*bally)[0] + 8 >= (*objecty)[i] && (*bally)[0] <= (*objecty)[i] + 32)
		{
			int flag = 0;
			if((*ballx)[0] >= (*objectx)[i] + 28)
			{
				(*ballmovex)[0] = abs((*ballmovex)[0]);
				flag = 1;
			}
			if((*bally)[0] >= (*objecty)[i] + 28)
			{
				(*ballmovey)[0] = abs((*ballmovey)[0]);
				flag = 1;
			}
			if(flag == 0)
			{
				short balltempx = (*ballx)[0] + 4;
				short balltempy = (*bally)[0] + 4;
				float slope, y_int;
				slope = -1;
				y_int = (*objecty)[i] - (slope * ((*objectx)[i] + 27));
				float distance = abs(-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
				if(distance <= gravity)
				{
					float temp = abs((*ballmovex)[0]);
					(*ballmovex)[0] = -abs((*ballmovey)[0]);
					(*ballmovey)[0] = -temp;
				}
			}
		}
		if((*objectflags)[i] == BigLeftDeltoid && (*ballx)[0] + 8 >= (*objectx)[i] && (*ballx)[0] <= (*objectx)[i] + 32 && (*bally)[0] + 8 >= (*objecty)[i] && (*bally)[0] <= (*objecty)[i] + 32)
		{ 
			int flag = 0;
			if((*ballx)[0] + 8 <= (*objectx)[i] + 4)
			{
				(*ballmovex)[0] = -abs((*ballmovex)[0]);
				flag = 1;
			}
			if((*bally)[0] >= (*objecty)[i] + 28)
			{
				(*ballmovey)[0] = abs((*ballmovey)[0]);
				flag = 1;
			}
			if(flag == 0)
			{
				short balltempx = (*ballx)[0] + 4;
				short balltempy = (*bally)[0] + 4;
				float slope, y_int;
				slope = 1;
				y_int = (*objecty)[i] - (slope * ((*objectx)[i] + 5));
				float distance = abs(-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
				if(distance <= gravity)
				{
					float temp = abs((*ballmovex)[0]);
					(*ballmovex)[0] = abs((*ballmovey)[0]);
					(*ballmovey)[0] = -temp;
				}
			}
		}
		if((*objectflags)[i] == SmallRightDeltoid && (*ballx)[0] + 8 >= (*objectx)[i] && (*ballx)[0] <= (*objectx)[i] + 16 && (*bally)[0] + 8 >= (*objecty)[i] && (*bally)[0] <= (*objecty)[i] + 16)
		{
			int flag = 0;
			if((*ballx)[0] >= (*objectx)[i] + 15)
			{
				(*ballmovex)[0] = abs((*ballmovex)[0]);
				flag = 1;
			}
			if((*bally)[0] + 8 <= (*objecty)[i] + 1)
			{
				(*ballmovey)[0] = abs((*ballmovey)[0]);
				flag = 1;
			}
			if(flag == 0)
			{
				short balltempx = (*ballx)[0] + 4;
				short balltempy = (*bally)[0] + 4;
				float slope, y_int;
				slope = 1;
				y_int = (*objecty)[i] + 2 - (slope * (*objectx)[i]);
				float distance = abs(-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
				if(distance <= gravity)
				{
					float temp = abs((*ballmovex)[0]);
					(*ballmovex)[0] = -abs((*ballmovey)[0]);
					(*ballmovey)[0] = temp;
				}
			}
		}
		if((*objectflags)[i] == SmallLeftDeltoid && (*ballx)[0] + 8 >= (*objectx)[i] && (*ballx)[0] <= (*objectx)[i] + 16 && (*bally)[0] + 8 >= (*objecty)[i] && (*bally)[0] <= (*objecty)[i] + 16)
		{
			int flag = 0;
			if((*ballx)[0] + 8 <= (*objectx)[i] + 1)
			{
				(*ballmovex)[0] = -abs((*ballmovex)[0]);
				flag = 1;
			}
			if((*bally)[0] + 8 <= (*objecty)[i] + 1)
			{
				(*ballmovey)[0] = abs((*ballmovey)[0]);
				flag = 1;
			}
			if(flag == 0)
			{
				short balltempx = (*ballx)[0] + 4;
				short balltempy = (*bally)[0] + 4;
				float slope, y_int;
				slope = -1;
				y_int = (*objecty)[i] + 2 - (slope * ((*objectx)[i]+16));
				float distance = abs(-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
				if(distance <= gravity)
				{
					float temp = abs((*ballmovex)[0]);
					(*ballmovex)[0] = abs((*ballmovey)[0]);
					(*ballmovey)[0] = temp;
				}
			}
		}
		if((*objectflags)[i] == BinaryBumper && (*ballx)[0] + 8 >= (*objectx)[i] && (*ballx)[0] <= (*objectx)[i] + 32 && (*bally)[0] + 8 >= (*objecty)[i] && (*bally)[0] <= (*objecty)[i] + 10)
		{
			if((*bally)[0] + 8 <= (*objecty)[i] + 8) (*ballmovey)[0] = -abs(gravity);
			else (*ballmovey)[0] = abs(gravity);
			*score = *score + 5;
			(*ballmovex)[0] = random(8) - 4;
		}
		if((*objectflags)[i] == BallEater && (*ballx)[0] + 8 >= (*objectx)[i] && (*ballx)[0] <= (*objectx)[i] + 16 && (*bally)[0] + 8 >= (*objecty)[i] && (*bally)[0] <= (*objecty)[i] + 12)
		{
			GraySprite8 ((*ballx)[0], (*bally)[0], 8, smallobjects[0], doublescreen, SPRT_XOR);
			(*ballx)[0] = (*launcherxy)[0] + 4;
			(*bally)[0] = (*launcherxy)[1] - 4;
			*numballs = *numballs - 1;
			if(*numballs == -1) *trueflag = 0;
			(*ballmovex)[0] = 0;
			(*ballmovey)[0] = 0.0001;
			GraySprite8 ((*ballx)[0], (*bally)[0], 8, smallobjects[0], doublescreen, SPRT_XOR);
		}
		int j;
		if((*objectflags)[i] == Teleporter && (*ballx)[0] + 8 >= (*objectx)[i] && (*ballx)[0] <= (*objectx)[i] + 12 && (*bally)[0] + 8 >= (*objecty)[i] && (*bally)[0] <= (*objecty)[i] + 12)
		{
			GraySprite8 ((*ballx)[0], (*bally)[0], 8, smallobjects[0], doublescreen, SPRT_XOR);
			for(j=i+1; j<65; j++)
			{
				if((*objectflags)[j] == Teleporter && j != i)
				{
					(*ballx)[0] = (*objectx)[j] + 2;
					(*bally)[0] = (*objecty)[j] + 6 + 18*(abs((*ballmovey)[0])/(*ballmovey)[0]);
					GraySprite8 ((*ballx)[0], (*bally)[0], 8, smallobjects[0], doublescreen, SPRT_XOR);
					break;
				}
				if(j>63) j = 0;
			}
		}
		if(use_flags == 1 && (*objectflags)[i] == Flag && (*ballx)[0] + 8 >= (*objectx)[i] && (*ballx)[0] <= (*objectx)[i] + 8 && (*bally)[0] + 8 >= (*objecty)[i] && (*bally)[0] <= (*objecty)[i] + 8)
		{
			(*objectflags)[i] = -Flag;
			GraySprite8 ((*objectx)[i], (*objecty)[i], 8, smallobjects[8], doublescreen, SPRT_XOR);
			(*ballmovey)[0] = -(*ballmovey)[0];
			*score += 4;
		}
	}
	for(i=65; i<75; i++)
	{
		short balltempx = (*ballx)[0] + 4;
		short balltempy = (*bally)[0] + 4;
		float slope = 0, y_int;
		int flag1 = 0;
		int flag2 = 0;
		int usedflag = 0;
		float topdistflag = 1000;
		float bottomdistflag = 1000;
		float lfdist_topline = 1000.1, lfdist_bottomline = 1000.1, rfdist_topline = 1000.1, rfdist_bottomline = 1000.1; //these are initialized to 1000.1 so I can safely make comparisons with them, and still get a false answer if the ball wasn't near the flipper
		if((*objectflags)[i] > 1 && balltempx + 4 >= (*objectx)[i] && balltempx - 4 <= (*objectx)[i] + 26 && balltempy >= (*objecty)[i] - 4 && balltempy <= (*objecty)[i] + 4 + 12) {
			flag1 = 2;
			usedflag = 1;
			slope = 0;
			y_int = (*objecty)[i] - (slope * (*objectx)[i]);
			lfdist_topline = abs(-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
			topdistflag = (-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
			if((*ballmovey)[0] > 0) slope = -0.0; //the actual slope is -8/27
			else slope = -0.296296296296;
			y_int = (*objecty)[i] + 8 - (slope * (*objectx)[i]);
			lfdist_bottomline = abs(-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
			bottomdistflag = (-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
			if(flag1 == 2 && lfdist_topline <= 10 && (*objectflags)[i] > 1 && (topdistflag <= 0 || bottomdistflag <= 0))
			{
				(*ballmovey)[0] = -abs(abs((*objectflags)[i]));
				(*ballmovex)[0] = (*ballmovex)[0] + 3;
			}
			else if(lfdist_bottomline <= 10 && (*objectflags)[i] > 1 && topdistflag > 0 && bottomdistflag > 0) {
				(*ballmovey)[0] = 3;
				(*ballmovex)[0] = (*ballmovex)[0] + 1.5;
			}
		}
		if((*objectflags)[i] == 1 && balltempx + 4 >= (*objectx)[i] && balltempx - 4 <= (*objectx)[i] + 26 && balltempy >= (*objecty)[i] - 4 && balltempy <= (*objecty)[i] + 4 + 12) {
			flag1 = 1;
			usedflag = 1;
			slope = 0.481481481481; //13/27
			y_int = (*objecty)[i] - (slope * (*objectx)[i]);
			lfdist_topline = abs(-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
			topdistflag = (-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
			slope = 0.185185185185; //the actual slope is 5/27
			y_int = (*objecty)[i] + 8 - (slope * (*objectx)[i]);
			lfdist_bottomline = abs(-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
			bottomdistflag = (-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
			if((*objectflags)[i] == 1 && lfdist_topline <= 10 && flag1 == 1 && (topdistflag <= 0 || bottomdistflag <= 0))
			{
				(*ballmovey)[0] = -1.5;
				(*ballmovex)[0] = (*ballmovex)[0] + 1;
			}
			else if(lfdist_bottomline <= 10 && (*objectflags)[i] == 1 && topdistflag > 0 && bottomdistflag > 0) {
				(*ballmovey)[0] = 3;
				(*ballmovex)[0] -= 0.5;
			}
		}
		if((*objectflags)[i] < -1 && balltempx - 4 <= (*objectx)[i] + 32 && balltempx + 4 >= (*objectx)[i] + 6 && balltempy >= (*objecty)[i] - 4 && balltempy <= (*objecty)[i] + 4 + 12) {
			flag2 = 2;
			usedflag = 1;
			slope = 0;
			y_int = (*objecty)[i] - (slope * ((*objectx)[i]+32-8));
			rfdist_topline = abs(-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
			topdistflag = (-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
			if((*ballmovey)[0] > 0) slope = 0.0;
			else slope = 0.296296296296;
			y_int = (*objecty)[i]+8 - (slope * ((*objectx)[i]+32-8));
			rfdist_bottomline = abs(-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
			bottomdistflag = (-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
			if(flag2 == 2 && rfdist_topline <= 10 && (*objectflags)[i] < -1 && (topdistflag <= 0 || bottomdistflag <= 0))
			{
				(*ballmovey)[0] = -abs(abs((*objectflags)[i]));
				(*ballmovex)[0] = (*ballmovex)[0] - 3;
			}
			else if(rfdist_bottomline <= 10 && (*objectflags)[i] < -1 && topdistflag > 0 && bottomdistflag > 0) {
				(*ballmovey)[0] = 3;
				(*ballmovex)[0] -= 1.5;
			}
		}
		if((*objectflags)[i] == -1 && balltempx - 4 <= (*objectx)[i] + 32 && balltempx + 4 >= (*objectx)[i] + 6 && balltempy >= (*objecty)[i] - 4 && balltempy <= (*objecty)[i] + 4 + 12) {
			flag2 = 1;
			usedflag = 1;
			slope = -0.481481481481;
			y_int = (*objecty)[i] - (slope * ((*objectx)[i]+32));
			rfdist_topline = abs(-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
			topdistflag = (-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
			slope = -0.185185185185;
			y_int = (*objecty)[i] + 8 - (slope * ((*objectx)[i]+32));
			rfdist_bottomline = abs(-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
			bottomdistflag = (-slope*balltempx + balltempy - y_int)/(sqrt((-slope)*(-slope) + 1));
			if((*objectflags)[i] == -1 && rfdist_topline <= 10 && flag2 == 1 && (topdistflag <= 0 || bottomdistflag <= 0))
			{
				(*ballmovey)[0] = -1.5;
				(*ballmovex)[0] = (*ballmovex)[0] -1;
			}
			else if((*objectflags)[i] == -1 && rfdist_bottomline <= 10 && topdistflag > 0 && bottomdistflag > 0)
			{
				(*ballmovey)[0] = 3;
				(*ballmovex)[0] += 0.5;
			}
		}
	}
	FontSetSys(F_4x6);
}

void moveball(short (*ballflags)[], float (*ballx)[], float (*bally)[], float (*ballmovex)[], float (*ballmovey)[], float (*moveshiftx)[], float (*moveshifty)[], unsigned char *smallobjects[], short *updateflag)
{
	if((*ballflags)[0] > 0)
	{
		GraySprite8 ((*ballx)[0], (*bally)[0], 8, smallobjects[0], doublescreen, SPRT_XOR);
		(*bally)[0] = (*bally)[0] + (*ballmovey)[0];
		(*ballx)[0] = (*ballx)[0] + (*ballmovex)[0];
		if(horizontalgravity == 0) (*moveshiftx)[0] = 0;
		float temp = abs((*ballmovex)[0]) - 0.055;
		if(temp < 0) temp = 0;
		if((*ballmovex)[0] < 0) temp = -abs(temp);
		(*ballmovex)[0] = temp;
		(*ballmovex)[0] = (*ballmovex)[0] + (*moveshiftx)[0];
		(*ballmovey)[0] = (*ballmovey)[0] + (gravity/4)*(*moveshifty)[0];
		if((*ballmovex)[0] < -(8-gravity)) (*ballmovex)[0] = -(8 - gravity);
		if((*ballmovex)[0] > 8-gravity) (*ballmovex)[0] = 8 - gravity;
		if((*ballmovey)[0] > gravity) (*ballmovey)[0] = gravity - ((gravity/2)-0.5);
		if((*ballx)[0] < 0) {
			//int temp = (*ballmovex)[0] / abs((*ballmovex)[0]);
			(*ballmovex)[0] = abs((*ballmovex)[0]) - 0.25;
			if((*ballmovex)[0] < 0) (*ballmovex)[0] = 0;
			(*ballmovex)[0] = -1 * (*ballmovex)[0];
			if((*ballmovex)[0] < 0) (*ballmovex)[0] = -1 * (*ballmovex)[0];
			(*ballx)[0] = 0;
			/*int temp2 = (*moveshiftx)[0] / abs((*moveshiftx)[0]);
			(*moveshiftx)[0] = abs((*moveshiftx)[0]);
			(*moveshiftx)[0] = (*moveshiftx)[0] - 0.2;
			if((*moveshiftx)[0] < 0) (*moveshiftx)[0] = 0;
			(*moveshiftx)[0] = -temp2 * (*moveshiftx)[0];*/
		}
		if((*ballx)[0] > 160 - 8) {
			//int temp = (*ballmovex)[0] / abs((*ballmovex)[0]);
			(*ballmovex)[0] = abs((*ballmovex)[0]) - 0.25;
			if((*ballmovex)[0] < 0) (*ballmovex)[0] = 0;
			(*ballmovex)[0] = -1 * (*ballmovex)[0];
			if((*ballmovex)[0] > 0) (*ballmovex)[0] = -1 * (*ballmovex)[0];
			(*ballx)[0] = 160 - 8;
			/*int temp2 = (*moveshiftx)[0] / abs((*moveshiftx)[0]);
			(*moveshiftx)[0] = abs((*moveshiftx)[0]);
			(*moveshiftx)[0] = (*moveshiftx)[0] - 0.2;
			if((*moveshiftx)[0] < 0) (*moveshiftx)[0] = 0;
			(*moveshiftx)[0] = -temp2 * (*moveshiftx)[0];*/
		}
		if((*bally)[0] > 200 - 8 && tablepart == 1) {
			(*ballmovey)[0] = abs((*ballmovey)[0]) - 1.5;
			if((*ballmovey)[0] < 0) (*ballmovey)[0] = 0;
			(*ballmovey)[0] = -1 * (*ballmovey)[0];
			(*bally)[0] = 200-8;
		}
		if((*bally)[0] < 0)
		{
			(*ballmovey)[0] = abs((*ballmovey)[0]) - 1;
			if((*ballmovey)[0] < 0) (*ballmovey)[0] = 0;
			(*bally)[0] = 1;
			(*bally)[0] = (*bally)[0] + (*ballmovey)[0];
		}
		GraySprite8 ((*ballx)[0], (*bally)[0], 8, smallobjects[0], doublescreen, SPRT_XOR);
		if(horizontalgravity == 1)
		{
			if((*ballx)[0] >= 160/2) (*moveshiftx)[0] = -0.2;
			if((*ballx)[0] < 160/2) (*moveshiftx)[0] = 0.2;
		}
	}
}

void moveplunger(short (*plungerxy)[], float *plungerpower, unsigned char *smallobjects[])
{
	if((*plungerpower < 4)) {
		*plungerpower = *plungerpower + 0.25;
		int intnum;
		intnum = *plungerpower;
		if(intnum == *plungerpower)
		{
			if((*plungerxy)[1] > 0 && (*plungerxy)[1] < 200) GraySprite8 ((*plungerxy)[0], (*plungerxy)[1], 16, smallobjects[6], doublescreen, SPRT_XOR);
			(*plungerxy)[1] = (*plungerxy)[1] + 1;
			if((*plungerxy)[1] > 0 && (*plungerxy)[1] < 200) GraySprite8 ((*plungerxy)[0], (*plungerxy)[1], 16, smallobjects[6], doublescreen, SPRT_XOR);
		}
	}
}

void leftflipperup(short (*objectflags)[], short (*objectx)[], short (*objecty)[], unsigned long *bigobjects[])
{
	int i;
	for(i=65; i<75; i++) {
		if((*objectflags)[i] > 0 && (*objecty)[i] > 0 && (*objecty)[i] < 200) {
			GraySprite32((*objectx)[i], (*objecty)[i], 13, bigobjects[4], doublescreen, SPRT_XOR);
			GraySprite32((*objectx)[i], (*objecty)[i], 13, bigobjects[3], doublescreen, SPRT_XOR);
		}
	}
}

void leftflipperdown(short (*objectflags)[], short (*objectx)[], short (*objecty)[], unsigned long *bigobjects[])
{
	int i;
	for(i=65; i<75; i++) {
		if((*objectflags)[i] > 0 && (*objecty)[i] > 0 && (*objecty)[i] < 200) {
			GraySprite32((*objectx)[i], (*objecty)[i], 13, bigobjects[3], doublescreen, SPRT_XOR);
			GraySprite32((*objectx)[i], (*objecty)[i], 13, bigobjects[4], doublescreen, SPRT_XOR);
		}
	}
}

void rightflipperup(short (*objectflags)[], short (*objectx)[], short (*objecty)[], unsigned long *bigobjects[])
{
	int i;
	for(i=65; i<75; i++) {
		if((*objectflags)[i] < 0 && (*objecty)[i] > 0 && (*objecty)[i] < 200) {
			GraySprite32((*objectx)[i], (*objecty)[i], 13, bigobjects[6], doublescreen, SPRT_XOR);
			GraySprite32((*objectx)[i], (*objecty)[i], 13, bigobjects[5], doublescreen, SPRT_XOR);
		}
	}
}

void rightflipperdown(short (*objectflags)[], short (*objectx)[], short (*objecty)[], unsigned long *bigobjects[])
{
	int i;
	for(i=65; i<75; i++) {
		if((*objectflags)[i] < 0 && (*objecty)[i] > 0 && (*objecty)[i] < 200) {
			GraySprite32((*objectx)[i], (*objecty)[i], 13, bigobjects[5], doublescreen, SPRT_XOR);
			GraySprite32((*objectx)[i], (*objecty)[i], 13, bigobjects[6], doublescreen, SPRT_XOR);
		}
	}
}

void getkey(short (*objectflags)[])
{
	int i = 0;
	if(_keytest (RR_F1) && lkeypress == 0)
	{
		for(i=65; i<75; i++) {
			if((*objectflags)[i] > 0) (*objectflags)[i] = 10;
		}
	}
	if((_keytest (RR_F5) || _keytest (RR_F8)) && rkeypress == 0)
	{
		for(i=65; i<75; i++) {
			if((*objectflags)[i] < 0) (*objectflags)[i] = -10;
		}
	}
}


void draw(short (*objectflags)[], short (*objectx)[], short (*objecty)[], short (*ballflags)[], float (*ballx)[], float (*bally)[], float (*ballmovex)[], float (*ballmovey)[], float (*moveshiftx)[], float (*moveshifty)[], short (*launcherxy)[], short (*plungerxy)[], short (*cgravity)[], unsigned long *bigobjects[], unsigned short *medobjects[], unsigned char *smallobjects[])
{
	int i;
	if((*cgravity)[0] == 1 && (*cgravity)[2] >= 0 && (*cgravity)[2] <= 200) GraySprite16 ((*cgravity)[1], (*cgravity)[2], 16, medobjects[6], doublescreen, SPRT_XOR);
	for(i=0; i<65; i++)
	{
		if((*objectflags)[i] == BigBumper && (*objecty)[i] >= 0 && (*objecty)[i] <= 200) GraySprite16 ((*objectx)[i], (*objecty)[i], 16, medobjects[3], doublescreen, SPRT_XOR);
		if((*objectflags)[i] == RectBumper && (*objecty)[i] >= 0 && (*objecty)[i] <= 200) GraySprite8 ((*objectx)[i], (*objecty)[i], 16, smallobjects[5], doublescreen, SPRT_XOR);
		if((*objectflags)[i] == Accelerator && (*objecty)[i] >= 0 && (*objecty)[i] <= 200) GraySprite16 ((*objectx)[i], (*objecty)[i], 20, medobjects[4], doublescreen, SPRT_XOR);
		if((*objectflags)[i] == SmallBumper && (*objecty)[i] >= 0 && (*objecty)[i] <= 200) GraySprite8 ((*objectx)[i], (*objecty)[i], 8, smallobjects[4], doublescreen, SPRT_XOR);
		if((*objectflags)[i] == BigLeftDeltoid && (*objecty)[i] >= 0 && (*objecty)[i] <= 200) GraySprite32 ((*objectx)[i], (*objecty)[i], 32, bigobjects[1], doublescreen, SPRT_XOR);
		if((*objectflags)[i] == BigRightDeltoid && (*objecty)[i] >= 0 && (*objecty)[i] <= 200) GraySprite32 ((*objectx)[i], (*objecty)[i], 32, bigobjects[2], doublescreen, SPRT_XOR);
		if((*objectflags)[i] == SmallRightDeltoid && (*objecty)[i] >= 0 && (*objecty)[i] <= 200) GraySprite16 ((*objectx)[i], (*objecty)[i], 16, medobjects[5], doublescreen, SPRT_XOR);
		if((*objectflags)[i] == SmallLeftDeltoid && (*objecty)[i] >= 0 && (*objecty)[i] <= 200) GraySprite16 ((*objectx)[i], (*objecty)[i], 16, medobjects[7], doublescreen, SPRT_XOR);
		if((*objectflags)[i] == BinaryBumper && (*objecty)[i] >= 0 && (*objecty)[i] <= 200) GraySprite32 ((*objectx)[i], (*objecty)[i], 10, bigobjects[0], doublescreen, SPRT_XOR);
		if((*objectflags)[i] == BallEater && (*objecty)[i] >= 0 && (*objecty)[i] <= 200) GraySprite16 ((*objectx)[i], (*objecty)[i], 12, medobjects[1], doublescreen, SPRT_XOR);
		if((*objectflags)[i] == Flag && (*objecty)[i] >= 0 && (*objecty)[i] <= 200) GraySprite8 ((*objectx)[i], (*objecty)[i], 8, smallobjects[8], doublescreen, SPRT_XOR);
		if((*objectflags)[i] == Teleporter && (*objecty)[i] >= 0 && (*objecty)[i] <= 200) GraySprite16 ((*objectx)[i], (*objecty)[i], 12, medobjects[0], doublescreen, SPRT_XOR);
	}
	if((*bally)[0] >= 0 && (*bally)[0] <= 200) GraySprite8 ((*ballx)[0], (*bally)[0], 8, smallobjects[0], doublescreen, SPRT_XOR);
	if((*launcherxy)[1] >= 0 && (*launcherxy)[1] <= 200) GraySprite16 ((*launcherxy)[0], (*launcherxy)[1], 8, medobjects[2], doublescreen, SPRT_XOR);
	if((*plungerxy)[1] >= 0 && (*plungerxy)[1] <= 200) GraySprite8 ((*plungerxy)[0], (*plungerxy)[1], 16, smallobjects[6], doublescreen, SPRT_XOR);
	for(i=65; i<75; i++)
	{
		if((*objectflags)[i] != 0 && (*objecty)[i] >= 0 && (*objecty)[i] <= 200)
		{
//			GraySprite8 ((*flipperx)[i], (*flippery)[i], 8, smallobjects[3], doublescreen, SPRT_XOR);
			if((*objectflags)[i] == 1)
			{
				GraySprite32((*objectx)[i], (*objecty)[i], 13, bigobjects[4], doublescreen, SPRT_XOR);
			}
			if((*objectflags)[i] > 1)
			{
				GraySprite32((*objectx)[i], (*objecty)[i], 13, bigobjects[3], doublescreen, SPRT_XOR);
			}
			if((*objectflags)[i] == -1)
			{
				GraySprite32((*objectx)[i], (*objecty)[i], 13, bigobjects[6], doublescreen, SPRT_XOR);
			}
			if((*objectflags)[i] < -1)
			{
				GraySprite32((*objectx)[i], (*objecty)[i], 13, bigobjects[5], doublescreen, SPRT_XOR);
			}
		}
	}
}

void gamemenu(void)
{
	//OSSetSR(0x0000);
	ClrScr();
	if(initilized == 1) cleanup();
	menus:
	title = " TI Pinball 0.2 ";
	str1 = "Start Game";
	str2 = "Game Options";
	str3 = "Game Controls";
	str4 = "About this Game";
	str5 = "Exit this Game";
	short termflags[] = {14,1,1,1,1,1,0,0,0,0};
	short r = 0;
	menu(100,65,5,&termflags,&r,title,str1,str2,str3,str4,str5);
	//this is what the program does after the menu, depending on what r is
	if(r == 1)
	{
		title = " Select a Table ";
		str1 = "Traditional";
		str2 = "Detached";
		str3 = "Symmetrical";
		str4 = "Chambered";
		str5 = "Load Custom";
		short tableflags[] = {14,1,1,1,1,1,0,0,0,0};
		short j = 0;
		menu(150,65,5,&tableflags,&j,title,str1,str2,str3);
		if(j == 1)
		{
			machine = 1;
		}
		if(j == 2)
		{
			machine = 2;
		}
		if(j == 3)
		{
			machine = 3;
		}
		if(j == 4)
		{
			machine = 4;
		}
		if(j == 5) {
			make_menu_box(40,150,LCD_WIDTH/2-75,30);
			FontSetSys(F_6x8);
			DrawStr(LCD_WIDTH/2-48,35,"Enter Table Name",A_REVERSE);
			char name[8];
			inputStr(name,LCD_WIDTH/2-24,50,8);
			SYM_ENTRY *sym = SymFindPtr(SYMSTR(name),0);
			ESQ type = *(HToESI (sym->handle));
			if(!sym || type != OTH_TAG) {
				make_menu_box(40,150,LCD_WIDTH/2-75,30);
				DrawStr(LCD_WIDTH/2-18,35,"Error!",A_REVERSE);
				DrawStr(LCD_WIDTH/2-51,50,"Cannot Find Table",A_NORMAL);
				ngetchx();
				ClrScr();
				goto menus;
			}
			strncpy(cname, name, 8);
			EM_moveSymFromExtMem(SYMSTR(cname),HS_NULL);
			FILE *fname = fopen(cname,"r+t");
			char buf[100];
			while(strncmp(buf, "Table=", 6) != 0) fgets(buf, 100, fname);
			strncpy(tname,buf+6,20);
			fclose(fname);
			machine = 10;
		}
		//OSSetSR(0x0400);
		ClrScr();
		if(initilized == 0) startup();
	}
	if(r == 2)
	{
		options:
		title = " Game Options ";
		str1 = "Difficulty";
		str2 = "Unstable Gravity";
		str3 = "Grayscale";
		str4 = "Back";
		short optflags[] = {12,1,1,1,1,0,0,0,0,0,0};
		short s = 0;
		menu(100,60,4,&optflags,&s,title,str1,str2,str3,str4);
		if(s == 1)
		{
			title = " Difficulty ";
			str1 = "Lunar Gravity";
			str2 = "Earth Gravity";
			str3 = "Solar Gravity";
			short z = 0;
			short dflags[] = {10,1,1,1,0,0,0,0,0,0,0};
			menu(100,55,3,&dflags,&z,title,str1,str2,str3);
			if(z == 1)
			{
				gravity = 3;
			}
			if(z == 2)
			{
				gravity = 5;
			}
			if(z == 3)
			{
				gravity = 7;
			}
			goto options;
		}
		if(s == 2)
		{
			title = " Unstable Gravity ";
			str1 = "On";
			str2 = "Off";
			short z = 0;
			short sflags[] = {17,1,1,0,0,0,0,0,0,0,0};
			menu(100,35,2,&sflags,&z,title,str1,str2);
			if(z == 1)
			{
				horizontalgravity = 1;
			}
			if(z == 2)
			{
				horizontalgravity = 0;
			}
			goto options;
		}
		if(s == 3)
		{
			title = " Grayscale ";
			str1 = "On";
			str2 = "Off";
			short f = 0;
			short fflags[] = {11,1,1,0,0,0,0,0,0,0,0};
			menu(100,35,2,&fflags,&f,title,str1,str2);
			if(f == 1)
			{
				grayflag = 1;
			}
			if(f == 2)
			{
				grayflag = 0;
			}
			goto options;
		}
		if(s == 4) goto menus;
	}
	if(r == 3)
	{
		ClrScr();
		FontSetSys(F_4x6);
		printf_xy(0,0,"Move Left Flippers:  F1");
		printf_xy(0,16,"Move Right Flippers: F5 or F8");
		printf_xy(0,32,"Rectract Launcher:  Down");
		printf_xy(0,48,"Toggle Unstable Gravity:  F3");
		printf_xy(0,64,"Pause: Mode");
		ngetchx();
		ClrScr();
		goto menus;
	}
	if(r == 4)
	{
		ClrScr();
		FontSetSys(F_6x8);
		printf_xy(0,0,"TI Pinball 0.2");
		FontSetSys(F_4x6);
		printf_xy(0,10,"Author:  Jude Nelson");
		printf_xy(0,18,"Contact:  judecn@aol.com");
		printf_xy(0,36,"Sponsored by www.ticalc.org");
		printf_xy(0,44,"and www.nexusprograms.net");
		printf_xy(0,60,"This program was written entirely in C");
		printf_xy(0,68,"using the TIGCC IDE.  There is no ");
		printf_xy(0,76,"waranty to this program, and therefore ");
		printf_xy(0,84,"anything that this program causes is not");
		printf_xy(0,92,"my fault.");
		ngetchx();
		ClrScr();
		goto menus;
	}
	if(r == 5)
	{
		//OSSetSR(0x0000);
		if(initilized == 1) cleanup();
		exit(0);
	}
}

void menu(short width, short height, short numterms, short (*termflags)[], short *r, ...)
{
	//width is the width of the menu
	//height is the height of the menu
	//numterms is the number of terms to be put in the menu
	//(*termflags)[] is the length of each term
	//the ... allows one to put in as many as ten strings, which are the terms
	short x,y,i;
	height -= 3;
	x = (LCD_WIDTH/2) - (width/2);
	y = (LCD_HEIGHT/2) - (height/2);
	short spacing = 8;
	ClrScr();
	make_menu_box(height, width, x, y);
	short x2 = x + (width - (width - 15));
	short y2 = y+6;
	for(i=1; i<11; i++)
	{
		if((*termflags)[i-1] == 1)
		{
			FontSetSys(F_4x6);
			if(i==2) printf_xy(x2, y2+((i-1)*spacing), str1);
			if(i==3) printf_xy(x2, y2+((i-1)*spacing), str2);
			if(i==4) printf_xy(x2, y2+((i-1)*spacing), str3);
			if(i==5) printf_xy(x2, y2+((i-1)*spacing), str4);
			if(i==6) printf_xy(x2, y2+((i-1)*spacing), str5);
			if(i==7) printf_xy(x2, y2+((i-1)*spacing), str6);
			if(i==8) printf_xy(x2, y2+((i-1)*spacing), str7);
			if(i==9) printf_xy(x2, y2+((i-1)*spacing), str8);
			if(i==10) printf_xy(x2, y2+((i-1)*spacing), str9);
			if(i==11) printf_xy(x2, y2+((i-1)*spacing), str0);
		}
		if((*termflags)[i-1] > 1)
		{
			short x3 = (LCD_WIDTH/2) - (4*(*termflags)[i-1]/2);
			short y3 = y + 6;
			FontSetSys(F_4x6);
			DrawStr(x3,y3,title,SPRT_XOR);
			//drawbox(x3-2,y3-2,4*(*termflags)[i-1]+2,8,1);
		}
	}
	unsigned char selector[] = {
		0b00110000,
		0b01111000,
		0b11001100,
		0b11001100,
		0b01111000,
		0b00110000
	};
	short spx = x2 - 8;
	short spy = (y2 - 1) + spacing + 1;
	short returnflag = 0;
	short selection = 1;
	Sprite8 (spx, spy, 6, selector, LCD_MEM, SPRT_XOR);
	while(returnflag == 0)
	{
		short k = ngetchx();
		if(k == KEY_UP && spy > (y2-1)+2*spacing)
		{
			Sprite8 (spx, spy, 6, selector, LCD_MEM, SPRT_XOR);
			spy = spy - spacing;
			selection = selection - 1;
			Sprite8 (spx, spy, 6, selector, LCD_MEM, SPRT_XOR);
		}
		if(k == KEY_DOWN && spy < (numterms * spacing) + y2)
		{
			Sprite8 (spx, spy, 6, selector, LCD_MEM, SPRT_XOR);
			spy = spy + spacing;
			selection = selection + 1;
			Sprite8 (spx, spy, 6, selector, LCD_MEM, SPRT_XOR);
		}
		if(k == KEY_CLEAR) {ClrScr(); exit(0);}
		if(k == 13)
		{
			//the number in *r is the rth term in the menu in this switch/case statement
			switch (selection)
			{
				case 1:
					*r = 1;
					break;
				case 2:
					*r = 2;
					break;
				case 3:
					*r = 3;
					break;
				case 4:
					*r = 4;
					break;
				case 5:
					*r = 5;
					break;
				case 6:
					*r = 6;
					break;
				case 7:
					*r = 7;
					break;
				case 8:
					*r = 8;
					break;
				case 9:
					*r = 9;
					break;
				case 10:
					*r = 10;
					break;
			}
			returnflag = 1;
		}
	}
}

void drawbox(short x, short y, short width, short height, short fill)
{
	DrawLine(x,y,x+width,y,A_XOR);
	DrawLine(x,y,x,y+height,A_XOR);
	DrawLine(x+width,y,x+width,y+height,A_XOR);
	DrawLine(x,y+height,x+width,y+height,A_XOR);
	if(fill == 1)
	{
		short i;
		for(i=1; i<height; i++)
		{
			DrawLine(x,y+i,x+width,y+i,A_XOR);
		}
	}
}

//Kevin Chang designed this.  Thanks!

void logo() {
	unsigned long part1[] = {
		 0x00000001,0x00000002,0x00000004
		,0x00000008,0x00000010,0x007C0020,0x03838040
		,0x04004080,0x1B003100,0x17E01200,0x2C800C00
		,0x4F000800,0x58001000,0x50002001,0x90004006
		,0x80008018,0x80010060,0x80020180,0x80040600
		,0x40081801,0x40106005,0x4021800D,0x2046002F
		,0x1098006F,0x1960026F,0x07800F6F,0x06001B6D
		,0x04001A6C,0x04041E68,0x040C1840,0x046C5801
		,0x05ECD806,0x05CC9018,0x04CC0060,0x04CC0180
		,0x04CC0601,0x04C81802,0x04C0610A,0x0481814A
		,0x0406014A,0x0418014B,0x04600148,0x05800158
		,0x06000120,0x00010900,0x00820A00,0x035A0400
		,0x044A0000,0x048A0000,0x07440000,0x04440000
		,0x04440000,0x04880000,0x07000000,0x04000000
	};
	unsigned long part2[] = {
		 0x00003F80,0x0003C078,0x001C0307,0x0077C600
		,0x00A83C00,0x01541E00,0x03A033C0,0x04506039
		,0x0830401E,0x100E03E0,0x2001FC03,0x40000007
		,0x80000018,0x00000061,0x00000182,0x00000600
		,0x00001800,0x00006005,0x00018022,0x00060054
		,0x00180000,0x00600845,0x0180182A,0x06001941
		,0x1801182A,0x60031D15,0x800339AB,0x00335F15
		,0x00731EAE,0x04537958,0x1E53EAE0,0xB673C580
		,0xB47A2E01,0xBE5D5802,0xB6C0E112,0xB6D5816A
		,0xBC8E0142,0xB9580133,0xA0609D08,0x0580A550
		,0x0610B920,0x181CA100,0x6014A000,0x8012B800
		,0x00138000,0x44108000,0x4A100000,0x56100000
		,0x58000000,0xD2000000,0x54000000,0x48000000
		,0xC0000000
	};
	unsigned char part3[] = {
		 0x00,0x00,0x00,0xE0
		,0x10,0x08,0x38,0xC8
		,0x18,0x78,0x88,0x58
		,0x88,0x58,0x28,0x58
		,0xA8,0x58,0x28,0x58
		,0xA8,0x58,0xB8,0x78
		,0xF8,0xF8,0xE0,0x80
		,0x00,0x04,0x1A,0x1A
		,0xD2,0x52,0x52,0x50
		,0x40,0x80,0xF8,0xE0
		,0x80,0x00,0x04,0x1A
		,0x1A,0xD2,0x52,0x52
		,0x50,0x40,0x80
	};
	unsigned char x=0,y=0;
	x = ((LCD_WIDTH-71)/2);
	y = ((LCD_HEIGHT-68)/2);
	Sprite32(x,y+13,55,part1,LCD_MEM,SPRT_XOR);
	Sprite32(x+32,y,53,part2,LCD_MEM,SPRT_XOR);
	Sprite8(x+64,y,38,part3,LCD_MEM,SPRT_XOR);
	printf_xy(((LCD_WIDTH-96)/2),92,"Press any key...");
	ngetchx();
	ClrScr();
}



void GraySprite8 (short x, short y, short h, char *spr, char *foo, short mode)
{
	if(grayflag == 1) {
		Sprite8 (x, y, h, spr, lightdoublescreen, mode);
		Sprite8 (x, y, h, spr, darkdoublescreen, mode);
		Sprite8 (x, y, h, spr+h, lightdoublescreen, mode);
	}
	else Sprite8 (x, y, h, spr, foo, mode);
}

void GraySprite16 (short x, short y, short h, short *spr, char *foo, short mode)
{
	if(grayflag == 1) {
		Sprite16 (x, y, h, spr, lightdoublescreen, mode);
		Sprite16 (x, y, h, spr, darkdoublescreen, mode);
		Sprite16 (x, y, h, spr+h, lightdoublescreen, mode);
	}
	else Sprite16 (x, y, h, spr, foo, mode);
}

void GraySprite32 (short x, short y, short h, long *spr, char *foo, short mode)
{
	if(grayflag == 1) {
		Sprite32 (x, y, h, spr, lightdoublescreen, mode);
		Sprite32 (x, y, h, spr, darkdoublescreen, mode);
		Sprite32 (x, y, h, spr+h, lightdoublescreen, mode);
	}
	else Sprite32 (x, y, h, spr, foo, mode);
}