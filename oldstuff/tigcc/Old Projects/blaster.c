// C Source File
// Created 25/Oct/03; 20:58:22
#define OPTIMIZE_ROM_CALLS // Use ROM Call Optimization

#define SAVE_SCREEN // Save/Restore LCD Contents

#include <tigcclib.h> // Include All Header Files

#include "blaster.h"
#include "extgraph.h"


short _ti89; // Produce .89Z File

short _ti92plus; // Produce .9XZ File

// a function that stores the key masks for use with _rowread
void getKeyMasks(short int *keys) {
		// find the key masks
		keys[0] = 0x0001;	// bit 0	Up
		keys[1] = 0x0004;	// bit 2	Down
		keys[2] = 0x0002;	// bit 1	Left
		keys[3] = 0x0008;	// bit 3	Right
		keys[4] = 0x0010; // bit 4	2nd
		keys[5] = 0x0020; // bit 5	Shift
		keys[6] = 0x0040; // bit 6	Diamond
		keys[7] = 0x0080; // bit 7	Alpha
}

// a function that draws the cannon on the screen
inline void drawLaser(POSITION pos, int map_x_location) {
	Sprite8((pos.x-map_x_location)*8,pos.y*8,LASER_HT,laserLight,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
	Sprite8((pos.x-map_x_location)*8,pos.y*8,LASER_HT,laserDark,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
	Sprite8((pos.x-map_x_location)*8,pos.y*8,LASER_HT,laserMid,GrayGetPlane(DARK_PLANE),SPRT_XOR);
	Sprite8((pos.x-map_x_location)*8,pos.y*8,LASER_HT,laserDark,GrayGetPlane(DARK_PLANE),SPRT_XOR);
}

// a function that erases the cannon, by redrawing it
inline void eraseLaser(POSITION pos, int map_x_location) {
	drawLaser(pos, map_x_location);
}

// a function that moves the cannon, by erasing and redrawing
inline void moveLaser(POSITION oldPos, POSITION newPos, int map_x_location) {
	eraseLaser(oldPos, map_x_location);
	drawLaser(newPos, map_x_location);
}

// same stuff for the other sprites
inline void drawShip(POSITION pos, int map_x_location) {
	Sprite16((pos.x-map_x_location)*8,pos.y*8,SHIP_HT,ship_light,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
	Sprite16((pos.x-map_x_location)*8,pos.y*8,SHIP_HT,ship_dark,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
	Sprite16((pos.x-map_x_location)*8,pos.y*8,SHIP_HT,ship_mid,GrayGetPlane(DARK_PLANE),SPRT_XOR);
	Sprite16((pos.x-map_x_location)*8,pos.y*8,SHIP_HT,ship_dark,GrayGetPlane(DARK_PLANE),SPRT_XOR);
}

inline void eraseShip(POSITION pos, int map_x_location) {
	drawShip(pos, map_x_location);
}

inline void moveShip(POSITION oldPos, POSITION newPos, int map_x_location) {
	eraseShip(oldPos, map_x_location);
	drawShip(newPos, map_x_location);
}

inline void drawMissile(POSITION pos, int map_x_location) {
	Sprite8((pos.x-map_x_location)*8,pos.y*8,MISSILE_HT,missileLight,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
	Sprite8((pos.x-map_x_location)*8,pos.y*8,MISSILE_HT,missileMid,GrayGetPlane(DARK_PLANE),SPRT_XOR);
}

inline void eraseMissile(POSITION pos, int map_x_location) {
	drawMissile(pos,map_x_location);
}

inline void moveMissile(POSITION oldPos, POSITION newPos, int map_x_location) {
	eraseMissile(oldPos,map_x_location);
	drawMissile(newPos,map_x_location);
}

inline void drawUpMissile(POSITION pos, int map_x_location) {
	Sprite8((pos.x-map_x_location)*8,pos.y*8,MISSILE_HT,missileUpLight,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
	Sprite8((pos.x-map_x_location)*8,pos.y*8,MISSILE_HT,missileUpMid,GrayGetPlane(DARK_PLANE),SPRT_XOR);
}

inline void eraseUpMissile(POSITION pos, int map_x_location) {
	drawUpMissile(pos,map_x_location);
}

inline void moveUpMissile(POSITION oldPos, POSITION newPos, int map_x_location) {
	eraseUpMissile(oldPos,map_x_location);
	drawUpMissile(newPos,map_x_location);
}

//  function that tests the presence of a block to be destroyed
short int blowWall(POSITION pos, char map_array[12][MAP_SIZE]) {
	if (map_array[pos.y][pos.x] != 0) return 1;
	return 0;
}

// a function that tests the prescence of a wall to be hit by the ship
short int detectWall(POSITION pos, char map_array[12][MAP_SIZE]) {
	if (_rowread(~((short)(1<<1))) & (1<<7) && _rowread(~((short)(1<<2))) & (1<<7)) return 0;
	if (map_array[pos.y][pos.x] == 0 && map_array[pos.y][pos.x+1] == 0) return 0;
	return 1;
}

/*short Draw_Map(int map_ptr, char map[12][MAP_SIZE], POSITION pos. POSITION laserPos, int laser, POSITION missilePos, int missile) {
	static int offset = 0;//offset variable
	//static int map_ptr = 0;//pointer to the column
	int x = map_ptr, y = 0;
		while (x < (map_ptr + 20)) {// Again, we can't view more than 20 blocks.. so.. don't calculate more than 20 blocks 
		if ((map_array[y][x] == 1)) { // If the array at this location is equal to 1, place a broken piece
			Sprite8((x - map_ptr) * 8, y * 8, 8, broken2_dark, GetPlane (DARK_PLANE), SPRT_XOR);
			Sprite8((x - map_ptr) * 8, y * 8, 8, broken2_light, GetPlane (LIGHT_PLANE), SPRT_XOR);
		}
		if ((map_array[y][x] == 2)) { // If the array at this location is equal to 1, place a broken piece
			Sprite8((x - map_ptr) * 8, y * 8, 8, broken1_dark, GetPlane (DARK_PLANE), SPRT_XOR);
			Sprite8((x - map_ptr) * 8, y * 8, 8, broken1_light, GetPlane (LIGHT_PLANE), SPRT_XOR);
		}
		if ((map_array[y][x] == 3)) { // .. and if it's 2, place block1
			Sprite8((x - map_ptr) * 8, y * 8, 8, block1_dark, GetPlane (DARK_PLANE), SPRT_XOR);
			Sprite8((x - map_ptr) * 8, y * 8, 8, block1_light, GetPlane (LIGHT_PLANE), SPRT_XOR);
		}
		if ((map_array[y][x] == 4)) { // a 3 means the indestructible block
			Sprite8((x - map_ptr) * 8, y * 8, 8, solid1_light, GrayGetPlane(LIGHT_PLANE), SPRT_XOR);
			Sprite8((x - map_ptr) * 8, y * 8, 8, solid1_dark, GrayGetPlane(LIGHT_PLANE), SPRT_XOR);
			Sprite8((x - map_ptr) * 8, y * 8, 8, solid1_mid, GrayGetPlane(DARK_PLANE), SPRT_XOR);
			Sprite8((x - map_ptr) * 8, y * 8, 8, solid1_dark, GrayGetPlane(DARK_PLANE), SPRT_XOR);
		}
		y++; // Now.. we're increasing the Y value.. note that i'm drawing the world from LEFT to RIGHT
				// I've seen people who draw their world TOP to DOWN or otherwise, but i found LEFT to RIGHT
		 	 // is the best way to draw the world..
		if (y > 11) { // We've reached the limit.. restore the Y value, and move 1 block row forward..
			y = 0;
			x++;
		} 
}*/


// function that draws the screen
void Draw_Map(int map_x_location, char map_array[12][MAP_SIZE], POSITION pos, POSITION laserPos, int laser, POSITION missilePos, int missile) {
	int x = map_x_location;
	int y = 0;
	
	// clear both planes
	SetPlane (LIGHT_PLANE);
	ClrScr();
	SetPlane (DARK_PLANE);
	ClrScr();
	
	// draw ship, and missiles and cannons if they exist
	drawShip(pos, map_x_location);
	if (laser) drawLaser(laserPos, map_x_location);
	if (missile == 1) drawMissile(missilePos, map_x_location);
	if (missile == -1) drawUpMissile(missilePos, map_x_location);
	
	// Well.. the map_x_location will keep track of where we are on the X-plane on the map array
	// This way we mask out the stuff in the array that we use.. So we will only read 20x12 elements of the
	// array (instead of MAP_SIZEx12 which it is at the moment.. )
	while (x < (map_x_location + 20)) {// Again, we can't view more than 20 blocks.. so.. don't calculate more than 20 blocks 
		if ((map_array[y][x] == 1)) { // If the array at this location is equal to 1, place a broken piece
			Sprite8((x - map_x_location) * 8, y * 8, 8, broken2_dark, GetPlane (DARK_PLANE), SPRT_XOR);
			Sprite8((x - map_x_location) * 8, y * 8, 8, broken2_light, GetPlane (LIGHT_PLANE), SPRT_XOR);
		}
		if ((map_array[y][x] == 2)) { // If the array at this location is equal to 1, place a broken piece
			Sprite8((x - map_x_location) * 8, y * 8, 8, broken1_dark, GetPlane (DARK_PLANE), SPRT_XOR);
			Sprite8((x - map_x_location) * 8, y * 8, 8, broken1_light, GetPlane (LIGHT_PLANE), SPRT_XOR);
		}
		if ((map_array[y][x] == 3)) { // .. and if it's 2, place block1
			Sprite8((x - map_x_location) * 8, y * 8, 8, block1_dark, GetPlane (DARK_PLANE), SPRT_XOR);
			Sprite8((x - map_x_location) * 8, y * 8, 8, block1_light, GetPlane (LIGHT_PLANE), SPRT_XOR);
		}
		if ((map_array[y][x] == 4)) { // a 3 means the indestructible block
			Sprite8((x - map_x_location) * 8, y * 8, 8, solid1_light, GrayGetPlane(LIGHT_PLANE), SPRT_XOR);
			Sprite8((x - map_x_location) * 8, y * 8, 8, solid1_dark, GrayGetPlane(LIGHT_PLANE), SPRT_XOR);
			Sprite8((x - map_x_location) * 8, y * 8, 8, solid1_mid, GrayGetPlane(DARK_PLANE), SPRT_XOR);
			Sprite8((x - map_x_location) * 8, y * 8, 8, solid1_dark, GrayGetPlane(DARK_PLANE), SPRT_XOR);
		}
		y++; // Now.. we're increasing the Y value.. note that i'm drawing the world from LEFT to RIGHT
				// I've seen people who draw their world TOP to DOWN or otherwise, but i found LEFT to RIGHT
		 	 // is the best way to draw the world..
		if (y > 11) { // We've reached the limit.. restore the Y value, and move 1 block row forward..
			y = 0;
			x++;
		} 
	}
}



// function that tests the escape key
short quit(void) {
	if (_rowread(TI89_ESCROW) & TI89_ESCKEY) {
		return 1;
	}
	
	return 0;
}

// a function to slow down the prog due to _rowread
void delay(void) {
	short int loop = 2300, randNum;
	
	// generate random numbers to slow down the program...
	while (loop-- > 0) {
		randNum = rand() % loop;
	}
}

// function that randomizes the map
void randomMap(int difficulty, char map[12][MAP_SIZE], int level_num) {
	
	int i, ii, block_type, block1, block2, block3, block4;
	for (i = 10; i <= MAP_SIZE - 21; i++) {
		block1 = random(11);
		block2 = random(11);
		block3 = random(11);
		block4 = random(11);
		if (difficulty > HARD) block1 = block2;
		if (difficulty >= EASY) block3 = block2; 
		for (ii = 0; ii <= 11; ii++) {
			if (ii != block1 && ii != block2 && ii!= block3 && ii!= block4){
				map[ii][i] = 0;
			}	else {
				block_type = random(10);
				if (block_type >= difficulty + 5 - level_num) map[ii][i] = 4;
				else map[ii][i] = 3;
			}
		}
	}
	for (i = 0; i <= 9; i++) {
		for (ii = 0; ii <= 11; ii++) {
			map[ii][i] = 0;
		}
	}
	for (i = MAP_SIZE - 20; i <= MAP_SIZE - 1; i++) {
		for (ii = 0; ii <= 11; ii++) {
			map[ii][i] = 0;
		}
	}  
}

// function that draws the title screen, in one or both planes
void drawTitle(int planes) {
		int x,y;
    int tilewidth  = 32;
    int tileheight = 32;
    int vcount     = 3;
    int hcount     = 5;
    int i;
    memset(GetPlane(0),0,LCD_SIZE);
    memset(GetPlane(1),0,LCD_SIZE);
    for (y=0;y<vcount;y++) {
        for (x=0;x<hcount;x++) { 
            int offset = (y*hcount+x)*tileheight;
            for (i=0;i<planes;i++) {
                Sprite32(x*tilewidth,y*tileheight,tileheight,&title[i][offset],GetPlane(i),SPRT_OR);
            }
        }
    }
}

// function to draw the pointer
inline void drawPointer(POSITION pos) {
	Sprite8(pos.x,pos.y*15 + 20,8,pointer_sprite,GrayGetPlane(DARK_PLANE),SPRT_XOR);
}

// function to draw the main menu
void drawWords(unsigned int difficulty) {
	char * diffStr;
	if (difficulty == VERY_EASY) diffStr = (char *)"Very Easy";
	if (difficulty == EASY) diffStr = (char *)"Easy";
	if (difficulty == NORMAL) diffStr = (char *)"Normal";	
	if (difficulty == HARD) diffStr = (char *)"Hard";
	if (difficulty == VERY_HARD) diffStr = (char *)"Very Hard";
	if (difficulty == IMPOSSIBLE) diffStr = (char *)"Impossible!";
	FontSetSys(F_8x10);
	GraySetAMSPlane(LIGHT_PLANE);
	DrawStr(20,20,"Play", A_NORMAL);
	DrawStr(20,35,"Diff -", A_NORMAL);
	DrawStr(80,35,diffStr, A_NORMAL);
	DrawStr(20,50,"High Scores", A_NORMAL);
	DrawStr(20,65,"Help", A_NORMAL);
	DrawStr(20,80,"Exit",A_NORMAL);
	GraySetAMSPlane(DARK_PLANE);
	DrawStr(20,20,"Play", A_NORMAL);
	DrawStr(20,35,"Diff -", A_NORMAL);
	DrawStr(80,35,diffStr, A_NORMAL);
	DrawStr(20,50,"High Scores", A_NORMAL);
	DrawStr(20,65,"Help", A_NORMAL);
	DrawStr(20,80,"Exit",A_NORMAL);
	FontSetSys(F_6x8);
}

// function to let the user type in a name for the hi-scores
void getName(unsigned long int score) {
	int key = 0;
	int x = 0;
	int y = 45;
	int back = 0;
	clrscr();
	GrayOff();
	FontSetSys(F_8x10);	
	printf_xy(20,0,"New High Score!");
	printf_xy(60,12,"%lu", score);
	FontSetSys(F_4x6);
	printf_xy(53,95,"Jet Blaster v 1.0 by Ben Cherry");
	FontSetSys(F_6x8);
	printf_xy(11,25,"Please Enter Your Name:");
	while (key != KEY_ENTER) {
		key = ngetchx();
		if (key == KEY_BACKSPACE) {
			if (x>0) x--;
			key = (int)' ';
			back = 1;
		}
		if (x <= 9 && key != KEY_ENTER) {
			printf_xy(6*x+50,y,"%c",(char)key);
			hiScores[0].name[x] = key;
		}
		if (!back && x <= 9) x++;
		if (back) back--;
	}
	clrscr();
}

// prints the hi-score table
void printHiScores(void) {
	int i = 0;
	GraySetAMSPlane(DARK_PLANE);
	clrscr();
	GraySetAMSPlane(LIGHT_PLANE);
	clrscr();
	GraySetAMSPlane(DARK_PLANE);
	FontSetSys(F_8x10);
	DrawStr(36,0,"Jet Blaster",A_NORMAL);
	DrawStr(36,12,"High Scores",A_NORMAL);
	FontSetSys(F_4x6);
	DrawStr(53,95,"Jet Blaster v 1.0 by Ben Cherry",A_NORMAL);
	FontSetSys(F_6x8);
	for (i = 1; i <= 5; i++) {
		printf_xy(10,i*9 + 26,"%d. %s - %lu",i,hiScores[i].name,hiScores[i].score);
	}
	GraySetAMSPlane(LIGHT_PLANE);
	FontSetSys(F_8x10);
	printf_xy(36,0,"Jet Blaster");
	printf_xy(36,12,"High Scores");
	FontSetSys(F_4x6);
	printf_xy(53,95,"Jet Blaster v 1.0 by Ben Cherry");
	FontSetSys(F_6x8);
	for (i = 1; i <= 5; i++) {
		printf_xy(10,i*9 + 26,"%d. %s - %lu",i,hiScores[i].name,hiScores[i].score);
	}
	while (ngetchx() != KEY_ENTER);
	GraySetAMSPlane(DARK_PLANE);
	clrscr();
	GraySetAMSPlane(LIGHT_PLANE);
	clrscr();
}

// computes the player's score
unsigned long int scoreCompute(int score, int difficulty, int level_num) {
	return (unsigned long)((((score / difficulty) * 100) + ((score>=difficulty) * difficulty * 100) + ((score<difficulty) * score * 10)) * level_num);
}

// tests the score for a new hi-score
void hiScoresGo(unsigned long int score, unsigned int difficulty, unsigned int level_num) {
	clrscr();
	hiScores[0].score = scoreCompute(score, difficulty, level_num);
	if (hiScores[0].score > hiScores[5].score){
		getName(hiScores[0].score);
		hiScores[5] = hiScores[0];
		if (hiScores[0].score > hiScores[4].score) {
			hiScores[5] = hiScores[4];
			hiScores[4] = hiScores[0];
			if (hiScores[0].score > hiScores[3].score) {
				hiScores[4] = hiScores[3];
				hiScores[3] = hiScores[0];
				if (hiScores[0].score > hiScores[2].score) {
					hiScores[3] = hiScores[2];
					hiScores[2] = hiScores[0];
					if (hiScores[0].score > hiScores[1].score) {
						hiScores[2] = hiScores[1];
						hiScores[1] = hiScores[0];
					}
				}
			}
		}
	}	else {
		printf("You failed to achieve\na new High Score\nYour Score was:\n%lu", hiScores[0].score);
		while (ngetchx()!= KEY_ENTER);
		clrscr();
	}
	printHiScores();
}

// loads the next level when you finish one
void won(int difficulty, char map[12][MAP_SIZE], int level_num) {
	randomMap(difficulty, map, level_num);
	Draw_Map(0, map, (POSITION){0,0}, (POSITION){0,0}, 0, (POSITION){0,0}, 0);
}

// draws the shop screen
void shop(unsigned int *money, int *cannon_level, int *missile_level) {
	int key = 0;
	while (key != KEY_ENTER) {
		clrscr();
		printf("You have $%d\n", *money);
		if (*cannon_level < 4) printf("Your cannons are level %d\n", *cannon_level);
		if (*cannon_level == 1) printf("Upgrade cannon - $%d\n", CANNON_UPGRADE_2);
		if (*cannon_level == 2) printf("Upgrade cannon - $%d\n", CANNON_UPGRADE_3);
		if (*cannon_level == 3) printf("Upgrade cannon - $%d\n", CANNON_UPGRADE_4);
		if (*missile_level == 0 || *missile_level == 2) printf("Down missiles  - $%d\n", MISSILES_COST);
		if (*missile_level == 0 || *missile_level == 1) printf("Up Missiles    - $%d\n", MISSILES_COST);
		if (*cannon_level < 4 ) printf("Press F1 to buy cannon\n");
		if (*missile_level < 3) printf("Up/down to buy missile\n");
		if (*cannon_level >= 4 && *missile_level >= 3) printf("You have it all!!\n");
		printf("Press enter to leave\n");
		key = ngetchx();
		if (key == KEY_F1 && *cannon_level == 1 && *money >= CANNON_UPGRADE_2) {
		 *cannon_level+=1;
		 *money -= CANNON_UPGRADE_2;
		} else if (key == KEY_F1 && *cannon_level == 2 && *money >= CANNON_UPGRADE_3) {
			*cannon_level+=1;
			*money -= CANNON_UPGRADE_3;
		} else if (key == KEY_F1 && *cannon_level == 3 && *money >= CANNON_UPGRADE_4) {
			*cannon_level+=1;
			*money -= CANNON_UPGRADE_4;
		}
		if (key == KEY_DOWN && *money >= MISSILES_COST) {
			if (*missile_level == 0 || *missile_level == 2) {
			*missile_level+=1;
			*money -= MISSILES_COST;
			}
		} else if (key == KEY_UP && *money >= MISSILES_COST) {
			if (*missile_level == 0 || *missile_level == 1) {
				*missile_level+=2;
				*money -= MISSILES_COST;
			}
		}
		//if (key == KEY_F5) *money+=1000;
	}
	GKeyFlush();
}

// draws the help screen
void doHelp(void) {
	drawTitle(1);
	GraySetAMSPlane(LIGHT_PLANE);
	clrscr();
	printf("Jet Blaster Controls\n\nArrow keys - Move Ship\nDiamond, Shift - Missiles\nSecond - Cannon");
	GraySetAMSPlane(DARK_PLANE);
	clrscr();
	printf("Jet Blaster Controls\n\nArrow keys - Move Ship\nDiamond, Shift - Missiles\nSecond - Cannon");
	while (ngetchx()!=KEY_ENTER);
	GraySetAMSPlane(LIGHT_PLANE);
	clrscr();
	GraySetAMSPlane(DARK_PLANE);
	clrscr();
}

// the main function
void _main(void) {	
	unsigned int difficulty = NORMAL;
	short int key=0;	
	short int keys[8];	
	unsigned int level_num = 1;
	unsigned short int score = 0;
	int done = 0;
	unsigned int money = 0;
	int cannon_level = 1;
	int missile_level = 0;	
	char map[12][MAP_SIZE];
	
	// seed the random numbers
	randomize();
	// get the key masks	
	getKeyMasks(keys);
	
	
  INT_HANDLER interrupt1 = GetIntVec(AUTO_INT_1);  // this will save auto int 1

	// draw title screen and wait for keypress
	GrayOn();
	drawTitle(2);
	ngetchx();
	
	//draw background title screen and menu
  drawTitle(1);
  drawWords(difficulty);
  POSITION pointer = {10,0};
  drawPointer(pointer);
  // the menu loop
  while (1) {
  	key = ngetchx();
  	if (key == KEY_ENTER && pointer.y != OPTIONS) {
  		if (pointer.y == PLAY) break;
  		if (pointer.y == HIGH_SCORES) printHiScores();
  		if (pointer.y == HELP) doHelp();
  		if (pointer.y == ABOUT) {SetIntVec(AUTO_INT_1,interrupt1);GrayOff();exit(0);}
  		GraySetAMSPlane(LIGHT_PLANE);clrscr();GraySetAMSPlane(DARK_PLANE);clrscr();
  		drawTitle(1); drawWords(difficulty); pointer=(POSITION){10,0}; drawPointer(pointer);
  	}
  	if (key == KEY_LEFT && pointer.y == OPTIONS && difficulty < VERY_EASY) {
  		difficulty+=1;GraySetAMSPlane(LIGHT_PLANE);clrscr();GraySetAMSPlane(DARK_PLANE);clrscr();
  		drawTitle(1);drawPointer(pointer);drawWords(difficulty);}
  	if (key == KEY_RIGHT && pointer.y == OPTIONS && difficulty > IMPOSSIBLE)  {
  		difficulty-=1;GraySetAMSPlane(LIGHT_PLANE);clrscr();GraySetAMSPlane(DARK_PLANE);clrscr();
  		drawTitle(1);drawPointer(pointer);drawWords(difficulty);}
  	if (key == KEY_UP || key == KEY_DOWN) drawPointer(pointer);
  	if (key == KEY_UP && pointer.y > 0) pointer.y--;
  	if (key == KEY_DOWN && pointer.y < 4) pointer.y++;
  	if (key == KEY_UP || key == KEY_DOWN) drawPointer(pointer);
  }
  key = 0;
  // turn off gray, so we can destroy auto-int-1 and not mess it up
  GrayOff();  
	
	// DESTROY auto-interrupt 1 so as not to mess up _rowread
	SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
	
	// turn gray back on
	GrayOn();
	
	// randomize the map
	randomMap(difficulty, map, level_num);
	//the main game loop
	while (!quit() && !done) {		
		done = 0;
		int fin = 0;
		int win = 0;
		int forward = 0;
		int map_x_location = 0;
		int laser = 0;
		int justLaser = 0;
		int missile = 0;
		int justMissile = 0;
		int missileSlow = 2;
		POSITION laserPos;
		POSITION oldLaserPos;
		POSITION missilePos;
		POSITION oldMissilePos;
		POSITION oldShip;
		POSITION ShipPos = {0,5};
		
		// we need to disable gray temporarily to do the shop screen...
		GrayOff();
		SetIntVec(AUTO_INT_1, interrupt1);
		shop(&money, &cannon_level, &missile_level);
		SetIntVec(AUTO_INT_1, DUMMY_HANDLER);
		GrayOn();
		
		// draws the level
		Draw_Map(map_x_location, map, ShipPos, laserPos, laser, missilePos, missile);
	
	
		// the loop for the action in the level
		while (!quit()) {
		
			// if the user has a missile out, deal with it
			if (missile && missileSlow == 2) {
				missileSlow = 1;
				oldMissilePos = missilePos;
				missilePos.x++;
				missilePos.y+=missile;
				if (missilePos.x > map_x_location + 20) missile = 0;
				if (missilePos.y < 0 || missilePos.y > 12) {
					missile = 0;
					eraseMissile(missilePos, map_x_location);
				}
				if (blowWall(missilePos,map)) {
					if (map[missilePos.y][missilePos.x] < 4) {
						map[missilePos.y][missilePos.x]-=3;
						if (map[missilePos.y][missilePos.x] == 0) {
							score++;
							money+=BLOCK_VALUE;
						}
					}
					if (map[missilePos.y][missilePos.x] < 0) map[missilePos.y][missilePos.x] = 0;
					missile = 0;
					justMissile = 0;
					Draw_Map(map_x_location,map,ShipPos,laserPos,laser,missilePos,missile);
				}
				// if the shot was just fired, then there wont be one to erase
				if (!justMissile && missile == 1) {
					moveMissile(oldMissilePos,missilePos,map_x_location);
				} else if (missile == 1) {
					drawMissile(missilePos,map_x_location);
					justMissile = 0;
				} else if (!justMissile && missile == -1) {
					moveUpMissile(oldMissilePos, missilePos, map_x_location);
				} else if (missile == -1) {
					drawUpMissile(missilePos, map_x_location);
					justMissile = 0;
				}
					
			} else {
				missileSlow++;
			}
			
			// if the user has a laser shot that is still going, continue it
			if (laser) {
				oldLaserPos = laserPos;
				laserPos.x++;
				if (laserPos.x > map_x_location + 20) laser=0;
				if (blowWall(laserPos,map)) {
					if (map[laserPos.y][laserPos.x] < 4) {
						map[laserPos.y][laserPos.x]-=cannon_level;
						if (map[laserPos.y][laserPos.x] <= 0) {
							score++;
							money+=BLOCK_VALUE;
							map[laserPos.y][laserPos.x] = 0;
						}
					} else if (map[laserPos.y][laserPos.x] == 4 && cannon_level == 4) {
						map[laserPos.y][laserPos.x] = 1;
					}
					laser = 0;
					justLaser = 0;
					Draw_Map(map_x_location,map,ShipPos,laserPos,laser, missilePos, missile);				
				}
				// if the shot was just fired, then there wont be one to erase
				if (!justLaser && laser) {
					moveLaser(oldLaserPos,laserPos,map_x_location);
				} else if (laser){
					drawLaser(laserPos,map_x_location);
					justLaser = 0;
				}
			}	
		
			// scroll the screen forward one block every (difficulty) time through the main loop
			if (forward == difficulty) {
				map_x_location++;
				ShipPos.x++;
				forward = 0;
				if (map_x_location >= MAP_SIZE - 20) {
					win = 1;
					level_num++;
					break;
				}
				// if you ran into a wall, quit
				if (detectWall(ShipPos,map)) {
					win = 1;
					score /= 2 ;
					break;
				} 
				Draw_Map(map_x_location,map,ShipPos,laserPos,laser, missilePos, missile);
			} else {
				forward++;
			}
			
			// if you ran into a wall, quit
			if (detectWall(ShipPos,map)) {
				win = 1;
				score = 0;
				break;
			} 
			
			if (_rowread(~((short)(1<<1))) & (1<<6) && _rowread(~((short)(1<<2))) & (1<<6)) {
				win = 1;
				level_num++;
				break;
			}
			
			// get keypresses
			key = _rowread(ARROW_ROW);
			
			// if the user pressed right, move the ship right
			if (key & keys[RIGHT]) {
				oldShip = ShipPos;
				ShipPos.x++;
				if (ShipPos.x > map_x_location + 18) ShipPos.x--;
				if (!detectWall(ShipPos,map)){
					moveShip(oldShip,ShipPos,map_x_location);
				}	else {
					win = 1;
					score = 0;
					break;
				}
			}	
			// If the user pressed left, move the ship left			
			if (key & keys[LEFT]) {
				oldShip = ShipPos;
				ShipPos.x--;
				if (ShipPos.x < map_x_location) ShipPos.x++;
				if (!detectWall(ShipPos,map)){
					moveShip(oldShip,ShipPos,map_x_location);
				} else {
					win = 1;
					score = 0;
					break;
				}
			}
				
			// if the user pressed up, move the ship up
			if (key & keys[UP]) {
					oldShip = ShipPos;				
					if (ShipPos.y - 1 < 0) {
						ShipPos.y = 0;
					} else {
						ShipPos.y--;
					}
					if (!detectWall(ShipPos,map))	{		
						moveShip(oldShip,ShipPos,map_x_location);
					} else {
						win = 1;
						score = 0;
						break;
					}	
			}	
			
			// if the user pressed down, move the ship down
			if (key & keys[DOWN]) {
					oldShip = ShipPos;
					ShipPos.y++;
					if (ShipPos.y > 10) ShipPos.y = 10;
					if (!detectWall(ShipPos,map)) {
						moveShip(oldShip,ShipPos,map_x_location);
					} else {
						win = 1;
						score = 0;
						break;
					}
			}
				
			// if 2nd was pushed, fire the laser
			if (key & keys[SECOND]) {
				if (!laser) {
				justLaser = 1;
				laser = 1;
				laserPos.x = ShipPos.x + 1;
				laserPos.y = ShipPos.y;
				}	
			}
		
			// if diamond was pushed fire the downward missiles
			if (key & keys[DIAMOND]) {
				if (missile_level == 3 || missile_level == 1) {
					if (!missile) {
						justMissile = 1;
						missile = 1;
						missileSlow = 2;
						missilePos.x = ShipPos.x;
						missilePos.y = ShipPos.y;
					}
				}
			}
			
			// if shift was pushed fire the upward missiles
			if (key & keys[SHIFT]) {
				if (missile_level == 2 || missile_level == 3) {
					if (!missile) {
						justMissile = 1;
						missile = -1;
						missileSlow = 2;
						missilePos.x = ShipPos.x;
						missilePos.y = ShipPos.y;
					}
				}
			}
			
			/*if (key & keys[ALPHA]) {
			score += 10;
			}*/
			// slow down the program because _rowread is too fast
			delay();
		}
		
	
		
		// back to the overall game loop
		if (win) {
			if (level_num <= LEVEL_NUM) {
				won(difficulty, map, level_num);
			} else {
				fin = 1;
				break;
			}
		}	
	}
		
	// the user left, either by winning or quitting, so make sure everything is reset so the calc will be fine	
	GrayOff();
	SetIntVec(AUTO_INT_1,interrupt1);
	hiScoresGo(score, difficulty, level_num);
}
