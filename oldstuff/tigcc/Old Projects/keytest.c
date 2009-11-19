// C Source File
// Created 11/Nov/03; 20:32:25

#define USE_TI89              // Compile for TI-89

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

// Main Function
void _main(void)
{
	int key = 0;
	int x = 0;
	int y = 45;
	int back = 0;
	int final_score = 10000;
	char name[10] = "          ";
	clrscr();
	FontSetSys(F_8x10);
	printf_xy(20, 0, "New High Score!");
	printf_xy(60, 12, "%d", final_score);
	FontSetSys(F_4x6);
	printf_xy(53, 95, "Jet Blaster v 1.0 by Ben Cherry");
	FontSetSys(F_6x8);
	printf_xy(11,25, "Please Enter Your Name:");
	while (key != KEY_ENTER) {
		key = ngetchx();
		if (key == KEY_BACKSPACE) {
			if (x > 0) x--;
			key = (int)' ';
			back = 1;
		}
		if (x <= 9 && key != KEY_ENTER) {
				printf_xy(6 * x + 50, y, "%c", (char)key);
				name[x] = key;
		}
		if (!back && x <= 9) x++;
		if (back) back--;
	}
	clrscr();
	printf("%s\n%d\n%s",(char *)name,final_score);
	while (ngetchx() != KEY_ENTER);	
}
