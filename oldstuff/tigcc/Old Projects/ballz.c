// C Source File
// Created 10/26/03; 3:15:56 PM

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

#define YMIN 1
#define YMAX 9
#define XMIN 1
#define XMAX 15
#define SPRITE_HEIGHT	8

// check if sprite within dimension
inline int dim(int x, int y) {
	if ( (x>=XMIN) && (x<=XMAX) && (y>=YMIN) && (y<=YMAX) ) {
		return 1;
	} else {
		return 0;
	}
}

// draw or undraw spriter at posx and posy
inline void drawBall (unsigned char spriter[], int x, int y) {
	Sprite8((4+(10*(x))), (4+(10*(y))), SPRITE_HEIGHT, spriter, LCD_MEM, SPRT_XOR);
}

// check for other balls
int ballCheck (int x, int y, unsigned short int bmap[]) {
	unsigned long int mask = 0x800 >> x, ballpos = bmap[y];
	
	if (mask & ballpos) {
		return 1;
	}
	return 0;
}

// check for objects
int objCheck (int x, int y, unsigned short int omap[]) {
	unsigned long int mask = 0x800 >> x, opos = omap[y];
	
	if (mask & opos) {
		return 1;
	}
	return 0;
}

// draw borders
inline void borderDraw (void) {
	DrawLine(3,3,3,95,A_NORMAL);
	DrawLine(3,3,155,3,A_NORMAL);
	DrawLine(3,95,155,95,A_NORMAL);
	DrawLine(155 ,3,155,95,A_NORMAL);
}

