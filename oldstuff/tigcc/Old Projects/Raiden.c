// C Source File
// Created 29/Oct/03; 16:58:15

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define MIN_AMS 100           // Compile for AMS 1.00 or higher

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files


/* Constants */
enum MapDimensions {MAP_ROWS = 12, MAP_COLUMNS = 20, LEVEL_NUM = 5};
enum BlockConstants {BLK_HT = 8, BLK_WIDTH = 8};
enum MetConstants {MET_HT = 16, MET_WIDTH = 16};
enum FrtConstants {FRT_HT = 16, FRT_WIDTH = 16};

enum ArrowKeys {UP,DOWN,LEFT,RIGHT};
enum KeyMatrix {TI89_ESCROW = ~0x0040, TI89_ESCKEY = 0x0001, ARROW_ROW = ~0x0001};

typedef struct {
	unsigned short int x : 8, y : 8;
} POSITION;


static unsigned long int map1[] = {0x00001,0x80000,0x00001,0x80000,0x00001,0x80000,
																	0x00001,0x80000,0x00001,0x80000,0x00001,0x80000};
																	
static unsigned char lightBlk[] = {0x00,0x18,0x00,0x42,0x42,0x00,0x18,0x00};
static unsigned char midBlk[]   = {0x00,0x24,0x5A,0x24,0x24,0x5A,0x24,0x00};
static unsigned char darkBlk[]  = {0xFF,0xC3,0xA5,0x99,0x99,0xA5,0xC3,0xFF};

