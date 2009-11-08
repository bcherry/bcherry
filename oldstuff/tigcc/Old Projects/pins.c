// C Source File
// Created 19/Oct/03; 12:51:02

#define USE_TI89              // Compile for TI-89

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

// define the global constants
#define SPRITE_HEIGHT	8
#define LEFT		0
#define RIGHT		1

// Main Function
void _main(void)
{
	// declare the sprite positions
	int x1 = 20, y1 = 20, direction1 = RIGHT;
	int x2 = 40, y2 = 40, direction2 = LEFT;
	int x3 = 60, y3 = 60, direction3 = RIGHT;
	int x4 = 80, y4 = 80, direction4 = LEFT;
	int x5 = 50, y5 = 50, direction5 = RIGHT;
	
	// keycode variable
	int key = 0;
	
	// Define the pins
	unsigned char pinsprt[] = {0x18,0x3C,0x18,0x38,0x7C,0x7C,0x7C,0x38};
	unsigned long int balsprt[] = {0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
																 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
																 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
																 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
																 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
																 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
																 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,
																 0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,0xFFFFFFFF,};

	// clear the screen
	ClrScr();
	
	// Annotate the display
	DrawStr(0, 0, "These Are Bowling Pins!", A_NORMAL);
	
	// draw the sprites
	Sprite8(x1, y1, SPRITE_HEIGHT, pinsprt, LCD_MEM, SPRT_XOR);
	Sprite8(x2, y2, SPRITE_HEIGHT, pinsprt, LCD_MEM, SPRT_XOR);
	Sprite8(x3, y3, SPRITE_HEIGHT, pinsprt, LCD_MEM, SPRT_XOR);
	Sprite8(x4, y4, SPRITE_HEIGHT, pinsprt, LCD_MEM, SPRT_XOR);
	Sprite32(x5, y5, 32, balsprt, LCD_MEM, SPRT_XOR);
	
	// wait for a key before exit
	ngetchx();
}
