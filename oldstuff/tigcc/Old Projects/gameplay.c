// C Source File
// Created 21/Oct/03; 22:06:35
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
// Created 7/2/2001; 1:26:19 PM

#include <kbd.h>

#include "nibbles.h"

// delay - slows the program down
void delay(short int value) {
	short int loop1, loop2, val;

	for (loop1 = 0; loop1 < value; loop1++) {
		val = value;

		for (loop2 = 0; loop2 < value; loop2++) {
			val--;
		}
	}
}

// getKey - checks for keypresses
// 	returns - non-standard keycode of the key pressed, or 0 if no key was pressed
inline short int getKey(void) {
	if (_rowread(ARROWROW) & UPKEY) {
		return KUP;
	} else if (_rowread(ARROWROW) & DOWNKEY) {
		return KDOWN;
	} else if (_rowread(ARROWROW) & LEFTKEY) {
		return KLEFT;
	} else if (_rowread(ARROWROW) & RIGHTKEY) {
		return KRIGHT;
	} else if (_rowread(ESCROW) & ESCKEY) {
		return KESC;
	} else if (_rowread(ENTERROW) & ENTERKEY) {
		return KENTER;
	} else if (_rowread(CHEATROW) & CHEATKEY) {
		return KCHEAT;
	}

	return 0;
}

// hasHitApple - checks to see whether the snake ate an apple
// 	returns - TRUE if they hit the apple, FALSE otherwise
short int hasHitApple(POSITION apple, POSITION *snake) {
	short int snagged = FALSE;

	// if the position and the apple position match, we snagged it
	if ((apple.x == snake[0].x) && (apple.y == snake[0].y)) {
		snagged = TRUE;
	}

	// otherwise, we didn't
	return snagged;
}

// hasHitWall - checks to see whether the snake hit a wall
// 	returns - TRUE if snake hit wall, FALSE otherwise
short int hasHitWall(POSITION pos, short int level) {
	long int mask = 0x80000, area;
	short int crash = FALSE;

	// find the search area
	area = levels[level][pos.y];
	mask = mask >> pos.x;

	// if we have a match, we crashed
	if (area & mask) {
		crash = TRUE;
	}

	return crash;
}

// hasHitSnake - checks to see if the snake hit itself
// 	returns - TRUE if the snake hit itself, FALSE otherwise
short int hasHitSnake(POSITION pos, SNAKE *snake, short int start) {
	short int loop;

	// loop through the body of the snake
	for (loop = start; loop < snake->length; loop++) {
		// if the position and the snake match, we hit the snake
		if ((snake->location[loop].x == pos.x) && (snake->location[loop].y == pos.y)) {
			return TRUE;
		}
	}

	return FALSE;
}

// getApplePoints - calculates the number of points the apple is worth
//	returns - points apple is worth
short int getApplePoints(short int level, short int speed, short int difficulty) {
	short int speedBonus;

	// determine speed bonus
	if (speed <= MEDIUM) {
		speedBonus = 0;
	} else if (speed > MEDIUM && speed < QUICK) {
		speedBonus = 25;
	} else if (speed > AVERAGEPLUS && speed < VERYFAST) {
		speedBonus = 50;
	} else { // speed = VERYFAST
		speedBonus = 100;
	}

	// calculate and return bonus
	return (((level * 5) + 10 + (speedBonus)) * (++difficulty));
}

// initSnake - sets up the snake parameters
void initSnake(SNAKE *snake) {
	short int loop;

	// reset the snake to standard
	snake->dead = FALSE;
	snake->length = 3;
	snake->direction = RIGHT;

	// setup the initial position
	for (loop = 0; loop < 4; loop++) {
		snake->location[loop].x = 5 - loop;
		snake->location[loop].y = 3;
	}

	// clear all the excess positions
	for (loop = 3; loop < 28; loop++) {
		snake->location[loop].x = 0;
		snake->location[loop].y = 0;
	}
}

// growSnake - makes the snake grow
void growSnake(SNAKE *snake) {
	short int loop;

	// add snake positions for growth
	for (loop = snake->length; loop < (snake->length+SNAKE_GROW); loop++) {
		snake->location[loop].x = snake->location[snake->length].x;
		snake->location[loop].y = snake->location[snake->length].y;
	}

	// adjust the snakes length
	snake->length += SNAKE_GROW;
}

// moveSnake - moves the snake across the screen
void moveSnake(SNAKE *snake) {
	short int loop;

	// relocate the positions
	for (loop = snake->length; loop >= 0; loop--) {
		snake->location[loop] = snake->location[loop-1];
	}

	// grab the second position
	snake->location[0].x = snake->location[1].x;
	snake->location[0].y = snake->location[1].y;

	// adjust the snake based on the direction
	if (snake->direction == UP) {
		snake->location[0].y--;
	} else if (snake->direction == DOWN) {
		snake->location[0].y++;
	} else if (snake->direction == LEFT) {
		snake->location[0].x--;
	} else if (snake->direction == RIGHT) {
		snake->location[0].x++;
	}

	// re-draw the snake at the new position
	drawSnake(snake);
}

