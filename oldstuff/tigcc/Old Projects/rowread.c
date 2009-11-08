// C Source File
// Created 21/Oct/03; 19:37:00

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files
#include "bitmanip.h"

inline void drawBlock(POSITION pos) {
	Sprite8(pos.x*BLOCK_WIDTH,pos.y*BLOCK_HEIGHT,BLOCK_HEIGHT,block,LCD_MEM,SPRT_XOR);
}

inline void eraseBlock(POSITION pos) {
	drawBlock(pos);
}

inline void moveBlock(POSITION oldPosition, POSITION newPosition) {
	eraseBlock(oldPosition);
	drawBlock(newPosition);
}

void move(POSITION *pos, short int direction) {
	POSITION oldPosition = *pos;
	
	switch (direction) {
		case UP:
			// if we can move up, then do so
			if (!isWall((POSITION){pos->x,pos->y-1})) {
				pos->y--;
				moveBlock(oldPosition,*pos);
			}
			break;
		case DOWN:
			// if we can move down, then do so
			if (!isWall((POSITION){pos->x,pos->y+1})) {
				pos->y++;
				moveBlock(oldPosition,*pos);
			}
			break;
		case LEFT:
			// if we can move left, then do so
			if (!isWall((POSITION){pos->x-1,pos->y})) {
				pos->x--;
				moveBlock(oldPosition,*pos);
			}
			break;
		case RIGHT:
			// if we can move right, then do so
			if (!isWall((POSITION){pos->x+1,pos->y})) {
				pos->x++;
				moveBlock(oldPosition,*pos);
			}
			break;
	}
}

void getKeyMasks(short int *keys, short int calc) {
	// find the correct key masks based on which calculator we have
	if (calc == 0) {		// do we have a TI-89
		keys[0] = 0x0001;	// bit 0
		keys[1] = 0x0004;	// bit 2
		keys[2] = 0x0002;	// bit 1
		keys[3] = 0x0008;	// bit 3
	} else {			// then we must have a TI-92+
		keys[0] = 0x0020;	// bit 5
		keys[1] = 0x0080;	// bit 7
		keys[2] = 0x0010;	// bit 4
		keys[3] = 0x0040;	// bit 6
	}
}

// slow the program down
void delay(void) {
	short int loop = 13000, temp = 0;
	
	// count to slow the program down
	while (loop-- > 0) {
		temp++;
	}
}

short int quit(short int calc) {
	if (calc == 0) {	// test for TI-89 ESC key
		if (_rowread(TI89_ESCROW) & TI89_ESCKEY) {
			return 1;
		}
	} else {		// test for TI-92+ ESC key
		if (_rowread(TI92_ESCROW) & TI92_ESCKEY) {
			return 1;
		}
	}
	
	return 0;
}

short int isWall(POSITION pos) {
	unsigned long int mask = 0x80000 >> pos.x, position = map[pos.y];

	// if mask and position collide, we hit a wall
	if (mask & position) {
		return 1;
	}
	
	// otherwise, we didn't
	return 0;
}

void drawMap(void) {
	short int row, column;
	unsigned long int mask, position;

	// clear the screen before drawing
	ClrScr();
	
	// loop through the map array
	for (row = 0; row < MAP_ROWS; row++) {
		// get the positions for this row
		position = map[row];
		
		// reset the bit mask
		mask = 0x100000;
		
		// loop through the column to see where in the row we need to draw blocks
		for (column = 0; column < MAP_COLUMNS; column++) {
			// shift the bit mask
			mask >>= 1;
			
			// check the position, if set, draw the block
			if (position & mask) {
				drawBlock((POSITION){column,row});
			}
		}
	}
}

// Main Function
void _main(void) {
	short int keys[4];
	short int calc = CALCULATOR, key;
	INT_HANDLER interrupt1 = GetIntVec(AUTO_INT_1);	// save auto-interrupt 1
	POSITION pos = {2,2};
	
	// destroy auto-interrupt 1 so that it doesn't interfere with _rowread()
	SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
	
	// get the correct key masks based on which calculator we have. The TI-89
	// has a different keyboard mapping than the TI-92+
	getKeyMasks(keys,calc);
	
	// draw the map on the screen
	drawMap();
	
	// draw the block on the screen at our initial position
	drawBlock(pos);
	
	// until the user presses ESC
	while (!quit(calc)) {
		key = _rowread(ARROW_ROW);
	
		// check for UP arrow
		if (key & keys[UP]) {
			move(&pos,UP);
		}
		
		// check for LEFT arrow
		if (key & keys[LEFT]) {
			move(&pos,LEFT);
		}
		
		// check for DOWN arrow
		if (key & keys[DOWN]) {
			move(&pos,DOWN);
		}
		
		// check for RIGHT arrow
		if (key & keys[RIGHT]) {
			move(&pos,RIGHT);
		}
		
		// slow the program down
		delay();
	}
	
	// restore auto interrupt 1
	SetIntVec(AUTO_INT_1,interrupt1);
}