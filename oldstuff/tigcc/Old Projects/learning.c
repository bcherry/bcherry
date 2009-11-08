// C Source File
// Created 19/Oct/03; 10:11:20
/*
    Slider Puzzle 1.0
    Copyright (C) 2000 John David Ratliff

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
// Created 12/20/00; 4:01:49 PM
// Updated 8/22/2002; 11:29:55 PM

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

// Main Function
void _main(void)
{
	void *kbq = kbd_queue();
	unsigned short int key;
	unsigned long int count = 0;

	// clear the screen
	clrscr();

	// print the string telling the user to press a key
	printf("Please press a key\n\n");
	
	// while no key was retrieved from the keyboard queue
	while (OSdequeue(&key, kbq)) {
		// increment the variable number by one
		count++;
	}
	
	// tell them how many loops were executed
	printf("You waited %lu loops!\n", count);
	
	// wait for input before exiting
	ngetchx();
}