// play - the main game loop
void play(short int speed, short int difficulty) {
	SNAKE snake, *snakeptr = &snake;
	short int totalApples, levels, speedDelay, done = FALSE, applePoints, level = 0, apples;
	short int key = 0, cheater = FALSE, begin, loop, keyDelay, ending = NONE;
	unsigned long int score = 0;
	POSITION currentApple;

	// difficulty level values
	const short int startLives[3] = {6, 4, 2};
	const short int startApples[3] = {5, 8, 12};
	const short int startLevels[3] = {5, 8, 10};

	// speed delay
	const short int startDelay[10] = {750,650,600,550,425,375,325,250,175,100};

	// setup the game options
	snake.lives = startLives[difficulty];
	totalApples = startApples[difficulty];
	levels = startLevels[difficulty];
	speedDelay = startDelay[speed];

	// the main game loop
	while (!done) {
		// setup game play
		drawLevel(level);
		initSnake(snakeptr);
		drawSnake(snakeptr);
		drawScore(score);
		currentApple = drawApple(level, snakeptr);
		applePoints = getApplePoints(level, speed, difficulty);
		apples = totalApples;
		drawApples(apples);
		drawLives(snake.lives);

		begin = FALSE;
		while (!begin) {
			while ((key = getKey()) == 0);

			if (key == KESC) {
				snake.dead = TRUE;
				begin = TRUE;
				done = TRUE;
				ending = QUITTER;
			} else if (key == KENTER) {
				begin = TRUE;
			}

			// wait for keypresses to dissipate
			delay(KEYDELAY);
		}

		while (!snake.dead && apples > 0) {
			for (loop = 0; loop < speedDelay; loop++) {
				if ((key = getKey()) != 0) {
					if (speed < MEDIUM) {
						keyDelay = (6 * KEYDELAY) / 7;
					} else if (speed < AVERAGEPLUS) {
						keyDelay = (2 * KEYDELAY) / 3;
					} else if (speed < FAST) {
						keyDelay = (53 * KEYDELAY) / 100;
					} else {
						keyDelay = KEYDELAY / 2;
					}

					delay(keyDelay);

					break;
				}
			}

			switch (key) {
				case KLEFT:
					if (snake.direction != RIGHT) {
						snake.direction = LEFT;
					}
					break;
				case KUP:
					if (snake.direction != DOWN) {
						snake.direction = UP;
					}
					break;
				case KDOWN:
					if (snake.direction != UP) {
						snake.direction = DOWN;
					}
					break;
				case KRIGHT:
					if (snake.direction != LEFT) {
						snake.direction = RIGHT;
					}
					break;
				case KCHEAT:
					if (cheater == TRUE) {
						if (level < levels) {
							level++;
						}

						apples = -1;
					}

					cheater = TRUE;
					break;
				case KESC:
					done = TRUE;
					snake.dead = TRUE;
					ending = QUITTER;
					break;
				case KENTER:
					// pause game
					delay(KEYDELAY);
					while (getKey() != KENTER);
					delay(KEYDELAY);
					break;
			}

			// move the snake
			moveSnake(snakeptr);

			// check for snake hitting things
			if (hasHitWall(snake.location[0], level)) {
				// hit wall
				drawCrash();
				snake.dead = TRUE;
			} else if (hasHitSnake(snake.location[0], snakeptr, SNAKE_HIT_SELF)) {
				// hit self
				drawCrash();
				snake.dead = TRUE;
			} else if (hasHitApple(currentApple,snake.location)) {
				// ate apple
				growSnake(snakeptr);
				currentApple = drawApple(level,snakeptr);
				score += applePoints;
				drawScore(score);
				apples--;
				eraseApple(apples);

				if (apples == 0) {
					// next level, add point and life bonuses
					level++;
					score+=500*difficulty+10*snake.lives;
					snake.lives++;

					if (level > levels) {
						ending = WINNER;
						done = TRUE;
						score += difficulty*10000;
					}
				}
			}
		}

		if (snake.dead && !cheater) {
			// cheaters get infinite lives, but they pay for it
			if (--(snake.lives) < 0) {
				done = TRUE;
				ending = LOSER;
			}
		}
	}

	if (cheater) {
		score = 0;
		ending = CHEATER;
	}

	// display ending message
	drawEndMessage(ending);

	// wait for keypress before continuing
	delay(KEYDELAY);
	while (!getKey());
	delay(KEYDELAY);

	// display the hiscore board
	drawHiScoreBoard(score);
}

