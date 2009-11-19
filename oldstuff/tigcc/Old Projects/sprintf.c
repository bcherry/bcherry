// C Source File
// Created 20/Oct/03; 22:27:06
#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

// a VERY simplified version of a sprintf() function
int sprintf2(char *str, const char *literal, const char *args) {
	int length = 0;

	// loop until we find the terminator
	while (*literal != 0) {
		if (*literal == '%') {
			// we have a format specifier
			// increment the literal string position
			literal++;
			
			if (*literal == 'd') {
				// we have an integer
				*str = 'i';
				*++str = 'n';
				*++str = 't';
				
				// it's too hard to convert
				// so we'll just pretend by skipping
				// the integer bytes
				args+=4;
				
				length+=3;
			} else if (*literal == 's') {
				// we have a string
				while (*args != 0) {
					// loop till we find the string terminator
					*str++ = *args++;
					length++;
				}
				
				// make sure we aren't past the end
				str--;
				args++;
			} else if (*literal == '%') {
				*str = '%';

				length++;
			} else {
				*str = 'm';
				*++str = 'i';
				*++str = 's';
				*++str = 'c';
				
				// assume all other types
				// take 1 byte
				args++;
				
				length+=4;
			}
			
			str++;
			literal++;
		} else {
			// regular character
			*str = *literal;
			str++;
			literal++;
			length++;
		}
	}
	
	// terminate the string
	*str = 0;
	
	return length;
}

// Main Function
void _main(void) {
	int length;
	char string[80];
	
	// clear the screen
	clrscr();
	
	// format the string
	length = sprintf2(string,"Test %s. %s!!","String\x00QWERTY\x00");
	
	// print the string
	printf_xy(0,0,string);
	
	// format a new string
	length = sprintf2(string,"%d %f %% %m'isc'","0030FM");
	
	// print the new string
	printf_xy(0,10,string);
	
	// wait for user to press a key to exit the program
	ngetchx();
}