// C Source File
// Created 27/Oct/03; 21:12:00

#define USE_TI89              // Compile for TI-89
//#define USE_TI92PLUS          // Compile for TI-92 Plus
//#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

//#define MIN_AMS 100           // Compile for AMS 1.00 or higher

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

#include "gradius.h" 					// include the header file

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

inline void drawLaser(POSITION pos) {
	Sprite8(pos.x*8,pos.y*8,LASER_HT,laserLight,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
	Sprite8(pos.x*8,pos.y*8,LASER_HT,laserDark,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
	Sprite8(pos.x*8,pos.y*8,LASER_HT,laserMid,GrayGetPlane(DARK_PLANE),SPRT_XOR);
	Sprite8(pos.x*8,pos.y*8,LASER_HT,laserDark,GrayGetPlane(DARK_PLANE),SPRT_XOR);
}

inline void eraseLaser(POSITION pos) {
	drawLaser(pos);
}

inline void moveLaser(POSITION oldPos, POSITION newPos) {
	eraseLaser(oldPos);
	drawLaser(newPos);
}

inline void drawRType(POSITION pos) {
	Sprite16(pos.x*8,pos.y*8,R_TYPE_HT,r_type_light,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
	Sprite16(pos.x*8,pos.y*8,R_TYPE_HT,r_type_dark,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
	Sprite16(pos.x*8,pos.y*8,R_TYPE_HT,r_type_mid,GrayGetPlane(DARK_PLANE),SPRT_XOR);
	Sprite16(pos.x*8,pos.y*8,R_TYPE_HT,r_type_dark,GrayGetPlane(DARK_PLANE),SPRT_XOR);
}

inline void eraseRType(POSITION pos) {
	drawRType(pos);
}

inline void moveRType(POSITION oldPos, POSITION newPos) {
	eraseRType(oldPos);
	drawRType(newPos);
}

inline void drawMissile(POSITION pos) {
	Sprite8(pos.x*8,pos.y*8,MISSILE_HT,missileLight,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
	//Sprite8((pos.x-map_x_location)*8,pos.y*8,MISSILE_HT,missileMid,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
	Sprite8(pos.x*8,pos.y*8,MISSILE_HT,missileMid,GrayGetPlane(DARK_PLANE),SPRT_XOR);
	//Sprite8((pos.x-map_x_location)*8,pos.y*8,MISSILE_HT,missileDark,GrayGetPlane(DARK_PLANE),SPRT_XOR);
}

inline void eraseMissile(POSITION pos) {
	drawMissile(pos);
}

inline void moveMissile(POSITION oldPos, POSITION newPos) {
	eraseMissile(oldPos);
	drawMissile(newPos);
}

short int blowWall(POSITION pos, int map_array[12][20]) {
	if (map_array[pos.y][pos.x] != 0) return 1;
	return 0;
}

short int detectWall(POSITION pos, int map_array[12][20]) {
	if (map_array[pos.y][pos.x] == 0 && map_array[pos.y][pos.x+1] == 0) return 0;
	return 1;
}

void Draw_Map(int map_array[12][20], POSITION pos, POSITION laserPos, int laser, POSITION missilePos, int missile) {
	//int x = map_x_location;
	int x = 0;
	int y = 0;
	

	SetPlane (LIGHT_PLANE);
	ClrScr ();

	SetPlane (DARK_PLANE);
	ClrScr ();
	
	drawRType(pos);
	if (laser) drawLaser(laserPos);
	if (missile) drawMissile(missilePos);
	
	// Well.. the map_x_location will keep track of where we are on the X-plane on the map array
	// This way we mask out the stuff in the array that we use.. So we will only read 30x8 elements of the
	// array (instead of 60x8 which it is at the moment.. ).. This might sound unuseful, and it sort of is
	// with a small array as this one.. but i'm guessing you will create worlds that are much much bigger..
	// so.. to gain more speed.. we mask it out ;)

	/*while (x<20) {// Again, we can't view more than 30 blocks.. so.. don't calculate more than 30 blocks 
		if ((map_array[y][x]==7)) { // If the array at this location is equal to 1, place a ground piece
			Sprite8(x*8, y*8, 8, ground_down, GetPlane (DARK_PLANE), SPRT_XOR);
			Sprite8(x*8, y*8, 8, ground_down, GetPlane (LIGHT_PLANE), SPRT_XOR);
		}
		if ((map_array[y][x]==8)) { // .. and if it's 2, place block1
			Sprite8(x*8, y*8, 8, ground_flat, GetPlane (DARK_PLANE), SPRT_XOR);
			Sprite8(x*8, y*8, 8, ground_flat, GetPlane (LIGHT_PLANE), SPRT_XOR);
		}
		if (map_array[y][x]==9) {
			Sprite8(x*8, y+8, 8, ground_up, GrayGetPlane(LIGHT_PLANE), SPRT_XOR);
			Sprite8(x*8, y+8, 8, ground_up, GrayGetPlane(DARK_PLANE), SPRT_XOR);
		}
		y++; // Now.. we're increasing the Y value.. note that i'm drawing the world from LEFT to RIGHT
				// I've seen people who draw their world TOP to DOWN or otherwise, but i found LEFT to RIGHT
		 	 // is the best way to draw the world..
		if (y>11) { // We've reached the limit.. restore the Y value, and move 1 block row forward..
			y=0;
			x++;
		} 
	}*/
	
	for (x = 0; x <= 19; x++) {
		for (y = 0; y <= 11; y++) {
			if (map_array[y][x] == 7) {
				Sprite8(x*8, y*8, 8, ground_down, GrayGetPlane(DARK_PLANE), SPRT_XOR);
				Sprite8(x*8, y*8, 8, ground_down, GrayGetPlane(LIGHT_PLANE), SPRT_XOR);
			}	else if (map_array[y][x] == 8) {
				Sprite8(x*8, y*8, 8, ground_flat, GrayGetPlane(DARK_PLANE), SPRT_XOR);
				Sprite8(x*8, y*8, 8, ground_flat, GrayGetPlane(LIGHT_PLANE), SPRT_XOR);
			} else if (map_array[y][x] == 9) {
				Sprite8(x*8, y*8, 8, ground_up, GrayGetPlane(DARK_PLANE), SPRT_XOR);
				Sprite8(x*8, y*8, 8, ground_up, GrayGetPlane(LIGHT_PLANE), SPRT_XOR);
			}
		}
	}	
}

short int quit(void) {
	if (_rowread(TI89_ESCROW) & TI89_ESCKEY) {
		return 1;
	}
	
	return 0;
}

// a function to slow down the prog due to _rowread
void delay(void) {
	short int loop = 1800, randNum;
	
	// generate random numbers to slow down the program...
	while (loop-- > 0) {
		randNum = rand() % loop;
	}
}

void randMap(int *map[12][20]) {
	//int map[12][20] = *map1; 
	int x = 0;
	int y = 0;
	int top_bott = 0;
	int top = 0;
	int bott = 0;
	int up_down = 0;
	int loop = 0;
	for (x = 0; x <= 19; x++) {
		for (y = 0; y <= 11; y++) {
			*map[x][y] = *map[x+1][y];
			if (x == 19 && *map[x][y] >= 7) {
				if (top_bott == 0) {
					top = y;
					top_bott++;
				} else if (top_bott == 1) bott = y;
			} 
		}
	}
	// does the top
	up_down = random(2) + 7;
	*map[19][top] = up_down;
	if (top != 0) {
		for (loop = 0; loop < top; loop++) {
			*map[19][loop] = 0;
		}
	}
	up_down = random(2) + 7;
	*map[19][bott] = up_down;
	if (bott != 19) {
		for (loop = bott + 1; loop <= 19; loop++) {
			*map[19][loop] = 0;
		}
	}
	for (loop = top + 1; loop <= bott - 1; loop++) {
		*map[19][loop] = 0;
	}
	//*map1 = map;
}

// Main Function
void _main(void) {
	// This is where the level map is defined
	// The map is 12 blocks of height 8 tall, and that fills the screen
	// 0s are blank, 1s are ground blocks, and 2s are the other blocks
	/*int map1[12][82] = { 
	{0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,0,0,0,2,2,0,2,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,2,2,2,0,0,0,2,2,2,2,2,2,2,0,0,0,2,2,0,2,0,2,0,2,0,0,0,0,0},
	{0,0,0,2,0,0,0,0,2,2,0,0,0,2,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,2,2,0,0,0,2,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,2,0,0,2,0,0,2,2,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,0,2,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,2,2,0,2,0,2,0,2,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,2,2,0,2,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,2,2,0,2,0,2,0,2,0,0,0,0,0},
	{0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,2,0,2,0,2,0,2,2,2,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,2,0,2,0,2,0,2,2,2,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,2,2,2,0,2,0,0,0,2,0,0,2,2,0,0,0,0,0,2,2,0,2,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,2,0,0,0,2,0,0,2,2,0,0,0,0,0,2,2,0,2,0,2,0,2,0,0,0,0,0},
	{0,0,0,2,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,2,0,0,0,2,0,2,2,2,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,0,0,0,0,2,0,0,0,2,0,2,2,2,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,2,2,2,0,2,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,2,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,0,2,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,2,0,2,0,0,0,0,0},
	{0,0,0,2,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,2,2,2,0,2,2,0,0,0,0,0,0,0,2,2,0,2,0,2,2,2,0,0,0,0,0,0,2,0,0,2,0,0,0,0,0,0,0,0,0,0,2,2,2,0,2,2,0,0,0,0,0,0,0,2,2,0,2,0,2,2,2,0,0,0},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}};*/
	
	int map1[12][20] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{8,7,0,0,0,9,7,9,7,0,0,0,0,0,9,8,8,8,8,8},
		{0,0,7,0,9,0,0,0,0,7,0,0,0,9,0,0,0,0,0,0},
		{0,0,0,8,0,0,0,0,0,0,7,0,9,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,8,0,0,0,0,9,7,0,0},
		{0,0,0,0,0,0,0,9,7,0,0,0,0,0,0,9,0,0,7,0},
		{0,9,8,8,7,0,9,0,0,7,0,0,0,0,9,0,0,0,0,8},
		{8,0,0,0,0,8,0,0,0,0,7,0,0,9,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,7,9,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};	
	int key=0;
	int laser = 0;
	int justLaser = 0;
	int missile = 0;
	int justMissile = 0;
	int missileSlow = 2;
	short int keys[8];
	POSITION laserPos;
	POSITION oldLaserPos;
	POSITION missilePos;
	POSITION oldMissilePos;
	POSITION oldRType;
	POSITION rTypePos = {0,4};
	int score = 0;
	int forward = 0;
	//int map_x_location=0;
	//POSITION pos = {80,5};
	//int pos = 0;
	INT_HANDLER interrupt1 = GetIntVec(AUTO_INT_1);  // this will save auto int 1

	// print a simple title and disclaimer
	clrscr();
	printf("A game by Ben Cherry \nversion 0.2 Beta \nUse at your own risk! \nreport any bugs to me\nArrow keys - move ship\n2nd-fire straight laser\ndiamond-fire down missile\nDestroy the blocks!\nGame may become unstable\npast the ground raise!\nPress Enter to Begin");
	while (ngetchx() != KEY_ENTER){}   
	clrscr();

	// seed the random numbers
	randomize();
	
	// get the key masks	
	getKeyMasks(keys);

	// DESTROY auto-interrupt 1 so as not to mess up _rowread
	SetIntVec(AUTO_INT_1,DUMMY_HANDLER);

	if (!GrayOn ()) return;

	Draw_Map(map1, rTypePos, laserPos, laser, missilePos, missile);
	
	
	// the main game loop
	while (!quit()) {
		
		// if the user has a missile out, deal with it
		if (missile && missileSlow == 2) {
			missileSlow = 1;
			oldMissilePos = missilePos;
			missilePos.x++;
			missilePos.y++;
			if (missilePos.x > 20) missile = 0;
			if (blowWall(missilePos,map1)) {
				map1[missilePos.y][missilePos.x]-=2;//= 0;
				/**/score++;
				/**/if (map1[missilePos.y][missilePos.x] < 0) map1[missilePos.y][missilePos.x] = 0;
				missile = 0;
				justMissile = 0;
				Draw_Map(map1,rTypePos,laserPos,laser,missilePos,missile);
			}
			// if the shot was just fired, then there wont be one to erase
			if (!justMissile && missile) {
				moveMissile(oldMissilePos,missilePos);
			} else if (missile) {
				drawMissile(missilePos);
				justMissile = 0;
			}
		} else {
			missileSlow++;
		}
		
		// if the user has a laser shot that is still going, continue it
		if (laser) {
			oldLaserPos = laserPos;
			laserPos.x++;
			if (laserPos.x > 20) laser=0;
			if (blowWall(laserPos,map1)) {
				map1[laserPos.y][laserPos.x]--; //= 0;
				/**/if (map1[laserPos.y][laserPos.x] == 0) score++;
				laser = 0;
				justLaser = 0;
				Draw_Map(map1,rTypePos,laserPos,laser, missilePos, missile);				
			}
			// if the shot was just fired, then there wont be one to erase
			if (!justLaser && laser) {
				moveLaser(oldLaserPos,laserPos);
			} else if (laser){
				drawLaser(laserPos);
				justLaser = 0;
			}
		}	
		
		// scroll the screen forward one block every third time through the main loop
		if (forward == 3) {
		randMap(&map1);
		//rTypePos.x++;
		forward = 0;
		Draw_Map(map1,rTypePos,laserPos,laser, missilePos, missile);
		} else {
			forward++;
		}
		
		// if you ran into a wall, quit
		if (detectWall(rTypePos,map1)) break;
		
		
		
		key = _rowread(ARROW_ROW);
		
		// if the user pressed right, move the ship right
		if (key & keys[RIGHT]) {
			oldRType = rTypePos;
			rTypePos.x++;
			if (rTypePos.x > 20) rTypePos.x--;
			if (!detectWall(rTypePos,map1)){
				moveRType(oldRType,rTypePos);
			}	else {
				//rTypePos.x--;
				break;
			}
		}	
			// If the user pressed left, move the ship left			
			if (key & keys[LEFT]) {
				oldRType = rTypePos;
				//rTypePos.x--;
				if (rTypePos.x - 1 >= 0) rTypePos.x--;
				if (!detectWall(rTypePos,map1)){
					moveRType(oldRType,rTypePos);
				} else {
					//rTypePos.x++;
					break;
				}
			}
			
			// if the user pressed up, move the ship up
			if (key & keys[UP]) {
					oldRType = rTypePos;				
					//rTypePos.y--;	
					if (rTypePos.y - 1 >= 0) rTypePos.y--;
					if (!detectWall(rTypePos,map1))	{		
						moveRType(oldRType,rTypePos);
					} else {
						//rTypePos.y++;
						break;
					}	
			}	
			
			// if the user pressed down, move the ship down
			if (key & keys[DOWN]) {
					oldRType = rTypePos;
					rTypePos.y++;
					if (rTypePos.y > 12) rTypePos.y = 12;
					if (!detectWall(rTypePos,map1)) {
						moveRType(oldRType,rTypePos);
					} else {
						//rTypePos.y--;
						break;
					}
			}
			
			// if 2nd was pushed, fire the laser
			if (key & keys[SECOND]) {
				if (!laser) {
				justLaser = 1;
				laser = 1;
				laserPos.x = rTypePos.x + 1;
				laserPos.y = rTypePos.y;
				}
			}
			
			// if diamond was pushed fire the downward missiles
			if (key & keys[DIAMOND]) {
				if (!missile) {
					justMissile = 1;
					missile = 1;
					missileSlow = 2;
					missilePos.x = rTypePos.x;
					missilePos.y = rTypePos.y;
				}
			}
			
			// slow down the program because _rowread is too fast
			delay();
		}
		
		// somehow the user left the game, either by crashing or quitting, so make sure to disable all changes		
		GrayOff();
		SetIntVec(AUTO_INT_1,interrupt1);
		clrscr();
		printf("You destroyed %d blocks!",score);
		while (ngetchx() != KEY_ENTER){}
	}



