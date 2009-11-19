// C Source File
// Created 22/Oct/03; 15:23:52
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
// v3.0 Created 12/10/00; 3:36:43 PM
// v3.2 Modified 3/12/01; 7:05:17 AM
// v4.0 Created 7/1/2001; 6:52:49 AM
// v4.1 Updated 8/23/2002; 8:44:12 PM

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <system.h>		// interrupt functions
#include <stdlib.h>		// random number generation
#include <graph.h>		// display functions
#include <kbd.h>		// keyboard codes

#include "nibbles.h"

// doIntro - display the game intro and wait for menu selections
// 	returns FALSE if user selects exit, TRUE otherwise
short int doIntro(short int *gamespeed, short int *gamedifficulty) {
	short int selector = MENUSTART, done = FALSE, key, options = FALSE, selection = 0;
	short int speed = *gamespeed, difficulty = *gamedifficulty;

	// clear the screen
	ClrScr();

	// draw the intro screen
	drawBorder();
	drawLogo();
	drawMenu();
	drawCopyright();
	drawSelector(selector);

	while (!done) {
		// wait for keypress
		while ((key = getKey()) == 0);

		if (key == KUP) {
			// move up the menu
			drawSelector(selector);

			if (selector == MENUSTART) {
				selector = MENUEND;
			} else {
				selector -= MENUSTEP;
			}

			drawSelector(selector);
		} else if (key == KDOWN) {
			// move down the menu
			drawSelector(selector);

			if (selector == MENUEND) {
				selector = MENUSTART;
			} else {
				selector += MENUSTEP;
			}

			drawSelector(selector);
		} else if (key == KLEFT) {
			if (options) {
				// if we are in the options menu
				selection = (selector - MENUSTART) / MENUSTEP;

				// set the speed option
				if (selection == START) {
					// same as speed option -- option 1 == SPEED
					drawSpeedOption(speed);

					if (speed == VERYSLOW) {
						speed = VERYFAST;
					} else {
						speed--;
					}

					drawSpeedOption(speed);

				// set the difficulty option
				} else if (selection == OPTIONS) {
					// same as difficulty option -- option 2 == DIFFICULTY
					drawDifficultyOption(difficulty);

					if (difficulty == EASY) {
						difficulty = HARD;
					} else {
						difficulty--;
					}

					drawDifficultyOption(difficulty);
				}
			}
		} else if (key == KENTER || key == KRIGHT) {
			// select menu option
			selection = (selector - MENUSTART) / MENUSTEP;

			if (options) {
			// if we're in the options menu

				// exit the options menu
				if (selection == QUIT) {
					// close options menu
					options = FALSE;

					// switch the options and main menus
					drawOptionsMenu(speed,difficulty);
					drawMenu();

					// reset the selector
					drawSelector(selector);
					selector = MENUSTART;
					drawSelector(selector);
				} else if (selection == START) {
					// same as speed option -- option 1 == SPEED
					drawSpeedOption(speed);

					if (speed == VERYFAST) {
						speed = VERYSLOW;
					} else {
						speed++;
					}

					drawSpeedOption(speed);
				} else if (selection == OPTIONS) {
					// same as difficulty option -- option 2 == DIFFICULTY
					drawDifficultyOption(difficulty);

					if (difficulty == HARD) {
						difficulty = EASY;
					} else {
						difficulty++;
					}

					drawDifficultyOption(difficulty);
				}
			} else {
				// if we chose to start or exit, end the loop
				if (selection == START || selection == QUIT) {
					done = TRUE;
				} else {
					// enter the options menu
					options = TRUE;

					// switch the main and options menus
					drawMenu();
					drawOptionsMenu(speed,difficulty);

					// reset the selector
					drawSelector(selector);
					selector = MENUSTART;
					drawSelector(selector);
				}
			}
		} else if (key == KESC) {
			// exit the options menu
			if (options) {
				// close options menu
				options = FALSE;

				// switch the options and main menus
				drawOptionsMenu(speed,difficulty);
				drawMenu();

				// reset the selector
				drawSelector(selector);
				selector = MENUSTART;
				drawSelector(selector);
			} else {
				selection = QUIT;
				done = TRUE;
			}
		}

		// wait for keypress to dissipate
		delay(KEYDELAY);
	}

	// set the game options
	*gamespeed = speed;
	*gamedifficulty = difficulty;

	return (selection == START) ? TRUE : FALSE;
}

void _main(void) {
	short int font = FontGetSys(), speed = AVERAGE, difficulty = NORMAL;

	// create the hiscore file, if it doesn't exist
	if (needScoreFile()) {
		createHiScoreTable();
	}

	// Make sure there are no keystrokes left in the buffer
	GKeyFlush();

	// setup medium size font
	FontSetSys(F_6x8);

	// seed the random number generator
	randomize();

	// disable interrupts
	OSSetSR(0x0700);

	// keep playing until doIntro returns FALSE (exit)
	while (doIntro(&speed,&difficulty)) {
		play(speed,difficulty);
	}

	// reenable interrupts before exit
	OSSetSR(0x0000);

	// restore the standard font
	FontSetSys(font);
}

