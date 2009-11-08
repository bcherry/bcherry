// C Source File
// Created 20/Oct/03; 22:12:55

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

void change_vars(int *x, int *y, int *z) {
	*x = 256;
	*y = 128;
	*z = 64;
}

// Main Function
void _main(void)
{
	// create three variables
	int x = 50, y = 75, z = 150;
	
	// clear the screen
	clrscr();
	
	// print the "before" values
	printf("Before:\n     x: %d\n     y: %d\n     z: %d\n\n", x, y, z);
	
	// change the variables and print them again
	change_vars(&x,&y,&z);
	printf(" After:\n     x: %d\n     y: %d\n     z: %d\n", x, y, z);
	
	// wait for user to press a key before exiting
	ngetchx();
}