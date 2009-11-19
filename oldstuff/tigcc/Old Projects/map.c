// C Source File
// Created 11/15/03; 4:49:22 PM

#define USE_TI89              // Compile for TI-89

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files
#include "map.h"

INT_HANDLER interrupt1;

unsigned long int map[]= 
	{
		0b01100000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b10000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000,
		0b00000000000000000000000000000000
	};

void makeMap(POSITION pos) {
	short int row, column;
	unsigned long int mask;
	
	
	for (row=pos.y; row < pos.y+11; row++) {
		mask = 0b1<<(31-pos.x);
		
		for (column = pos.x; column < pos.x+20; column++) {
			mask >>= 1;
			
			if (map[row] & mask) {
				Sprite8(column*8,row*8,8,box,LCD_MEM,SPRT_XOR);
			}
		}
	}
}

void getKeyMasks(char *keys) {
		// find the key masks
		keys[0] = 0x01;	// bit 0	Up
		keys[1] = 0x04;	// bit 2	Down
		keys[2] = 0x02;	// bit 1	Left
		keys[3] = 0x08;	// bit 3	Right
		keys[4] = 0x10; // bit 4	2nd
		keys[5] = 0x20; // bit 5	Shift
		keys[6] = 0x40; // bit 6	Diamond
		keys[7] = 0x80; // bit 7	Alpha
}

// main menu
inline void title(void) {
	clrscr();
	DrawStr(LCD_WIDTH/2-30,LCD_HEIGHT/2-4,"HIT ENTER!",A_NORMAL);
	ngetchx();
}

// delay program
inline void delay(unsigned int number) {
	unsigned short loop, loop2;
	for (loop=0;loop<number;loop++) {
		for (loop2=0;loop2<10000;loop2++);
	}
}

// dimension check
int dim(POSITION pos, BITS bit) {
	pos.x+=bit.xright+(-bit.xleft);
	pos.y+=bit.yup+(-bit.ydown);
	
	if ((pos.x>=0) && pos.x<32 && pos.y>=0 && pos.y<32) {
		return 1;
	}
	return 0;
}

// draw thing
inline void draw(unsigned char sprite[],POSITION pos,POSITION map) {
	POSITION real={0,0};
	real.x=pos.x-map.x;
	real.y=pos.y-map.y;
	Sprite8(real.x*8,real.y*8,8,sprite,LCD_MEM,SPRT_XOR);
}

// check if user pressed any button
inline short int button(int row, int key) {
	if (_rowread(row) & key) {
		return 1;
	}	
	return 0;
}

// Main Function
void _main(void)
{	
	// declare variables
	char keys[8]; getKeyMasks(keys);
	BITS bit= {0,0,0,0,0,0,0,0};
	char arrow_key=0;
	POSITION thingPOS= {0,0};
	POSITION mapPOS= {0,0};
	// main menu
	title();
	
	// prepare for GRAYSCALE AND ROWREAD
	interrupt1 = GetIntVec(AUTO_INT_1);
	SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
	
	while (1) {
		
		// initialize
		bit.one=0;
		thingPOS= (POSITION){0,0};
		
		// a blank slate
		ClrScr();
		
		//setup game, draw screen, etc
		draw(thing,thingPOS,mapPOS);
		makeMap(mapPOS);
		while (!button(ARROW_ROW,keys[UP]));
		
		while(!button(TI89_ESCROW,TI89_ESCKEY) && !bit.one) {
			// the game loop
			arrow_key=_rowread(ARROW_ROW);
			bit=(BITS){0,0,0,0,0,0,0,0};
			
			//get input
			if (arrow_key) {
				if (arrow_key & keys[UP]) bit.yup=1;
				if (arrow_key & keys[RIGHT]) bit.xright=1;
				if (arrow_key & keys[DOWN]) bit.ydown=1;
				if (arrow_key & keys[LEFT]) bit.xleft=1;
			}
			
			// test gunner is within dimension
			if (dim(thingPOS,bit)) {
				bit.draw=1;
				thingPOS.x+=bit.xright+(-bit.xleft);
				thingPOS.y+=bit.yup+(-bit.ydown);
			}
			
			if (bit.draw) {
			mapPOS.x=thingPOS.x-10;
			mapPOS.y=thingPOS.y-5;
			if (mapPOS.x<0) mapPOS.x=0;
			else if (mapPOS.x>11) mapPOS.x=11;
			if (mapPOS.y<0) mapPOS.y=0;
			else if (mapPOS.y>20) mapPOS.y=20;
			}
			
			// if movement=true, draw new position
			if (bit.draw) {
				ClrScr();
				makeMap(mapPOS);
				draw(thing,thingPOS,mapPOS);
			}
			
			//delay
			delay(6);
		} 
		if (!bit.one) {
			SetIntVec(AUTO_INT_1,interrupt1);
			return;
		}	  														//while !quit and !done
	} 																														//infinite loop
}