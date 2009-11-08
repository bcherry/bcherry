// C Source File
// Created 22/Oct/03; 15:25:34

#define USE_TI89              // Compile for TI-89

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files
#include "moid.h"

void getKeyMasks(short int *keys) {
		// find the key masks
		keys[0] = 0x0001;	// bit 0
		keys[1] = 0x0004;	// bit 2
		keys[2] = 0x0002;	// bit 1
		keys[3] = 0x0008;	// bit 3
}

inline void drawBlock(POSITION pos) {
		Sprite8(pos.x*BLK_WIDTH,pos.y*BLK_HT,BLK_HT,lightBlk,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
		Sprite8(pos.x*BLK_WIDTH,pos.y*BLK_HT,BLK_HT,midBlk,GrayGetPlane(DARK_PLANE),SPRT_XOR);
		Sprite8(pos.x*BLK_WIDTH,pos.y*BLK_HT,BLK_HT,darkBlk,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
		Sprite8(pos.x*BLK_WIDTH,pos.y*BLK_HT,BLK_HT,darkBlk,GrayGetPlane(DARK_PLANE),SPRT_XOR);
}

inline void drawMet(POSITION pos) {
		Sprite16(pos.x*BLK_WIDTH,pos.y*BLK_HT,MET_HT,metLight,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
		Sprite16(pos.x*BLK_WIDTH,pos.y*BLK_HT,MET_HT,metMid,GrayGetPlane(DARK_PLANE),SPRT_XOR);
		Sprite16(pos.x*BLK_WIDTH,pos.y*BLK_HT,MET_HT,metDark,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
		Sprite16(pos.x*BLK_WIDTH,pos.y*BLK_HT,MET_HT,metDark,GrayGetPlane(DARK_PLANE),SPRT_XOR);
}

inline void eraseMet(POSITION pos) {
		drawMet(pos);
}

inline void drawFruit(POSITION pos) {
		Sprite16(pos.x*BLK_WIDTH,pos.y*BLK_HT,FRT_HT,frtLight,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
		Sprite16(pos.x*BLK_WIDTH,pos.y*BLK_HT,FRT_HT,frtMid,GrayGetPlane(DARK_PLANE),SPRT_XOR);
		Sprite16(pos.x*BLK_WIDTH,pos.y*BLK_HT,FRT_HT,frtDark,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
		Sprite16(pos.x*BLK_WIDTH,pos.y*BLK_HT,FRT_HT,frtDark,GrayGetPlane(DARK_PLANE),SPRT_XOR);
}

inline void eraseFruit(POSITION pos) {
	drawFruit(pos);
}

inline void moveFruit(POSITION oldPosition, POSITION newPosition) {
	eraseFruit(oldPosition);
	drawFruit(newPosition);
}

inline void moveMet(POSITION oldPosition, POSITION newPosition) {
		eraseMet(oldPosition);
		drawMet(newPosition);
}

/*short int win(POSITION metPos, POSITION frtPos) {
	if (metPos.x == frtPos.x && metPos.y == frtPos.y) {
		return 1;
	}
	
	return 0;
}*/

short int quit(void) {
	if (_rowread(TI89_ESCROW) & TI89_ESCKEY) {
		return 1;
	}	
	return 0;
}

inline void won(int mapnum) {
	drawMap(mapnum);	
}

void finish(int moves) {
	clrscr();
	drawMap(0);
	
	GraySetAMSPlane(DARK_PLANE);
	printf_xy(20,50,"You finished the game");
	printf_xy(25,60,"in %d moves!",moves);
	printf_xy(20,70,"ENTER - Play Again");
	printf_xy(20,80,"ESC - Quit");
	
	GraySetAMSPlane(LIGHT_PLANE);
	printf_xy(20,50,"You finished the game");
	printf_xy(25,60,"in %d moves!",moves);
	printf_xy(20,70,"ENTER - Play Again");
	printf_xy(20,80,"ESC - Quit");
	
	int k = 0;
	
	while (k != KEY_ENTER && k != KEY_ESC) {
		k = ngetchx();	
	}
	if (k == KEY_ENTER) {
		GrayOff();
		_main();
	}
}

void moveFrt(POSITION *frtPos, int mapnum) {
			
		 short int direction = random(4); 
		 
		 //POSITION pos = *frtPos;
		 POSITION oldPosition = *frtPos;
		 
		if (direction == 0) {
			// If moving up is possible, do it
			if (!isWall((POSITION){frtPos->x,frtPos->y-1}, mapnum) && !isWall((POSITION){frtPos->x+1,frtPos->y-1}, mapnum)) {
				frtPos->y--;
				moveFruit(oldPosition,*frtPos);
			}
			
		}
			
		if (direction == 1) {
			// If moving down is possible, do it
			if (!isWall((POSITION){frtPos->x,frtPos->y+2}, mapnum) && !isWall((POSITION){frtPos->x+1,frtPos->y+2}, mapnum)) {
				frtPos->y++;
				moveFruit(oldPosition,*frtPos);
			}
			
		}
		
		if (direction == 2) {	
			// If moving left is possible then do it
			if (!isWall((POSITION){frtPos->x-1,frtPos->y}, mapnum) && !isWall((POSITION){frtPos->x-1,frtPos->y+1}, mapnum)) {
				frtPos->x--;
				moveFruit(oldPosition,*frtPos);
			}		
			
		}
		
		if (direction == 3) {
			// If moving right is ok, do it
			if (!isWall((POSITION){frtPos->x+2,frtPos->y}, mapnum) && !isWall((POSITION){frtPos->x+2,frtPos->y+1}, mapnum)) {
				frtPos->x++;
				moveFruit(oldPosition,*frtPos);
			}
						
		}
}
void move(POSITION *pos, short int direction, POSITION *frtPos, int mapnum) {
	POSITION oldPosition = *pos;
	
	//*moves = moves + 1;
	
	moveFrt(frtPos, mapnum);
	
	switch (direction) {
		case UP:
			// If moving up is possible, do it
			if (!isWall((POSITION){pos->x,pos->y-1}, mapnum) && !isWall((POSITION){pos->x+1,pos->y-1}, mapnum)) {
				pos->y--;
				moveMet(oldPosition,*pos);
			}
			break;
			
		case DOWN:
			// If moving down is possible, do it
			if (!isWall((POSITION){pos->x,pos->y+2}, mapnum) && !isWall((POSITION){pos->x+1,pos->y+2}, mapnum)) {
				pos->y++;
				moveMet(oldPosition,*pos);
			}
			break;
			
		case LEFT:
			// If moving left is possible then do it
			if (!isWall((POSITION){pos->x-1,pos->y}, mapnum) && !isWall((POSITION){pos->x-1,pos->y+1}, mapnum)) {
				pos->x--;
				moveMet(oldPosition,*pos);
			}
			break;
		
		case RIGHT:
			// If moving right is ok, do it
			if (!isWall((POSITION){pos->x+2,pos->y}, mapnum) && !isWall((POSITION){pos->x+2,pos->y+1}, mapnum)) {
				pos->x++;
				moveMet(oldPosition,*pos);
			}
			break;
			
	}
}

// a function to slow down the prog due to _rowread
void delay(void) {
	short int loop = 1800, randNum;
	
	// generate random numbers to slow down the program...
	while (loop-- > 0) {
		randNum = rand() % loop;
	}
}

short int isWall(POSITION pos, int mapnum) {

	unsigned long int mask = 0x80000 >> pos.x, position = map[pos.y];

	if (mapnum == 1) {
		mask = 0x80000 >> pos.x, position = map1[pos.y];
	} else if (mapnum == 2) {
		mask = 0x80000 >> pos.x, position = map2[pos.y];
	} else if (mapnum == 3) {
		mask = 0x80000 >> pos.x, position = map3[pos.y];
	} else if (mapnum == 4) {
		mask = 0x80000 >> pos.x, position = map4[pos.y];
	} else if (mapnum == 5) {
		mask = 0x80000 >> pos.x, position = map5[pos.y];
	}
	
	// if mask and position collide, then its a wall
	if (mask & position) {
		return 1;
	}
	
	// otherwise, no wall
	return 0;
}

void drawTitle(void) {
	drawMap(0);
	int i = 1;
	POSITION pos = {4,7};
	for (i = 1; i < 5; i++) {
		pos.x = (4*i - 1);
		drawMet(pos);
	}
	GraySetAMSPlane(LIGHT_PLANE);
	printf_xy(70,80,"by Ben Cherry");
	
	GraySetAMSPlane(DARK_PLANE);
	printf_xy(70,80,"by Ben Cherry");
	
	while (ngetchx() != KEY_ENTER){}
}

void drawMap(int mapnum) {
	short int row, column;
	unsigned long int mask, position;
	
	// clear the screen
	ClrScr();
	GraySetAMSPlane(LIGHT_PLANE);
	ClrScr();
	GraySetAMSPlane(DARK_PLANE);
	ClrScr();
	
	if (mapnum == 0) {
		// loop through the map array
		for (row=0; row < MAP_ROWS; row++) {
			//get positions
			position = title[row];
		
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
	}	else if (mapnum == 1) {
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
	} else if (mapnum == 2) {
			// loop through the map array
			for (row=0; row < MAP_ROWS; row++) {
				//get positions
				position = map2[row];
		
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
	} else if (mapnum == 3) {
			// loop through the map array
			for (row=0; row < MAP_ROWS; row++) {
				//get positions
				position = map3[row];
			
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
		}else if (mapnum == 4) {
			// loop through the map array
			for (row=0; row < MAP_ROWS; row++) {
				//get positions
				position = map4[row];
			
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
		}else if (mapnum == 5) {
			// loop through the map array
			for (row=0; row < MAP_ROWS; row++) {
				//get positions
				position = map5[row];
			
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
	short int keys[4];
	short int key;
	INT_HANDLER interrupt1 = GetIntVec(AUTO_INT_1);  // this will save auto int 1
	short int win = 0;
	short int done = 0;
	short int moves = 0;
	short int mapnum = 1;
	int fin = 0;
	
	
	
	
	
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
	
	// turn on the grayscale routine
	GrayOn();
	
	drawTitle();
	
	GrayOff();
	// DESTROY auto-interrupt 1 so as not to mess up _rowread
	SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
	
	GrayOn();
	
	drawMap(mapnum);
	
	while (!done && !quit()) {
	
		POSITION pos = {1,1};
		POSITION frtPos = {17,9};
	
		// draw our metroid
		drawMet(pos);
	
		// draw the luscious fruit
		drawFruit(frtPos);	
	
		// until ESC is pressed...
		while (!quit()) {
			key = _rowread(ARROW_ROW);
		
			if (pos.x == frtPos.x && pos.y == frtPos.y) {
				win = 1;
				mapnum = mapnum + 1;
				break;
			}
			// check the UP arrow
			if (key & keys[UP]) {
				moves = moves + 1;
				move(&pos,UP,&frtPos, mapnum);
			}
		
			// check LEFT
			if (key & keys[LEFT]) {
				moves = moves + 1;
				move(&pos,LEFT,&frtPos, mapnum);
			}
		
			// check for DOWN
			if (key & keys[DOWN]) {
				moves = moves + 1;
				move(&pos,DOWN,&frtPos, mapnum);
			}
		
			// Check RIGHT
			if (key & keys[RIGHT]) {
				moves = moves + 1;
				move(&pos,RIGHT,&frtPos, mapnum);
			}
		
			// of course then SLOOOOOW the prog
			delay();
		}
	
		if (win) {
			
			if (mapnum <= LEVEL_NUM) {		
				won(mapnum);		
			} else {
				fin = 1;
				break;
			}
		} 
	}
	
	// ending the prog because ESC was pressed
	// restore A-I 1
	// turn off grayscale
	GrayOff();
	SetIntVec(AUTO_INT_1,interrupt1);
	GrayOn();
	
	if (fin) {
		finish(moves);
	}
	
	GrayOff();
	
	// wait for another keypress before quitting
	//ngetchx();
	
	
	
}
