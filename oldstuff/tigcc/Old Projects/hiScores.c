// C Source File
// Created 12/Nov/03; 19:00:04

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

// #define OPTIMIZE_ROM_CALLS // Use ROM Call Optimization

#define MIN_AMS 100           // Compile for AMS 1.00 or higher

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

// Main Function
void _main(void) {
	typedef struct {
		long int score;
		char * name;
	} hiscore;
	int i = 0;
	hiscore hiScoreArray[5] = {{20000,"ben cherry"},{20000,"ben cherry"},{20000,"ben cherry"},{20000,"ben cherry"},{20000,"ben cherry"}};
	clrscr();
	FontSetSys(F_8x10);
	printf_xy(36,0,"Jet Blaster");
	printf_xy(36,12,"High Scores");
	FontSetSys(F_4x6);
	printf_xy(53,95,"Jet Blaster v 1.0 by Ben Cherry");
	FontSetSys(F_6x8);
	for (i = 0; i <= 4; i++) {
		printf_xy(10,i*9 + 26,"%d. %s - %d",i+1,hiScoreArray[i].name,hiScoreArray[i].score);
	}
	ngetchx();
}
