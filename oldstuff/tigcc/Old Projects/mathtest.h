// C Header File
// Created 20/Oct/03; 18:23:54
/*
   MathTest 1.0
   A Simple Mathematics Test Program to Illustrate the Use of Functions in C
   Copyright (C) 2000 John David Ratliff

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2
   of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
   
	Begin function prototyping here
*/

// returns the value of its argument times 2
int times2(int);

// returns the square of its argument
int square(int);

// returns the first argument raised to the
// power of the second argument
long int power(int, int);

// returns the sum of the numbers between the
// first argument and the second argument
int sum(int, int);

// gets a number between 0 and 9 from the user
// using keyboard input
short int getNumber(void);

// displays a short pause messsage and waits
// for user input before continuing
inline void pause(void);

// the entry point of the program
void _main(void);

/*
	End of Function Prototypes
*/
