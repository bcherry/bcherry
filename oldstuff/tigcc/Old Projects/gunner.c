// C Source File
// Created 11/15/03; 4:49:22 PM

#define USE_TI89              // Compile for TI-89

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files
#include "gunner.h"

INT_HANDLER interrupt1;

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
	int key = 0;
	while (key != KEY_ENTER){
		printf_xy(0,0,"delay - %d friction - %d", game_speed, game_friction);
		
		key = ngetchx();
		if (key == KEY_UP) game_speed+=5;
		if (key == KEY_DOWN) game_speed-=5;
		if (key == KEY_LEFT) game_friction-=1;
		if (key == KEY_RIGHT) game_friction+=1;
	}
}

// clear planes
inline void CLRSCR(void) {
	GraySetAMSPlane(LIGHT_PLANE);
	ClrScr();
	GraySetAMSPlane(DARK_PLANE);
	ClrScr();
}

// draw gunman
inline void drawGunner(POSITION pos) {
		Sprite16(pos.x,pos.y,16,gunmanLIGHT,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
		Sprite16(pos.x,pos.y,16,gunmanDARK,GrayGetPlane(DARK_PLANE),SPRT_XOR);
}

// delay program
inline void delay(unsigned int number) {
	unsigned short loop, loop2;
	for (loop=0;loop<number;loop++) {
		for (loop2=0;loop2<10000;loop2++);
	}
}

// check if user pressed any button
inline short int button(int row, int key) {
	if (_rowread(row) & key) {
		return 1;
	}	
	return 0;
}

//automatic "at exit" function called when leaving...
atexit_t leaving(void) {
	CLRSCR();
	GrayOff();
	SetIntVec(AUTO_INT_1, interrupt1);	
	return 0;
}

short int isAble(char axis, int pos, int change) {
	if (axis == 'x') {
		if (change < 0) {
			if (pos + change >= 0) return 1;
		}
		if (change > 0) {
			if (pos + change <= 160-16) return 1;
		}
	}
	if (axis == 'y') {
		if (change < 0) {
			if (pos + change >= 0) return 1;
		}
		if (change > 0) {
			if (pos + change <= 100- 16) return 1;
		}
	}
	return 0;
	
	/*if (direction == UP && pos.y > 0) return 1;
	if (direction == DOWN && pos.y < 100 - 16) return 1;
	if (direction == RIGHT && pos.x < 160 - 16) return 1;
	if (direction == LEFT && pos.x > 0) return 1;
	else return 0;*/
}

// Main Function
void _main(void)
{	
	// declare variables
	char keys[8]; getKeyMasks(keys);
	BITS bit= {0,0,0,0,0,0};
	char arrow_key=0;
	POSITION gunner_pos= {0,0};
	
	// main menu
	title();
	
	// prepare for GRAYSCALE AND ROWREAD
	interrupt1 = GetIntVec(AUTO_INT_1);
	atexit((atexit_t)leaving);
	SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
	GrayOn();
	
	while (1) {
		
		// initialize
		bit.done=0;
		gunner_pos= (POSITION){72,42};
		
		// a blank slate
		CLRSCR();
		
		//setup game, draw screen, etc
		drawGunner(gunner_pos);
		int friction_loop = 0;
		while(!button(TI89_ESCROW,TI89_ESCKEY) && !bit.done) {
			friction_loop++;
			if (friction_loop == game_friction) {
				friction_loop = 0;
				/*bit.xchange-=(bit.xchange>0);bit.xchange+=(bit.xchange<0);
				bit.ychange-=(bit.ychange>0);bit.ychange+=(bit.ychange<0);*/
				bit.xchange/=2;bit.ychange/=2;
			}
			// the game loop
			arrow_key=_rowread(ARROW_ROW);
			/*bit.draw=0;*/ /*bit.xchange=0; bit.ychange=0;*/
			
			//get input
			if (arrow_key) {
				if (arrow_key & keys[UP] /*&& isAble(UP, gunner_pos)*/ && bit.ychange > -10) {/*bit.draw=1*/;bit.ychange--;}
				if (arrow_key & keys[RIGHT] /*&& isAble(RIGHT, gunner_pos)*/&& bit.xchange < 10) {/*bit.draw=1*/;bit.xchange++;}
				if (arrow_key & keys[DOWN] /*&& isAble(DOWN, gunner_pos)*/&& bit.ychange < 10) {/*bit.draw=1*/;bit.ychange++;}
				if (arrow_key & keys[LEFT] /*&& isAble(LEFT, gunner_pos)*/&& bit.ychange > -10) {/*bit.draw=1*/;bit.xchange--;}
			}
				/*if(bit.draw) {*/
					drawGunner(gunner_pos);
					if (isAble('x',gunner_pos.x,bit.xchange)) gunner_pos.x+=bit.xchange;
					else {bit.xchange-=(bit.xchange>0);bit.xchange+=(bit.xchange<0);bit.xchange*=-1;}
					if (isAble('y',gunner_pos.y,bit.ychange)) gunner_pos.y+=bit.ychange;
					else {bit.ychange-=(bit.ychange>0);bit.ychange+=(bit.ychange<0);bit.ychange*=-1;}
					drawGunner(gunner_pos);
				/*}*/
			/*}*/	
			delay(game_speed);
		} 
		if (!bit.done) exit(0);	  														//while !quit and !done
	} 																														//infinite loop
}