// Main Function
void _main(void)
{
	// declare the sprite positions
	int x1 = 0, y1 = 0;
	int x2 = 0, y2 = 1;
	int x3 = 2, y3 = 2;
	
	// blocked vars
	unsigned char b1=0,b2=0;
	
	// keycode variable
	int key = 0;
	
	// declare the sprites 8 pixels wide x 8 pixels tall
	unsigned char sprite1[] = {0x3C,0x7E,0xFF,0xFF,0xFF,0xFF,0x7E,0x3C};
	unsigned char sprite2[] = {0x3C,0x42,0x81,0x81,0x81,0x81,0x42,0x3C};
	unsigned char sprite3[] = {0x3C,0x42,0x81,0xFF,0xFF,0x81,0x42,0x3C};
	unsigned char box01[]   = {0xFF,0x81,0x81,0xC3,0xC3,0x81,0x81,0xFF};
	
	// make the map
	unsigned short int bmap[] = {0x000,0x000,0x000,0x000,0x000,0x000,0x000,
															0x000,0x000};
		 
	// clear the screen
	ClrScr();
	
	// draw borders
	borderDraw();

	// draw the sprites
	drawBall(sprite1,x1,y1);
	bmap[y1] = bmap[y1] + (0x800 >> x1);
	drawBall(sprite2,x2,y2);
	bmap[y2] = bmap[y2] + (0x800 >> x2);
	drawBall(sprite3,x3,y3);
	bmap[y3] = bmap[y3] + (0x800 >> x3);
	
	// wait until the user presses ESC
	while ((key = ngetchx()) != KEY_ESC) {
		
		b1=0,b2=0;
		
		// find which direction was pushed
		if (key==KEY_UP) {
			if (dim(x1,y1)) {
				if (!ballCheck(x1,y1,bmap)) {
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] - (0x800 >> x1);
					y1-=1;
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] + (0x800 >> x1);
				} else {
					b1=1;
				}
			}
			
			if (dim(x2,y2)) {
				if (!ballCheck(x2,y2,bmap)) {
					drawBall(sprite2,x2,y2);
					bmap[y2] = bmap[y2] - (0x800 >> x2);
					y2-=1;
					drawBall(sprite2,x2,y2);
					bmap[y2] = bmap[y2] + (0x800 >> x2);
				} else {
					b2=1;
				}
			}
			
			if (dim(x3,y3)) {
				if (!ballCheck(x3,y3,bmap)) {
					drawBall(sprite3,x3,y3);
					bmap[y3] = bmap[y3] - (0x800 >> x3);
					y3-=1;
					drawBall(sprite3,x3,y3);
					bmap[y3] = bmap[y3] + (0x800 >> x3);
				}
			}
			
			if (b1==1) {
				if (!ballCheck(x1,y1-1,bmap)) {
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] - (0x800 >> x1);
					y1-=1;
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] + (0x800 >> x1);
					b1=0;
				}
			}
			
			if (b2==1) {
				if (!ballCheck(x2,y2-1,bmap)) {
					drawBall(sprite2,x2,y2);
					bmap[y2] = bmap[y2] - (0x800 >> x2);
					y2-=1;
					drawBall(sprite2,x2,y2);
					bmap[y2] = bmap[y2] + (0x800 >> x2);
				}
			}
			
			if (b1==1) {
				if (!ballCheck(x3,y3,bmap)) {
					drawBall(sprite3,x3,y3);
					bmap[y3] = bmap[y3] - (0x800 >> x3);
					y1-=1;
					drawBall(sprite3,x3,y3);
					bmap[y3] = bmap[y3] + (0x800 >> x3);
				}
			}
	// DOWN
		} else if (key==KEY_DOWN) {
			if (dim(x1,y1)) {
				if (!ballCheck(x1,y1,bmap)) {
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] + (0x800 >> x1);
					y1++;
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] - (0x800 >> x1);
				} else {
					b1=1;
				}
			}
			
			if (dim(x2,y2)) {
				if (!ballCheck(x2,y2,bmap)) {
					drawBall(sprite2,x2,y2);
					bmap[y2] = bmap[y2] + (0x800 >> x2);
					y2++;
					drawBall(sprite2,x2,y2);
					bmap[y2] = bmap[y2] - (0x800 >> x2);
				} else {
					b2=1;
				}
			}
			
			if (dim(x3,y3)) {
				if (!ballCheck(x3,y3,bmap)) {
					drawBall(sprite3,x3,y3);
					bmap[y3] = bmap[y3] - (0x800 >> x3);
					y3++;
					drawBall(sprite3,x3,y3);
					bmap[y3] = bmap[y3] + (0x800 >> x3);
				}
			}
			
			if (b1==1) {
				if (!ballCheck(x1,y1,bmap)) {
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] + (0x800 >> x1);
					y1++;
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] - (0x800 >> x1);
					b1=0;
				}
			}
			
			if (b2==1) {
				if (!ballCheck(x2,y2,bmap)) {
					drawBall(sprite2,x2,y2);
					bmap[y2] = bmap[y2] + (0x800 >> x2);
					y2++;
					drawBall(sprite2,x2,y2);
					bmap[y2] = bmap[y2] - (0x800 >> x2);
				}
			}
			
			if (b1==1) {
				if (!ballCheck(x1,y1,bmap)) {
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] + (0x800 >> x1);
					y1++;
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] - (0x800 >> x1);
				}
			}
		// RIGHT
		} else if (key==KEY_RIGHT) {
			if (dim(x1,y1)) {
				if (!ballCheck(x1,y1,bmap)) {
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] - (0x800 >> x1);
					x1++;
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] + (0x800 >> x1);
				} else {
					b1=1;
				}
			}
			
			if (dim(x2,y2)) {
				if (!ballCheck(x2,y2-1,bmap)) {
					drawBall(sprite2,x2,y2);
					bmap[y2] = bmap[y2] - (0x800 >> x2);
					x2++;
					drawBall(sprite2,x2,y2);
					bmap[y2] = bmap[y2] + (0x800 >> x2);
				} else {
					b2=1;
				}
			}
			
			if (dim(x3,y3)) {
				if (!ballCheck(x3,y3-1,bmap)) {
					drawBall(sprite3,x3,y3);
					bmap[y3] = bmap[y3] - (0x800 >> x3);
					x3++;
					drawBall(sprite3,x3,y3);
					bmap[y3] = bmap[y3] + (0x800 >> x3);
				}
			}
			
			if (b1==1) {
				if (!ballCheck(x1,y1,bmap)) {
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] - (0x800 >> x1);
					x1++;
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] + (0x800 >> x1);
					b1=0;
				}
			}
			
			if (b2==1) {
				if (!ballCheck(x2,y2,bmap)) {
					drawBall(sprite2,x2,y2);
					bmap[y2] = bmap[y2] - (0x800 >> x2);
					x2++;
					drawBall(sprite2,x2,y2);
					bmap[y2] = bmap[y2] + (0x800 >> x2);
				}
			}
			
			if (b1==1) {
				if (!ballCheck(x1,y1,bmap)) {
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] - (0x800 >> x1);
					x3++;
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] + (0x800 >> x1);
				}
			}
		// LEFT
		} else if (key==KEY_LEFT) {
			if (dim(x1,y1)) {
				if (!ballCheck(x1,y1,bmap)) {
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] - (0x800 >> x1);
					x1-=1;
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] + (0x800 >> x1);
				} else {
					b1=1;
				}
			}
			
			if (dim(x2,y2)) {
				if (!ballCheck(x2,y2-1,bmap)) {
					drawBall(sprite2,x2,y2);
					bmap[y2] = bmap[y2] - (0x800 >> x2);
					x2-=1;
					drawBall(sprite2,x2,y2);
					bmap[y2] = bmap[y2] + (0x800 >> x2);
				} else {
					b2=1;
				}
			}
			
			if (dim(x3,y3)) {
				if (!ballCheck(x3,y3-1,bmap)) {
					drawBall(sprite3,x3,y3);
					bmap[y3] = bmap[y3] - (0x800 >> x3);
					x3-=1;
					drawBall(sprite3,x3,y3);
					bmap[y3] = bmap[y3] + (0x800 >> x3);
				}
			}
			
			if (b1==1) {
				if (!ballCheck(x1,y1,bmap)) {
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] - (0x800 >> x1);
					x1-=1;
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] + (0x800 >> x1);
					b1=0;
				}
			}
			
			if (b2==1) {
				if (!ballCheck(x2,y2,bmap)) {
					drawBall(sprite2,x2,y2);
					bmap[y2] = bmap[y2] - (0x800 >> x2);
					x2-=1;
					drawBall(sprite2,x2,y2);
					bmap[y2] = bmap[y2] + (0x800 >> x2);
				}
			}
			
			if (b1==1) {
				if (!ballCheck(x1,y1,bmap)) {
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] - (0x800 >> x1);
					x1-=1;
					drawBall(sprite1,x1,y1);
					bmap[y1] = bmap[y1] + (0x800 >> x1);
				}
			}
		}
	}
}