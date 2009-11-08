// C Source File
// Created 2/2/2004; 5:57:44 PM

#define USE_TI89              // Compile for TI-89

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

//#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files
// Main Function
void _main(void){
	int a;
	a=NULL;
	clrscr();
	if(a)printf("NULL is true\n");
	if(!a)printf("NULL is false\n");
	ngetchx();
}
