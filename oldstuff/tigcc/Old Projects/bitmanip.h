// C Header File
// Created 21/Oct/03; 21:22:32
#ifndef _BITMANIP_H
#define _BITMANIP_H

// C Header File
// Created 3/17/01; 12:36:35 AM

/* Constants */
enum MapDimensions {MAP_ROWS = 19, MAP_COLUMNS = 20};
enum BlockConstants {BLOCK_HEIGHT = 5, BLOCK_WIDTH = 8};

enum ArrowKeys {UP,DOWN,LEFT,RIGHT};
enum KeyMatrix {TI89_ESCROW = ~0x0040, TI89_ESCKEY = 0x0001,
		TI92_ESCROW = ~0x0100, TI92_ESCKEY = 0x0040,
		ARROW_ROW = ~0x0001};

/* Structure Definitions */

typedef struct {
	unsigned short int x : 8, y : 8;
} POSITION;

/* Sprite Definitions */

static unsigned long int map[] = {0xFFFFF,0x80001,0x80001,0x80001,0x80401,0x80401,0x80401,0x80401,
				 0x80401,0x87FC1,0x80401,0x80401,0x80401,0x80401,0x80401,0x80001,
				 0x80001,0x80001,0xFFFFF};
				 
static unsigned char block[] = {0xFF,0xFF,0xFF,0xFF,0xFF};

/* Function Proto-types */

inline void drawBlock(POSITION);
inline void eraseBlock(POSITION);
inline void moveBlock(POSITION, POSITION);

void move(POSITION *, short int);

void getKeyMasks(short int *, short int);
void delay(void);
short int quit(short int);

short int isWall(POSITION);
void drawMap(void);

void _main(void);

#endif
