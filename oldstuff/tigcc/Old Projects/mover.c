// C Source File
// Created 10/26/03; 3:15:56 PM

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

#define YMIN 4
#define YMAX 86
#define XMIN 4
#define XMAX 146
#define SPRITE_HEIGHT	8

// Main Function
void _main(void)
{
	// declare the sprite positions
	int x1 = 4, y1 = 4;
	int x2 = 14, y2 = 14;
	int x3 = 24, y3 = 24;

	int b1 = 0, b2 = 0, b3 = 0;

	// keycode variable
	int key = 0;

	// declare the sprites (8,16,32) pixels wide x 8 pixels tall
	unsigned char sprite1[] = {0x3C,0x7E,0xFF,0xFF,0xFF,0xFF,0x7E,0x3C};
	unsigned char sprite2[] = {0x3C,0x42,0x81,0x81,0x81,0x81,0x42,0x3C};
	unsigned char sprite3[] = {0x3C,0x42,0x81,0xFF,0xFF,0x81,0x42,0x3C};

	// clear the screen
	ClrScr();

	// draw borders
	DrawLine(3,3,3,95,A_NORMAL);
	DrawLine(3,3,155,3,A_NORMAL);
	DrawLine(3,95,155,95,A_NORMAL);
	DrawLine(155 ,3,155,95,A_NORMAL);

	// draw the sprites
	Sprite8(x1, y1, SPRITE_HEIGHT, sprite1, LCD_MEM, SPRT_XOR);
	Sprite8(x2, y2, SPRITE_HEIGHT, sprite2, LCD_MEM, SPRT_XOR);
	Sprite8(x3, y3, SPRITE_HEIGHT, sprite3, LCD_MEM, SPRT_XOR);

	// wait until the user presses ESC
	while ((key = ngetchx()) != KEY_ESC) {
		// if the user pressed up

		b1 = 0; b2 = 0; b3 = 0;

		if (key == KEY_UP) {
			// remove the 8-bit sprite
			Sprite8(x1, y1, SPRITE_HEIGHT, sprite1, LCD_MEM, SPRT_XOR);
			Sprite8(x2, y2, SPRITE_HEIGHT, sprite2, LCD_MEM, SPRT_XOR);
			Sprite8(x3, y3, SPRITE_HEIGHT, sprite3, LCD_MEM, SPRT_XOR);

			// alter the sprite's position
			if (y1 >= (YMIN+10)) {
				y1-=10;
			}	else {
				b1 = 1;
			}
			if (y2 >= (YMIN+10)) {
				y2-=10;
			} else {
				b2 = 1;
			}
			if (y3 >= (YMIN+10)) {
				y3-=10;
			} else {
				b3 = 1;
			}

			if (b1=1) {
				if ((y1=y2)&&(x1=x2)) {
					y2+=10;
					if ((y2=y3)&&(x2=x3)) {
						y3+=10;
					}
				}else if ((y1=y3)&&(x1=x3)) {
					y3+=10;
					if ((y2=y3)&&(x2=x3)) {
						y2+=10;
					}
				}
			}
			if (b2=1) {
				if ((y1=y2)&&(x1=x2)) {
					y1+=10;
					if ((y1=y3)&&(x1=x3)) {
						y3+=10;
					}
				}else if ((y2=y3)&&(x2=x3)) {
					y3+=10;
					if ((y1=y3)&&(x1=x3)) {
						y1+=10;
					}
				}
			}
			if (b3=1) {
				if ((y3=y2)&&(x2=x3)) {
					y2+=10;
					if ((y2=y1)&&(x1=x2)) {
						y1+=10;
					}
				}else if ((y1=y3)&&(x1=x3)) {
					y1+=10;
					if ((y2=y1)&&(x1=x2)) {
						y2+=10;
					}
				}
			}

			// redraw the sprite at the new position
			Sprite8(x1, y1, SPRITE_HEIGHT, sprite1, LCD_MEM, SPRT_XOR);
			Sprite8(x2, y2, SPRITE_HEIGHT, sprite2, LCD_MEM, SPRT_XOR);
			Sprite8(x3, y3, SPRITE_HEIGHT, sprite3, LCD_MEM, SPRT_XOR);

		// if the user pressed down
		} else if (key == KEY_DOWN) {
			// remove the sprite
			Sprite8(x1, y1, SPRITE_HEIGHT, sprite1, LCD_MEM, SPRT_XOR);
			Sprite8(x2, y2, SPRITE_HEIGHT, sprite2, LCD_MEM, SPRT_XOR);
			Sprite8(x3, y3, SPRITE_HEIGHT, sprite3, LCD_MEM, SPRT_XOR);

			// alter the sprite's position
			if (y1 <= (YMAX-10)) {
				y1+=10;
			}
			if (y2 <= (YMAX-10)) {
				y2+=10;
			}
			if (y3 <= (YMAX-10)) {
				y3+=10;
			}

			// redraw the sprite at the new position
			Sprite8(x1, y1, SPRITE_HEIGHT, sprite1, LCD_MEM, SPRT_XOR);
			Sprite8(x2, y2, SPRITE_HEIGHT, sprite2, LCD_MEM, SPRT_XOR);
			Sprite8(x3, y3, SPRITE_HEIGHT, sprite3, LCD_MEM, SPRT_XOR);

		// if the user pressed right
		} else if (key == KEY_RIGHT) {
			// remove the sprite
			Sprite8(x1, y1, SPRITE_HEIGHT, sprite1, LCD_MEM, SPRT_XOR);
			Sprite8(x2, y2, SPRITE_HEIGHT, sprite2, LCD_MEM, SPRT_XOR);
			Sprite8(x3, y3, SPRITE_HEIGHT, sprite3, LCD_MEM, SPRT_XOR);

			// adjust the sprite's position
			if (x1 <= (XMAX-10)) {
				x1+=10;
			}
			if (x2 <= (XMAX-10)) {
				x2+=10;
			}
			if (x3 <= (XMAX-10)) {
				x3+=10;
			}

			// redraw the sprite at the new position
			Sprite8(x1, y1, SPRITE_HEIGHT, sprite1, LCD_MEM, SPRT_XOR);
			Sprite8(x2, y2, SPRITE_HEIGHT, sprite2, LCD_MEM, SPRT_XOR);
			Sprite8(x3, y3, SPRITE_HEIGHT, sprite3, LCD_MEM, SPRT_XOR);

		} else if (key == KEY_LEFT) {
			// remove the sprite
			Sprite8(x1, y1, SPRITE_HEIGHT, sprite1, LCD_MEM, SPRT_XOR);
			Sprite8(x2, y2, SPRITE_HEIGHT, sprite2, LCD_MEM, SPRT_XOR);
			Sprite8(x3, y3, SPRITE_HEIGHT, sprite3, LCD_MEM, SPRT_XOR);

			//adjust the sprite's position
			if (x1 >= (XMIN+10)) {
				x1-=10;
			}
			if (x2 >= (XMIN+10)) {
				x2-=10;
			}
			if (x3 >= (XMIN+10)) {
				x3-=10;
			}

			// redraw the sprite at the new place
			Sprite8(x1, y1, SPRITE_HEIGHT, sprite1, LCD_MEM, SPRT_XOR);
			Sprite8(x2, y2, SPRITE_HEIGHT, sprite2, LCD_MEM, SPRT_XOR);
			Sprite8(x3, y3, SPRITE_HEIGHT, sprite3, LCD_MEM, SPRT_XOR);
		}
	}
}


