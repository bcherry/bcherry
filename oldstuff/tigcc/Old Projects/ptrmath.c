// C Source File
// Created 21/Oct/03; 21:40:02

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

long int compare(const void *value1, const void *value2) {
	return (*(short int *)value1 - *(short int *)value2);
}

void printList(short int *list) {
	short int loop, loop2;
	
	// display the separator
	printf("-----------------------\n");
	
	// print the integer list
	for (loop = 0; loop < 5; loop++) {
		// indent the list
		printf("| ");
		
		for (loop2 = 0; loop2 < 5; loop2++) {
			// display the integers
			printf("%hd ",*(list + (5 * loop) + loop2));
		}
		
		// skip to the next line
		printf("|\n");
	}
	
	// put the final line separator
	printf("-----------------------\n\n");
}

void fillList(short int *list) {
	short int loop;
	
	for (loop = 0; loop < 25; loop++) {
		*(list + loop) = (rand() % 900) + 100;	// find numbers between 100 and 999
	}
}

// Main Function
void _main(void)
{
	short int list[25];
	
	// seed the random number generator
	randomize();
	
	// clear the screen
	clrscr();
	
	// get numbers for the list
	fillList(list);
	
	// display the title
	printf("Random List of Integers\n");
	
	// display the list
	printList(list);
	
	// give time for user to see the list
	printf("Press any key to sort...\n");
	ngetchx();
	
	// sort the list according to compare
	qsort(list,25,sizeof(short int),(compare_t)compare);
	
	// screen cleanup
	printf("Sorted List of Integers\n\n");
	
	// display the sorted list
	printList(list);
	
	// wait for user to press a key before exiting
	ngetchx();	
}