inline void drawBlock(POSITION pos) {
		Sprite8(pos.x*BLK_WIDTH,pos.y*BLK_HT,BLK_HT,lightBlk,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
		Sprite8(pos.x*BLK_WIDTH,pos.y*BLK_HT,BLK_HT,midBlk,GrayGetPlane(DARK_PLANE),SPRT_XOR);
		Sprite8(pos.x*BLK_WIDTH,pos.y*BLK_HT,BLK_HT,darkBlk,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
		Sprite8(pos.x*BLK_WIDTH,pos.y*BLK_HT,BLK_HT,darkBlk,GrayGetPlane(DARK_PLANE),SPRT_XOR);
}

inline void eraseBlock(POSITION pos) {
	drawBlock(pos);
}

inline void moveBlock(POSITION oldPos, POSITION newPos) {
	eraseBlock(oldPos);
	drawBlock(newPos);
}

short int isWall(POSITION pos, int mapnum) {

	unsigned long int mask = 0x80000 >> pos.x, position = map1[pos.y];

	if (mapnum == 1) {
		mask = 0x80000 >> pos.x, position = map1[pos.y];
	}
	/*} else if (mapnum == 2) {
		mask = 0x80000 >> pos.x, position = map2[pos.y];
	} else if (mapnum == 3) {
		mask = 0x80000 >> pos.x, position = map3[pos.y];
	} else if (mapnum == 4) {
		mask = 0x80000 >> pos.x, position = map4[pos.y];
	} else if (mapnum == 5) {
		mask = 0x80000 >> pos.x, position = map5[pos.y];
	}*/
	
	// if mask and position collide, then its a wall
	if (mask & position) {
		return 1;
	}
	
	// otherwise, no wall
	return 0;
}

short int quit(void) {
	if (_rowread(TI89_ESCROW) & TI89_ESCKEY) {
		return 1;
	}
	
	return 0;
}

// a function to slow down the prog due to _rowread
void delay(void) {
	short int loop = 1500, randNum;
	
	// generate random numbers to slow down the program...
	while (loop-- > 0) {
		randNum = rand() % loop;
	}
}

void move(POSITION *pos, short int direction, int mapnum) {
	POSITION oldPosition = *pos;
	
	//*moves = moves + 1;
	
	//moveFrt(frtPos, mapnum);
	
	switch (direction) {
		case UP:
			// If moving up is possible, do it
			if (!isWall((POSITION){pos->x,pos->y-1}, mapnum) /*&& !isWall((POSITION){pos->x+1,pos->y-1}, mapnum)*/) {
				pos->y--;
				moveBlock(oldPosition,*pos);
			}
			break;
			
		case DOWN:
			// If moving down is possible, do it
			if (!isWall((POSITION){pos->x,pos->y+1}, mapnum) /*&& !isWall((POSITION){pos->x+1,pos->y+2}, mapnum)*/) {
				pos->y++;
				moveBlock(oldPosition,*pos);
			}
			break;
			
		case LEFT:
			// If moving left is possible then do it
			if (!isWall((POSITION){pos->x-1,pos->y}, mapnum) /*&& !isWall((POSITION){pos->x-1,pos->y+1}, mapnum)*/) {
				pos->x--;
				moveBlock(oldPosition,*pos);
			}
			break;
		
		case RIGHT:
			// If moving right is ok, do it
			if (!isWall((POSITION){pos->x+1,pos->y}, mapnum) /*&& !isWall((POSITION){pos->x+2,pos->y+1}, mapnum)*/) {
				pos->x++;
				moveBlock(oldPosition,*pos);
			}
			break;
			
	}
}



void getKeyMasks(short int *keys) {
		// find the key masks
		keys[0] = 0x0001;	// bit 0
		keys[1] = 0x0004;	// bit 2
		keys[2] = 0x0002;	// bit 1
		keys[3] = 0x0008;	// bit 3
}
																	
void moveMap(int mapNum, int dir, POSITION pos) {
	int i = 0;
	unsigned long int mask = 0x80000 >> pos.x;
	if (mapNum == 1) {
		
		for (i = 0; i <=11; i++) {
			if (dir % 2 == 1) {
				map1[i] <<= 1;
				if (map1[i] & mask) map1[i] >>= 1;
			} else {
				map1[i] >>= 1;
				if (map1[i] & mask) map1[i] <<= 1;
			}
			dir++;
		}
	}

} 



void drawMap(int mapNum, POSITION pos) {
	short int row, column;
	unsigned long int mask, position;
	
	// clear the screen
	ClrScr();
	GraySetAMSPlane(LIGHT_PLANE);
	ClrScr();
	GraySetAMSPlane(DARK_PLANE);
	ClrScr();
	
	drawBlock(pos);
	
	if (mapNum == 1) {
		// loop through the map array
		for (row=0; row < MAP_ROWS; row++) {
			//get positions
			position = map1[row];
		
			// reset the mask
			mask = 0x100000;
		
			// looop through the column to see where in the row we draw blocks
			for (column = 0; column < MAP_COLUMNS; column++) {
				//shift the mask
				mask >>= 1;
			
				// check position, and then draw a block if needed
				if (position & mask) {
					drawBlock((POSITION){column,row});
				}
			}
		}
	}	
}

// Main Function
void _main(void) {
	short int dir = 1;
	short int keys[4];
	short int key;
	int rounds = 0;
	int mapnum = 1;
	INT_HANDLER interrupt1 = GetIntVec(AUTO_INT_1);  // this will save auto int 1
	
	// seed the random numbers
	randomize();
	
	// get the key masks
	getKeyMasks(keys);
	
	
	
	/* clear the screen
	ClrScr();
	GraySetAMSPlane(LIGHT_PLANE);
	ClrScr();
	GraySetAMSPlane(DARK_PLANE);
	ClrScr();*/
	
	// DESTROY auto-interrupt 1 so as not to mess up _rowread
	SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
	
	GrayOn();
	
		POSITION pos = {1,1};
	
	while (!quit()) {
		
	
		
		
		
		key = _rowread(ARROW_ROW);
		
		if (key & keys[UP]) {
			move(&pos,UP,mapnum);
		}
		
		if (key & keys[LEFT]) {
			move(&pos,LEFT,mapnum);
		}
		
		if (key & keys[DOWN]) {
			move(&pos,DOWN,mapnum);
		}
		
		if (key & keys[RIGHT]) {
			move(&pos,RIGHT,mapnum);
		}
		moveMap(mapnum, dir, pos);
		drawMap(mapnum, pos);
		
		rounds++;
		if (rounds > 18) {
			rounds = 0;
			dir++;
		}
		// of course then SLOOOOOW the prog
			delay();
	}
	
		GrayOff();
		SetIntVec(AUTO_INT_1,interrupt1);
}
