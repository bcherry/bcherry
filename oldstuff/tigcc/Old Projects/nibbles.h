// C Header File
// Created 21/Oct/03; 22:07:01
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

// C Header File
// Created 7/1/2001; 6:54:17 AM
// Updated 8/22/2002; 8:48:37 PM

#ifndef _NIBBLES_H
#define _NIBBLES_H

// Compile for specific Calc
// Comment out the next line to compile for TI-92+/V200
#define USE_TI89		// TI-89 Version

#ifndef USE_TI89
#define USE_TI92PLUS		// TI-92+ Version
#define USE_V200		// V200 Version
#endif

/* Structures */

typedef struct {
	char name[10];			// player's name
	unsigned long int score;	// their score
} SCORE;

typedef struct {
	unsigned short int x:8, y:8;
} POSITION;

typedef struct {
	POSITION location[28];
	signed short int lives;
	unsigned short int dead:1, direction:2, length:13;
} SNAKE;

/* Constants */

#define	MAX_HISCORES	5
#define MAX_LIVES	16

#ifdef USE_TI89
#define DLGWIDTH 140
#define DLGHEIGHT 45

enum KeyMatrix {
	ESCROW = ~0x0040, ESCKEY = 0x0001, CHEATROW = ~0x0020, CHEATKEY = 0x0010,
	ENTERROW = ~0x0002, ENTERKEY = 0x0001, ARROWROW = ~0x0001,
	UPKEY = 0x0001, LEFTKEY = 0x0002, DOWNKEY = 0x0004, RIGHTKEY = 0x0008,
};
#else
#define DLGWIDTH 225
#define DLGHEIGHT 55

enum KeyMatrix {ESCROW = ~0x0100, ESCKEY = 0x0040, CHEATROW = ~0x0080, CHEATKEY = 0x0001,
		ENTERROW = ~0x0200, ENTERKEY = 0x0002, ARROWROW = ~0x0001,
		UPKEY = 0x0020, LEFTKEY = 0x0010, DOWNKEY = 0x0080, RIGHTKEY = 0x0040
};
#endif

enum Intro	{START,OPTIONS,QUIT,COPYRIGHT,URL,SPEED,DIFFICULTY,QUITOPTIONS};
enum Speeds	{VERYSLOW,SLOW,MEDSLOW,LEISURELY,MEDIUM,AVERAGE,AVERAGEPLUS,QUICK,FAST,VERYFAST};
enum Difficulty	{EASY,NORMAL,HARD};
enum Logo	{LOGOWIDTH = 32, LOGOHEIGHT = 20, LOGOTILES = 5};
enum Selector	{SELECTORWIDTH = 32, SELECTORHEIGHT = 8, SELECTORTILES = 5};
enum Menu	{MENUSTART = 35, MENUEND = 55, MENUSTEP = 10};
enum Copyright	{COPYX = 15, COPYY = 85, URLX = 25, URLY = 92};
enum WaitKey	{NOWAIT, WAIT};
enum Delay	{KEYDELAY = 415};
enum Keys	{KUP = 1,KDOWN,KLEFT,KRIGHT,KESC,KENTER,KCHEAT};
enum Directions	{UP,LEFT,DOWN,RIGHT};
enum Dimensions	{WIDTH = 8, HEIGHT = 5, TOKEN_HEIGHT = 2, XTILES = 20, YTILES = 19};
enum Score	{SCOREX = 10, SCOREY = 93};
enum Snake	{SNAKE_HIT_APPLE, SNAKE_HIT_SELF, SNAKE_GROW};
enum Crash	{CRASHX = 32, CRASHY = 40, CRASHHEIGHT = 20, CRASHWIDTH = 88};
enum Endings	{NONE,QUITTER,LOSER,CHEATER,WINNER,ENDY = 45};
enum Borders	{X1 = 0, X2 = 0, X3 = 0, X4 = 159, X5 = 0, X6 = 159, X7 = 159, X8 = 159,
		 Y1 = 0, Y2 = 99, Y3 = 0, Y4 = 0, Y5 = 99, Y6 = 99, Y7 = 0, Y8 = 99};

/* External Variable Declarations */

extern unsigned long int levels[10][19];

/* Function proto-types */

/* nibbles.c */

short int doIntro(short int *, short int *);
void _main(void);

/* hiscore.c */

short int loadHiScores(SCORE *);
short int saveHiScores(SCORE *);
short int createHiScoreTable(void);
short int needScoreFile(void);

/* gfx.c */

inline void centerText(const char *, short int);
inline void drawBorder(void);
void drawLogo(void);
void drawSelector(short int);
inline void drawCopyright(void);
void drawMenu(void);
void drawSpeedOption(short int);
void drawDifficultyOption(short int);
inline void drawOptionsMenu(short int, short int);
inline void drawBlock(short int, short int, unsigned char *, short int);
inline void drawToken(short int, short int, unsigned char *);
inline void drawScore(unsigned long int);
void drawEndMessage(short int);
void drawCrash(void);
void drawLives(short int);
void drawApples(short int);
POSITION drawApple(short int, SNAKE *);
inline void eraseApple(short int);
void drawSnake(SNAKE *);
void clearDisplayLine(void);
void drawLevel(short int);
void drawHiScoreBoard(unsigned long int);

/* gameplay.c */

void delay(short int);
inline short int getKey(void);
short int hasHitApple(POSITION, POSITION *);
short int hasHitWall(POSITION, short int);
short int hasHitSnake(POSITION, SNAKE *, short int);
void initSnake(SNAKE *);
void growSnake(SNAKE *);
void moveSnake(SNAKE *);
void play(short int, short int);

#endif

