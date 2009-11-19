// C Source File
// Created 21/Oct/03; 22:05:35
/*
    Nibbles 68k v. 4.1 -- Nibbles Snake Game Clone
    Copyright (C) 2000-2002 John David Ratliff
    Based on Stilgar's Nibbles for Fargo

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

// C Source File
// Created 7/1/2001; 7:09:53 PM

#include "nibbles.h"
#include "gfx.h"

#include <stdio.h>		// string printing functions
#include <sprites.h>		// Sprite drawing functions
#include <graph.h>		// display functions
#include <string.h>		// string copy functions
#include <stdlib.h>		// random number functions
#include <dialogs.h>		// dialog box functions
#include <alloc.h>		// memory management functions
#include <kbd.h>		// keyboard keycodes
#include <system.h>		// interrupt functions

// centerText - draws a string at the horizontal center of line y
inline void centerText(const char *str, short int y) {
	short int len = (short int)strlen(str), x = ((160 - (len * 6)) / 2);

	DrawStr(x,y,(char *)str,A_XOR);
}

// drawBorder - draws four lines making a box around the screen
inline void drawBorder(void) {
	DrawLine(X1,Y1,X2,Y2,A_NORMAL);
	DrawLine(X3,Y3,X4,Y4,A_NORMAL);
	DrawLine(X5,Y5,X6,Y6,A_NORMAL);
	DrawLine(X7,Y7,X8,Y8,A_NORMAL);
}

// drawLogo - draws the Nibbles 68k logo across the top of the screen
void drawLogo(void) {
	short int loop, offset = 0;

	// draw the logo pieces
	for (loop = 0; loop < (LOGOTILES * LOGOWIDTH); loop+=LOGOWIDTH) {
		Sprite32(loop,0,LOGOHEIGHT,logo+offset,LCD_MEM,SPRT_XOR);
		offset += LOGOHEIGHT;
	}
}

// drawSelector - draws the menu selector bar over the menu options
void drawSelector(short int y) {
	short int loop, offset = 0;

	// draw the selector pieces
	for (loop = 0; loop < (SELECTORTILES * SELECTORWIDTH); loop+=SELECTORWIDTH) {
		Sprite32(loop,y,SELECTORHEIGHT,selector+offset,LCD_MEM,SPRT_XOR);
		offset += SELECTORHEIGHT;
	}
}

// drawCopyright - prints the copyright notice at the bottom of the screen
inline void drawCopyright(void) {
	FontSetSys(F_4x6);
	DrawStr(COPYX,COPYY,(char *)intro[COPYRIGHT],A_XOR);
	DrawStr(URLX,URLY,(char *)intro[URL],A_XOR);
	FontSetSys(F_6x8);
}

// drawMenu - displays the game menu
void drawMenu(void) {
	short int loop;

	// draw the three menu selections
	for (loop = START; loop <= QUIT; loop++) {
		centerText(intro[loop],MENUSTART+(loop*MENUSTEP));
	}
}

// drawSpeedOption - draws the speed setting (i.e. slow, fast, average)
void drawSpeedOption(short int speed) {
	char speedStr[25];

	// truncate to 0-length
	speedStr[0] = 0;

	// create the speed string
	strcat(speedStr,intro[SPEED]);
	strcat(speedStr,speeds[speed]);

	// display the string
	centerText((const char *)speedStr,MENUSTART);
}

// drawDifficultyOption - draws the difficulty setting (i.e. easy, hard, medium)
void drawDifficultyOption(short int difficulty) {
	char difficultyStr[25];

	// truncate to 0-length
	difficultyStr[0] = 0;

	// create the difficulty string
	strcat(difficultyStr,intro[DIFFICULTY]);
	strcat(difficultyStr,difficulties[difficulty]);

	// display the string
	centerText((const char *)difficultyStr,MENUSTART+MENUSTEP);
}

// drawOptionsMenu - draws the options menu items
inline void drawOptionsMenu(short int speed, short int difficulty) {
	drawSpeedOption(speed);
	drawDifficultyOption(difficulty);

	centerText(intro[QUITOPTIONS],MENUSTART+(2*MENUSTEP));
}

// drawBlock - draws a block on the screen (all nibble graphics are composed of blocks)
inline void drawBlock(short int x, short int y, unsigned char *sprite, short int mode) {
	Sprite8(x,y,HEIGHT,sprite,LCD_MEM,mode);
}

// drawToken - draws a life or apple token at (x,y)
inline void drawToken(short int x, short int y, unsigned char *sprite) {
	Sprite8(x,y,TOKEN_HEIGHT,sprite,LCD_MEM,SPRT_XOR);
}

// drawScore - draws the current player score at the lower-left
inline void drawScore(unsigned long int score) {
	printf_xy(SCOREX,SCOREY,"%lu",score);
}

// drawEndMessage - draws the message at games end (i.e. winner, loser, quitter)
void drawEndMessage(short int ending) {
	short int x, offset = 0;
	unsigned short int *sprite = endings[--ending];

	for (x = 8; x < 152; x += 16) {
		Sprite16(x,38,25,sprite+offset,LCD_MEM,SPRT_AND);
		Sprite16(x,38,25,sprite+offset,LCD_MEM,SPRT_OR);
		offset += 25;
	}
}

// drawCrash - draws the crash message across the screen
void drawCrash(void) {
	short int x, offset = 0;

	for (x = 0; x < CRASHWIDTH; x += 32) {
		Sprite32(CRASHX+x,CRASHY,CRASHHEIGHT,crash+offset,LCD_MEM,SPRT_AND);
		Sprite32(CRASHX+x,CRASHY,CRASHHEIGHT,crash+offset,LCD_MEM,SPRT_OR);
		offset+=CRASHHEIGHT;
	}

	// wait for keypress
	delay(KEYDELAY);
	while (!getKey());
	delay(KEYDELAY);
}

// drawLives - displays the life indicator using life tokens
void drawLives(short int lives) {
	short int loop, x = 76, max = lives;

	if (max > MAX_LIVES) {
		max = MAX_LIVES;
	}

	// draw a life token for each remaining life
	for (loop = 0; loop < max; loop++) {
		x+=4;
		drawToken(x,97,lifeToken);
		x++;
	}
}

// drawApples - displays remaining apples using apple tokens
void drawApples(short int apples) {
	short int loop, x = 76;

	// draw one apple token for each remaining apple
	for (loop = 0; loop < apples; loop++) {
		x+=4;
		drawToken(x,94,appleToken);
		x++;
	}
}

// drawApple - draws an apple on screen at a randomly selected location
// 	returns - the position of the newly drawn apple
POSITION drawApple(short int level, SNAKE *snake) {
	POSITION pos = {0,0};

	// find a suitable random position for the apple
	while (hasHitWall(pos,level) || hasHitSnake(pos,snake,SNAKE_HIT_APPLE)) {
		pos.x = random(XTILES);
		pos.y = random(YTILES);
	}

	// draw the apple sprite
	drawBlock(pos.x * WIDTH, pos.y * HEIGHT, apple, SPRT_XOR);

	// return the apple's position
	return pos;
}

// eraseApple - erases an apple token indicating an apple has just been eaten
inline void eraseApple(short int apples) {
	drawToken(76+4+apples*4+apples,94,appleToken);
}

// drawSnake - draws the snake on the LCD
void drawSnake(SNAKE *snake) {
	POSITION pos;
	short int loop;

	// draw the snake segments
	for (loop = 0; loop < snake->length; loop++) {
		pos = snake->location[loop];
		drawBlock(pos.x * WIDTH, pos.y * HEIGHT, block, SPRT_OR);
	}

	// remove the last segment to create the illusion of movement
	pos = snake->location[snake->length];

	if (pos.x != 0 && pos.y != 0) {
		drawBlock(pos.x * WIDTH, pos.y * HEIGHT, empty, SPRT_AND);
	}
}

// clearDisplayLine - removes the bottom 3 lines of the bottom border
//			for extra indicator display room
void clearDisplayLine(void) {
	short int loop;

	for (loop = 92; loop < 95; loop++) {
		DrawLine(0,loop,160,loop,A_REVERSE);
	}
}

// drawLevel - draws one of the maze levels
void drawLevel(short int level) {
	short int yLoop,xLoop;
	long int area, mask;

	// clear the screen first
	ClrScr();

	for (yLoop = 0; yLoop < YTILES; yLoop++) {
		area = levels[level][yLoop];
		mask = 0x100000;

		// start the x loop
		for (xLoop = 0; xLoop < XTILES; xLoop++) {
			// shift the mask index
			mask >>= 1;

			// if the level data says to put a block here, then do it
			if (area & mask) {
				drawBlock(xLoop * WIDTH, yLoop * HEIGHT, block, SPRT_OR);
			}
		}
	}

	// share the bottom line of the maze
	clearDisplayLine();
}

// drawHiScoreBoard - draws the hiscore board, and updates it with the hiscore from the latest game
void drawHiScoreBoard(unsigned long int newscore) {
	SCORE scores[MAX_HISCORES];
	short int loop, pos = -1;
	char name[10], str[50], *error = "File I/O Error";
	HANDLE dlg;

	// restore interrupt handlers
	OSSetSR(0x0000);

	if (!loadHiScores(scores)) {
		// cannot open hiscore file -- display error
		DlgMessage(error,"Unable to Load HiScore Data",BT_OK,BT_NONE);
		return;
	}

	// check if latest score is a highscore
	for (loop = (MAX_HISCORES - 1); loop >= 0; loop--) {
		if (newscore > scores[loop].score) {
			// new HiScore!!
			pos = loop;
		}
	}

	if (pos != -1) {
		// if we found a new hiscore
		if ((dlg = DialogNewSimple(DLGWIDTH,DLGHEIGHT)) == H_NULL) {
			DlgMessage("Memory Allocation Error","Not Enough Free Memory!",BT_OK,BT_NONE);
		} else {
			DialogAddTitle(dlg,"New Hiscore!",BT_OK,BT_NONE);
			DialogAddRequest(dlg,5,25,"Your Name:",0,9,11);

			sprintf(str,"You earned the #%hd hiscore position!",pos+1);
			DialogAddText(dlg,5,15,str);

			do {
				// truncate name variable
				name[0] = 0;
			} while (DialogDo(dlg,CENTER,CENTER,name,NULL) != KEY_ENTER);

			// free the dialog box memory
			HeapFree(dlg);

			// move the hiscore list down
			if (pos < (MAX_HISCORES - 1)) {
				for (loop = (MAX_HISCORES - 1); loop > pos; loop--) {
					scores[loop].score = scores[loop - 1].score;
					scores[loop].name[0] = 0;
					strcpy(scores[loop].name,scores[loop - 1].name);
				}
			}

			// fill in the new hiscore
			scores[pos].score = newscore;
			scores[pos].name[0] = 0;
			strcpy(scores[pos].name,name);

			if (!saveHiScores(scores)) {
				DlgMessage(error,"Unable to save HiScore Board",BT_OK,BT_NONE);
			}
		}
	}

	// display the hiscore board

	// clear the screen
	ClrScr();

	// draw the screen borders
	drawBorder();

	// draw the game logo
	drawLogo();

	FontSetSys(F_8x10);
	DrawStr(25,35,"Hiscore Board",A_NORMAL);
	FontSetSys(F_6x8);

	for (loop = 0; loop < 5; loop++) {
		printf_xy(20,50+loop*10,"#%hd %-9s %lu",loop+1,scores[loop].name,scores[loop].score);
	}

	ngetchx();

	// disable interrupts
	OSSetSR(0x0700);

	// wait for keypresses to dissipate
	delay(KEYDELAY);
}
