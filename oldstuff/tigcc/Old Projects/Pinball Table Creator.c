// C Source File
// Created 6/22/2003; 6:56:51 PM

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define ENABLE_ERROR_RETURN   // Enable Returning Errors to AMS

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define MIN_AMS 205           // Compile for AMS 2.05 or higher

#define SAVE_SCREEN           // Save/Restore LCD Contents

#define BigBumper 1
#define RectBumper 2
#define SmallBumper 3
#define BigLeftDeltoid 4
#define BigRightDeltoid 5
#define SmallLeftDeltoid 7
#define SmallRightDeltoid 6
#define Teleporter 8
#define Flag 9
#define BallEater 10
#define Accelerator 11
#define BinaryBumper 12
#define Launcher 13
#define LeftFlipper 100
#define RightFlipper -100
#define BlackHole 14

#define DrawScreen 0
#define ScrollScreen 1

#include <tigcclib.h>         // Include All Header Files
#include "extgraph.h"					// Include extended graphic functions

void startup(void);
void cleanup(void);
void driver(void);
void sys_menu(short (*)[], short (*)[], short (*)[]);
void menu(short, short, short, short *);
void drawbox(short, short, short, short, short);


//the ball sprite

static unsigned char selector[] = {
	0b11000000,
	0b10110000,
	0b10001100,
	0b10000011,
	0b10000011,
	0b10001100,
	0b10110000,
	0b11000000,
};
		
static unsigned char ball[] = {
	0b00111100,
	0b01000010,
	0b10000001,
	0b10000001,
	0b10000001,
	0b10000001,
	0b01000010,
	0b00111100,
	0b00000000,
	0b00111100,
	0b01001110,
	0b01011110,
	0b01111110,
	0b01111110,
	0b00111100,
	0b00000000
};
//the pivot point for the flippers
unsigned char pivot[] = {
	0b00000000,
	0b00000000
};
unsigned long lflipperdown[] = {
	0b00111111100000000000000000000000,
	0b01011010011000000000000000000000,
	0b10111101000110000000000000000000,
	0b11100111000001000000000000000000,
	0b11100111000000110000000000000000,
	0b10111101000000001100000000000000,
	0b01011010000000000010000000000000,
	0b00111100000000000001100000000000,
	0b00000011110000000000011000000000,
	0b00000000001111110000000100000000,
	0b00000000000000001111000011000000,
	0b00000000000000000000111111000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00100101100000000000000000000000,
	0b01000000111000000000000000000000,
	0b00011000111110000000000000000000,
	0b00011000111111000000000000000000,
	0b01000010111111110000000000000000,
	0b00100101111111111100000000000000,
	0b00000011111111111110000000000000,
	0b00000000001111111111100000000000,
	0b00000000000000001111111000000000,
	0b00000000000000000000111100000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000
};
unsigned long lflipperup[] = {
	0b00111111111111111111111111000000,
	0b01011010000000000000000111000000,
	0b10111101000000000000111000000000,
	0b11100111000000000111000000000000,
	0b11100111000000111000000000000000,
	0b10111101000111000000000000000000,
	0b01011010111000000000000000000000,
	0b00111111000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00100101111111111111111100000000,
	0b01000010111111111111100000000000,
	0b00011000111111111100000000000000,
	0b00011000111111100000000000000000,
	0b01000010111100000000000000000000,
	0b00100101000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000
};
unsigned long rflipperdown[] = {
	0b00000000000000000000000111111100,
	0b00000000000000000000011001011010,
	0b00000000000000000001100010111101,
	0b00000000000000000010000011100111,
	0b00000000000000001100000011100111,
	0b00000000000000110000000010111101,
	0b00000000000001000000000001011010,
	0b00000000000110000000000000111100,
	0b00000000011000000000001111000000,
	0b00000000100000001111110000000000,
	0b00000011000011110000000000000000,
	0b00000011111100000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000110100100,
	0b00000000000000000000011101000010,
	0b00000000000000000001111100011000,
	0b00000000000000000011111100011000,
	0b00000000000000001111111101000010,
	0b00000000000000111111111110100100,
	0b00000000000001111111111111000000,
	0b00000000000111111111110000000000,
	0b00000000011111110000000000000000,
	0b00000000111100000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
};
unsigned long rflipperup[] = {
	0b00000011111111111111111111111100,
	0b00000011100000000000000001011010,
	0b00000000011100000000000010111101,
	0b00000000000011100000000011100111,
	0b00000000000000011100000011100111,
	0b00000000000000000011100010111101,
	0b00000000000000000000011101011010,
	0b00000000000000000000000011111100,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000011111111111111110100100,
	0b00000000000011111111111101000010,
	0b00000000000000011111111100011000,
	0b00000000000000000011111100011000,
	0b00000000000000000000011101000010,
	0b00000000000000000000000010100100,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000
};
//the "cup" in which the ball is launched from
static short launcherxy[] = {0,0};
static unsigned short launcher[] = {
	0b0000000000000000,
	0b0000000000000000,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1000000000000001,
	0b1111111111111111,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0100000000000010,
	0b0100000000000010,
	0b0111111111111110,
	0b0000000000000000
};
//the part of the launcher that moves back to kick the ball into the table
static unsigned char plunger[] = {
	0b01111110,
	0b00111100,
	0b00100100,
	0b00100100,
	0b00100100,
	0b00100100,
	0b00100100,
	0b00100100,
	0b00100100,
	0b00100100,
	0b00100100,
	0b00100100,
	0b00100100,
	0b00100100,
	0b00100100,
	0b00011000,
	0b00000000,
	0b00000000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00011000,
	0b00000000
};
//big bumpers (kicks ball in direction dependent on ball vectors, increases ball speed by 1.5)
static unsigned short largebumper[] = {
	0b0000111111110000,
	0b0001111111111000,
	0b0011000000001100,
	0b0110000000000110,
	0b1100000000000011,
	0b1100000000000011,
	0b1100000110000011,
	0b1100001111000011,
	0b1100001111000011,
	0b1100000110000011,
	0b1100000000000011,
	0b1100000000000011,
	0b0110000000000110,
	0b0011000000001100,
	0b0001111111111000,
	0b0000111111110000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000111111110000,
	0b0001111111111000,
	0b0011100000011100,
	0b0011001111001100,
	0b0011011001101100,
	0b0011010000101100,
	0b0011010000101100,
	0b0011011001101100,
	0b0011001111001100,
	0b0011100000011100,
	0b0001111111111000,
	0b0000111111110000,
	0b0000000000000000,
	0b0000000000000000
}; 
//small bumpers (kicks ball in direction dependent on ball vectors, no speed change)
static unsigned char smallbumper[] = {
	0b01111110,
	0b11111111,
	0b11000011,
	0b11011011,
	0b11011011,
	0b11000011,
	0b11111111,
	0b01111110,
	0b00000000,
	0b00000000,
	0b00111100,
	0b00100100,
	0b00100100,
	0b00111100,
	0b00000000,
	0b00000000
};
//elongated "rectangular" bumpers (kicks ball in direction dependent on ball vectors, increases ball speed by 1.1)
static unsigned char rectbumper[] = {
	0b01111110,
	0b11111111,
	0b11000011,
	0b11000011,
	0b11000011,
	0b11011011,
	0b11011011,
	0b11011011,
	0b11011011,
	0b11011011,
	0b11011011,
	0b11000011,
	0b11000011,
	0b11000011,
	0b11111111,
	0b01111110,
	0b00000000,
	0b00000000,
	0b00111100,
	0b00111100,
	0b00111100,
	0b00100100,
	0b00100100,
	0b00100100,
	0b00100100,
	0b00100100,
	0b00100100,
	0b00111100,
	0b00111100,
	0b00111100,
	0b00000000,
	0b00000000
};
//binary bumper (kicks ball in random direction)
static unsigned long binary[] = {
	0b01111111111111111111111111111110,
	0b10000000000000000000000000000001,
	0b10111000010001000000011100000001,
	0b10100101011001001110010010100101,
	0b10111001010101010001011100100101,
	0b10100101010011011111010100011001,
	0b10111001010001010001010010010001,
	0b10000001000000010001000000100001,
	0b10000000000000000000000000000001,
	0b01111111111111111111111111111110,
	0b00000000000000000000000000000000,
	0b01111000110011100000011100000000,
	0b01000111101010101110100011111110,
	0b01011010100110110001101101011010,
	0b01000110101010101110100011011010,
	0b01011010101100100000101010100100,
	0b01000110101010101110101101101000,
	0b01111010111001101010111011010000,
	0b00000011100000111011100001110000,
	0b00000000000000000000000000000000
};
//the "teeth" that eat your ball on the table
static unsigned short balleater[] = {
	0b0001111111111000,
	0b0111111111111110,
	0b1100100110010011,
	0b1100110110110011,
	0b1100111111110011,
	0b1110111111110111,
	0b1111111111111111,
	0b1110111111110111,
	0b1100110110110011,
	0b1100100110010011,
	0b0111111111111110,
	0b0001111111111000,
	0b0000000000000000,
	0b0000000000000000,
	0b0011011001101100,
	0b0011001001001100,
	0b0011000000001100,
	0b0001000000001000,
	0b0000000000000000,
	0b0001000000001000,
	0b0011001001001100,
	0b0011011001101100,
	0b0000000000000000,
	0b0000000000000000
};
//kicks ball in perpendicular direction when it hits the hypotenuse
static unsigned long leftdeltabumper[] = {
	0b00000000000000000000000000000000,
	0b01111100000000000000000000000000,
	0b11111110000000000000000000000000,
	0b11110011000000000000000000000000,
	0b11111001100000000000000000000000,
	0b11111100110000000000000000000000,
	0b11111110011000000000000000000000,
	0b11111111001100000000000000000000,
	0b11111111100110000000000000000000,
	0b11100011110011000000000000000000,
	0b11100001111001100000000000000000,
	0b11100000111100110000000000000000,
	0b11100000011110011000000000000000,
	0b11100000001111001100000000000000,
	0b11100000000111100110000000000000,
	0b11100000000011110011000000000000,
	0b11100000000001111001100000000000,
	0b11100000000000111100110000000000,
	0b11100000000000011110011000000000,
	0b11100000000000001111001100000000,
	0b11100000000000000111100110000000,
	0b11100000000000000011110011000000,
	0b11100000000000000001111001100000,
	0b11100000000000000000111100110000,
	0b11100000000000000000011110011000,
	0b11100000000000000000001111001100,
	0b11100000000000000000000111100110,
	0b11100000000000000000000111111111,
	0b01110000000000000000000111111110,
	0b01111111111111111111111111111110,
	0b00111111111111111111111111111100,
	0b00001111111111111111111111110000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00011100000000000000000000000000,
	0b00011110000000000000000000000000,
	0b00011111000000000000000000000000,
	0b00011111100000000000000000000000,
	0b00011101110000000000000000000000,
	0b00011100111000000000000000000000,
	0b00011100011100000000000000000000,
	0b00011100001110000000000000000000,
	0b00011100000111000000000000000000,
	0b00011100000011100000000000000000,
	0b00011100000001110000000000000000,
	0b00011100000000111000000000000000,
	0b00011100000000011100000000000000,
	0b00011100000000001110000000000000,
	0b00011100000000000111000000000000,
	0b00011100000000000011100000000000,
	0b00011100000000000001110000000000,
	0b00011111111111111111111000000000,
	0b00011111111111111111111000000000,
	0b00001111111111111111111000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000
};
//same as above
static unsigned long rightdeltabumper[] = {
	0b00000000000000000000000000000000,
	0b00000000000000000000000000111110,
	0b00000000000000000000000001111111,
	0b00000000000000000000000011001111,
	0b00000000000000000000000110011111,
	0b00000000000000000000001100111111,
	0b00000000000000000000011001111111,
	0b00000000000000000000110011111111,
	0b00000000000000000001100111111111,
	0b00000000000000000011001111000111,
	0b00000000000000000110011110000111,
	0b00000000000000001100111100000111,
	0b00000000000000011001111000000111,
	0b00000000000000110011110000000111,
	0b00000000000001100111100000000111,
	0b00000000000011001111000000000111,
	0b00000000000110011110000000000111,
	0b00000000001100111100000000000111,
	0b00000000011001111000000000000111,
	0b00000000110011110000000000000111,
	0b00000001100111100000000000000111,
	0b00000011001111000000000000000111,
	0b00000110011110000000000000000111,
	0b00001100111100000000000000000111,
	0b00011001111000000000000000000111,
	0b00110011110000000000000000000111,
	0b01100111100000000000000000000111,
	0b11111111100000000000000000000111,
	0b01111111100000000000000000001110,
	0b01111111111111111111111111111110,
	0b00111111111111111111111111111100,
	0b00001111111111111111111111110000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000111000,
	0b00000000000000000000000001111000,
	0b00000000000000000000000011111000,
	0b00000000000000000000000111111000,
	0b00000000000000000000001110111000,
	0b00000000000000000000011100111000,
	0b00000000000000000000111000111000,
	0b00000000000000000001110000111000,
	0b00000000000000000011100000111000,
	0b00000000000000000111000000111000,
	0b00000000000000001110000000111000,
	0b00000000000000011100000000111000,
	0b00000000000000111000000000111000,
	0b00000000000001110000000000111000,
	0b00000000000011100000000000111000,
	0b00000000000111000000000000111000,
	0b00000000001110000000000000111000,
	0b00000000011111111111111111111000,
	0b00000000011111111111111111111000,
	0b00000000011111111111111111110000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000,
	0b00000000000000000000000000000000
};
//there are only two of these on a table, if at all.  When a ball
//lands in one, it comes out the other w/o altering the vectors
static unsigned short teleporter[] = {
	0b0011111111000000,
	0b0111000011100000,
	0b1111111111110000,
	0b1110000001110000,
	0b1010000001010000,
	0b1010000001010000,
	0b1010000001010000,
	0b1010000001010000,
	0b1110000001110000,
	0b1111111111110000,
	0b0111000011100000,
	0b0011111111000000,
	0b0000000000000000,
	0b0000111100000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0100000000100000,
	0b0100011000100000,
	0b0100011000100000,
	0b0100000000100000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000111100000000,
	0b0000000000000000
};
//kicks ball in opposite vertical vector upon impact, disappears when hit,
//and reappears once ball lands in black hole.  Once all of these (if any)
//are knocked down on a table, the black hole appears.
static unsigned char flag[] = {
	0b01111110,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b01000010,
	0b11111111,
	0b11111111,
	0b00000000,
	0b00111100,
	0b00100100,
	0b00100100,
	0b00100100,
	0b00100100,
	0b00000000,
	0b00000000
};
//the black hole.  This is the only object on a table that has gravity of its own.
//when the ball strays near the hole, it is sucked toward it by slightly altering
//the ball's vectors.  This only appears when all of the flags, if any, are knocked down.
//if the ball lands inside the hole, then the user gains an extra life, all of the flags
//are reactivated, and the hole disappears until the flags are knocked down again.
static short cgravity[] = {0,0};
static unsigned short centralgravity[] = {
	0b0001111111111000,
	0b0011111111111100,
	0b0111100011111110,
	0b1111011100001111,
	0b1110110001110111,
	0b1110100011011011,
	0b1110110110001011,
	0b1110011111001011,
	0b1101001111100111,
	0b1101000110110111,
	0b1101101100010111,
	0b1110111000110111,
	0b1111000011101111,
	0b0111111100011110,
	0b0011111111111100,
	0b0001111111111000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000011100000000,
	0b0000100011110000,
	0b0001001110001000,
	0b0001011100100100,
	0b0001001001110100,
	0b0001100000110100,
	0b0010110000011000,
	0b0010111001001000,
	0b0010010011101000,
	0b0001000111001000,
	0b0000111100010000,
	0b0000000011100000,
	0b0000000000000000,
	0b0000000000000000
};
//when the ball makes contact with this object, it is shot upwards at a speed
//dependent on the strength of the table's gravity
static unsigned short accelerator[] = {
	0b0110000000000110,
	0b1101000000001011,
	0b1011000000001101,
	0b0110000000000110,
	0b0000000000000000,
	0b0110000000000110,
	0b1011000110001101,
	0b1101001001001011,
	0b0110010000100110,
	0b0000000110000000,
	0b0110000110000110,
	0b1101000110001011,
	0b1011000110001101,
	0b0110000000000110,
	0b0000000000000000,
	0b0110000000000110,
	0b1011000000001101,
	0b1101000000001011,
	0b0110000000000110,
	0b0000000000000000,
	0b0000000000000000,
	0b0010000000000100,
	0b0100000000000010,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0100000000000010,
	0b0010000110000100,
	0b0000001111000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0010000000000100,
	0b0100000000000010,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0100000000000010,
	0b0010000000000100,
	0b0000000000000000,
	0b0000000000000000
};
//does the same as triangular bumpers, but just smaller
static unsigned short litedeltabumperleft[] = {
	0b0111111111111110,
	0b1111111111111111,
	0b1111111111111111,
	0b0100111100000111,
	0b0010011100000111,
	0b0001001110000111,
	0b0000100111000111,
	0b0000010011100111,
	0b0000001001111111,
	0b0000000100111111,
	0b0000000010011111,
	0b0000000001001111,
	0b0000000000100111,
	0b0000000000010111,
	0b0000000000001111,
	0b0000000000000110,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000011111000,
	0b0000000011111000,
	0b0000000001111000,
	0b0000000000111000,
	0b0000000000011000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000
};
//this is the mirror image of the one above, but this particular
//object is not yet implemented
static unsigned short litedeltabumperright[] = {
	0b0111111111111110,
	0b1111111111111111,
	0b1111111111111111,
	0b1110000011110010,
	0b1110000011100100,
	0b1110000111001000,
	0b1110001110010000,
	0b1110011100100000,
	0b1111111001000000,
	0b1111110010000000,
	0b1111100100000000,
	0b1111001000000000,
	0b1110010000000000,
	0b1110100000000000,
	0b1111000000000000,
	0b0110000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0001111100000000,
	0b0001111100000000,
	0b0001111000000000,
	0b0001110000000000,
	0b0001100000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000,
	0b0000000000000000
};
//this appears in the lower corner of the table when horizontal gravity is activated
static short grayflag = 1, initilized = 0;
INT_HANDLER save_vec_auto_int_1;
char title[80], str1[20], str2[20], str3[20], str4[20], str5[20], str6[20], str7[20], str8[20], str9[20], str0[20];
char tname[20], pword[20], cname[20];
char *scrollscreen, *drawscreen, *graybuffer;
FILE *opentable;
static int curritem = 1, currscreen = DrawScreen, numobjects = 0;
static int putpress = 0;

void GraySprite8 (short x, short y, short h, char *spr, char *video_mem_addr, short mode)
{
	if(grayflag == 1 && initilized == 1 && currscreen == DrawScreen) {
		Sprite8 (x, y, h, spr, drawscreen, mode);
		Sprite8 (x, y, h, spr, graybuffer, mode);
		Sprite8 (x, y, h, spr+h, graybuffer, mode);
	}
	else {
		//PortSet(video_mem_addr, 239, 127);
		Sprite8 (x, y, h, spr, video_mem_addr, mode);
	}
}

void GraySprite16 (short x, short y, short h, short *spr, char *video_mem_addr, short mode)
{
	if(grayflag == 1 && initilized == 1 && currscreen == DrawScreen) {
		Sprite16 (x, y, h, spr, drawscreen, mode);
		Sprite16 (x, y, h, spr, graybuffer, mode);
		Sprite16 (x, y, h, spr+h, graybuffer, mode);
	}
	else {
		//PortSet(video_mem_addr, 239, 127);
		Sprite16 (x, y, h, spr, video_mem_addr, mode);
	}
}

void GraySprite32 (short x, short y, short h, long *spr, char *video_mem_addr, short mode)
{
	if(grayflag == 1 && initilized == 1 && currscreen == DrawScreen) {
		Sprite32 (x, y, h, spr, drawscreen, mode);
		Sprite32 (x, y, h, spr, graybuffer, mode);
		Sprite32 (x, y, h, spr+h, graybuffer, mode);
	}
	else {
		//PortSet(video_mem_addr, 239, 127);
		Sprite32 (x, y, h, spr, video_mem_addr, mode);
	}
}

void clearfullscreen()
{
	ClrScr();
	if(grayflag == 1 && initilized == 1) {
		PortSet(GetPlane(DARK_PLANE), 239, 127);
		ClrScr();
		PortSet(GetPlane(LIGHT_PLANE), 239, 127);
		ClrScr();
		PortSet(graybuffer, 239, 127+128);
		ClrScr();
	}
}

void make_menu_box(short height, short width, short x, short y) {
	int i;
	for(i=x; i<=width; i++)
	{
		DrawLine(i,y,i,y+height,A_REVERSE);
	}
	drawbox(x,y,width,height,0);	
	drawbox(x+1,y+1,width-2,height-2,0);
	drawbox(x+3,y+3,width-6,height-6,0);
}

//originally from cblaster
void inputStr(char *str,short x,short y,short maxlen) {
	short i=0,key=0,len=maxlen;
	while(key != KEY_ENTER) {
		str[i]='_'; str[i+1]=' '; str[i+2]=0;
		DrawStr(x,y,str,A_NORMAL);
		key=ngetchx();
		if(key == KEY_ESC) break;
		if(key>=' '&&key<='~'&&i<len) {str[i] = ' ';DrawStr(x,y,str,A_REPLACE);str[i++]=key;};
		if(key==257&&i) {str[i] = ' ';DrawStr(x,y,str,A_REPLACE);i--;str[i] = ' ';DrawStr(x,y,str,A_REPLACE);};
		if(key=='\\'&&i) len=strlen(str)+7;
	}
	str[i]=0;
}

void drawsprite(short x, short y, char *vmaddr, short mode)
{
	if(curritem == BigBumper) GraySprite16(x, y, 16, largebumper, vmaddr, mode);
	if(curritem == RectBumper) GraySprite8(x, y, 16, rectbumper, vmaddr, mode);
	if(curritem == SmallBumper) GraySprite8(x, y, 8, smallbumper, vmaddr, mode);
	if(curritem == BigLeftDeltoid) GraySprite32(x, y, 32, leftdeltabumper, vmaddr, mode);
	if(curritem == BigRightDeltoid) GraySprite32(x, y, 32, rightdeltabumper, vmaddr, mode);
	if(curritem == SmallLeftDeltoid) GraySprite16(x, y, 16, litedeltabumperleft, vmaddr, mode);
	if(curritem == SmallRightDeltoid) GraySprite16(x, y, 16, litedeltabumperright, vmaddr, mode);
	if(curritem == Teleporter) GraySprite16(x, y, 12, teleporter, vmaddr, mode);
	if(curritem == Flag) GraySprite8(x, y, 8, flag, vmaddr, mode);
	if(curritem == BallEater) GraySprite16(x, y, 12, balleater, vmaddr, mode);
	if(curritem == Accelerator) GraySprite16(x, y, 20, accelerator, vmaddr, mode);
	if(curritem == BinaryBumper) GraySprite32(x, y, 10, binary, vmaddr, mode);
	if(curritem == LeftFlipper) GraySprite32(x, y, 13, lflipperdown, vmaddr, mode);
	if(curritem == RightFlipper) GraySprite32(x, y, 13, rflipperdown, vmaddr, mode);
	if(curritem == BlackHole) GraySprite16(x, y, 16, centralgravity, vmaddr, mode);
	if(curritem == Launcher) {
		GraySprite16(x, y, 8, launcher, vmaddr, mode);
		GraySprite8(x+4, y-2, 8, ball, vmaddr, mode);
		GraySprite8(x+4, y+8, 16, plunger, vmaddr, mode);
	}
}

inline void initscreens(short (*objectflags)[], short (*objectx)[], short (*objecty)[])
{
	PortSet(drawscreen, 239, 127+128);
	clearfullscreen();
	PortSet(scrollscreen, 239, 127+256);
	ClrScr();
	int center = 160/2;
	Sprite8(0,12,8,selector,scrollscreen,SPRT_XOR);
	Sprite16(center+8,8,16,largebumper,scrollscreen,SPRT_XOR);
	Sprite8(center+12,8+32,16,rectbumper,scrollscreen,SPRT_XOR);
	Sprite8(center+12,12+32+32,8,smallbumper,scrollscreen,SPRT_XOR);
	Sprite32(center,32+32+32,32,leftdeltabumper,scrollscreen,SPRT_XOR);
	Sprite32(center,32+32+32+32,32,rightdeltabumper,scrollscreen,SPRT_XOR);
	Sprite16(center+8,32+32+32+32+32+8,16,litedeltabumperright,scrollscreen,SPRT_XOR);
	Sprite16(center+8,32+32+32+32+32+32+8,16,litedeltabumperleft,scrollscreen,SPRT_XOR);
	Sprite16(center+10,32+32+32+32+32+32+32+10,12,teleporter,scrollscreen,SPRT_XOR);
	Sprite8(center+12,32+32+32+32+32+32+32+32+12,8,flag,scrollscreen,SPRT_XOR);
	Sprite16(center+8,32+32+32+32+32+32+32+32+32+10,12,balleater,scrollscreen,SPRT_XOR);
	Sprite16(center+8,32+32+32+32+32+32+32+32+32+32+6,20,accelerator,scrollscreen,SPRT_XOR);
	Sprite32(center,32+32+32+32+32+32+32+32+32+32+32+11,10,binary,scrollscreen,SPRT_XOR);
	PortSet(drawscreen, 239, 127+128);
	int i;
	for(i=0; i<75; i++) {
		curritem = (*objectflags)[i];
		drawsprite((*objectx)[i], (*objecty)[i], drawscreen, SPRT_XOR);
	}
	curritem = Launcher;
	if(launcherxy[0] != 0 && launcherxy[1] != 0) drawsprite(launcherxy[0], launcherxy[1], drawscreen, SPRT_XOR);
	curritem = BlackHole;
	if(cgravity[0] != 0 && cgravity[1] != 0) drawsprite(cgravity[0], cgravity[1], drawscreen, SPRT_XOR);
	curritem = BigBumper;
}

//Kevin Chang designed this.  Thanks!

void logo() {
	unsigned long part1[] = {
		 0x00000001,0x00000002,0x00000004
		,0x00000008,0x00000010,0x007C0020,0x03838040
		,0x04004080,0x1B003100,0x17E01200,0x2C800C00
		,0x4F000800,0x58001000,0x50002001,0x90004006
		,0x80008018,0x80010060,0x80020180,0x80040600
		,0x40081801,0x40106005,0x4021800D,0x2046002F
		,0x1098006F,0x1960026F,0x07800F6F,0x06001B6D
		,0x04001A6C,0x04041E68,0x040C1840,0x046C5801
		,0x05ECD806,0x05CC9018,0x04CC0060,0x04CC0180
		,0x04CC0601,0x04C81802,0x04C0610A,0x0481814A
		,0x0406014A,0x0418014B,0x04600148,0x05800158
		,0x06000120,0x00010900,0x00820A00,0x035A0400
		,0x044A0000,0x048A0000,0x07440000,0x04440000
		,0x04440000,0x04880000,0x07000000,0x04000000
	};
	unsigned long part2[] = {
		 0x00003F80,0x0003C078,0x001C0307,0x0077C600
		,0x00A83C00,0x01541E00,0x03A033C0,0x04506039
		,0x0830401E,0x100E03E0,0x2001FC03,0x40000007
		,0x80000018,0x00000061,0x00000182,0x00000600
		,0x00001800,0x00006005,0x00018022,0x00060054
		,0x00180000,0x00600845,0x0180182A,0x06001941
		,0x1801182A,0x60031D15,0x800339AB,0x00335F15
		,0x00731EAE,0x04537958,0x1E53EAE0,0xB673C580
		,0xB47A2E01,0xBE5D5802,0xB6C0E112,0xB6D5816A
		,0xBC8E0142,0xB9580133,0xA0609D08,0x0580A550
		,0x0610B920,0x181CA100,0x6014A000,0x8012B800
		,0x00138000,0x44108000,0x4A100000,0x56100000
		,0x58000000,0xD2000000,0x54000000,0x48000000
		,0xC0000000
	};
	unsigned char part3[] = {
		 0x00,0x00,0x00,0xE0
		,0x10,0x08,0x38,0xC8
		,0x18,0x78,0x88,0x58
		,0x88,0x58,0x28,0x58
		,0xA8,0x58,0x28,0x58
		,0xA8,0x58,0xB8,0x78
		,0xF8,0xF8,0xE0,0x80
		,0x00,0x04,0x1A,0x1A
		,0xD2,0x52,0x52,0x50
		,0x40,0x80,0xF8,0xE0
		,0x80,0x00,0x04,0x1A
		,0x1A,0xD2,0x52,0x52
		,0x50,0x40,0x80
	};
	unsigned char x=0,y=0;
	x = ((LCD_WIDTH-71)/2);
	y = ((LCD_HEIGHT-68)/2);
	Sprite32(x,y+13,55,part1,LCD_MEM,SPRT_XOR);
	Sprite32(x+32,y,53,part2,LCD_MEM,SPRT_XOR);
	Sprite8(x+64,y,38,part3,LCD_MEM,SPRT_XOR);
	printf_xy(((LCD_WIDTH-96)/2),92,"Press any key...");
	ngetchx();
	clearfullscreen(LCD_MEM);
}

void nullstring(char (*string)[]) {
	unsigned int i;
	for(i=0; i<strlen((*string)); i++) {
		(*string)[i] = ' ';
	}
}

void nullallstrings(void)
{
	nullstring(&title);
	nullstring(&str1);
	nullstring(&str2);
	nullstring(&str3);
	nullstring(&str4);
	nullstring(&str5);
	nullstring(&str6);
	nullstring(&str7);
	nullstring(&str8);
	nullstring(&str9);
	nullstring(&str0);
}

void sys_menu(short (*objectflags)[], short (*objectx)[], short (*objecty)[]) {
	menu:
	ClrScr();
	nullallstrings();
	strncpy(title, " TI Pinball Table Developer 0.1a ", 33);
	strncpy(str1, "New Table", 9);
	strncpy(str2, "Load Table", 10);
	strncpy(str3, "Help", 4);
	strncpy(str4, "Exit", 5);
	short r;
	menu(150,55,4,&r);
	nullallstrings();
	if(r==1)
	{
		ClrScr();
		FontSetSys(F_6x8);
		make_menu_box(40,150,LCD_WIDTH/2-75,30);
		DrawStr((LCD_WIDTH/2)-60,35,"New Table File Name:",A_REVERSE);
		char tablename[10];
		inputStr(tablename, LCD_WIDTH/2-6*4-6, 50, 8);
		SYM_ENTRY *sym = SymFindPtr(SYMSTR(tablename),0);
		ESQ type = *(HToESI (sym->handle));
		if(sym && type != OTH_TAG)
		{
			ClrScr();
			make_menu_box(40,150,LCD_WIDTH/2-75,30);
			DrawStr(LCD_WIDTH/2-20,35,"Error",A_REVERSE);
			DrawStr(LCD_WIDTH/2-72,50,"Variable already exists!",A_NORMAL);
			ngetchx();
			goto menu;
		}
		if(sym && type == OTH_TAG)
		{
			ClrScr();
			nullallstrings();
			strncpy(title, " Table already exists.  Overwrite? ", 35);
			strncpy(str1, "Yes", 3);
			strncpy(str2, "No", 2);
			short r;
			menu(150,40,2,&r);
			nullallstrings();
			if(r==1)
			{
				EM_moveSymFromExtMem(SYMSTR(tablename),HS_NULL);
				SymDel(SYMSTR(tablename));
			}
			if(r==2) goto menu;
		}
		FontSetSys(F_6x8);
		strncpy(tname, tablename, 10);
		ClrScr();
		make_menu_box(40,150,LCD_WIDTH/2-75,30);
		DrawStr(LCD_WIDTH/2-33,35,"Table Name:", A_REVERSE);
		char name[20];
		inputStr(name,LCD_WIDTH/2-60-6,50,20);
		strncpy(cname, name, 20);
		inputpass1:
		ClrScr();
		make_menu_box(40,150,LCD_WIDTH/2-75,30);
		DrawStr(LCD_WIDTH/2-54,35,"5-Letter Password:", A_REVERSE);
		char password[10];
		nullstring(&password);
		inputStr(password, LCD_WIDTH/2-15-6, 50, 5);
		if(strlen(password) != 5) goto inputpass1;
		strncpy(pword, password, 10);
		ClrScr();
		startup();
		return;
	}
	if(r==2)
	{
		ClrScr();
		FontSetSys(F_6x8);
		make_menu_box(40,150,LCD_WIDTH/2-75,30);
		DrawStr(LCD_WIDTH/2-45,35,"Open Table File", A_REVERSE);
		char tablename[10];
		inputStr(tablename, LCD_WIDTH/2-24-6, 50, 8);
		SYM_ENTRY *symbol2 = SymFindPtr(SYMSTR(tablename), 0);
		ESQ type = *(HToESI (symbol2->handle));
		if(symbol2 && type != OTH_TAG)
		{
			ClrScr();
			FontSetSys(F_6x8);
			make_menu_box(40,150,LCD_WIDTH/2-75,30);
			DrawStr(LCD_WIDTH/2-15,35,"Error",A_REVERSE);
			DrawStr(LCD_WIDTH/2-72,50,"Variable is not a table!", A_NORMAL);
			ngetchx();
			goto menu;
		}
		if(!symbol2)
		{
			ClrScr();
			nullallstrings();
			strncpy(title, " Table Does Not Exist.  Create it? ", 35);
			strncpy(str1, "Create File", 11);
			strncpy(str2, "Maybe Later", 11);
			short r;
			menu(150,40,2,&r);
			nullallstrings();
			if(r==1)
			{
				FontSetSys(F_6x8);
				strncpy(tname, tablename, 10);
				ClrScr();
				FontSetSys(F_6x8);
				make_menu_box(40,150,LCD_WIDTH/2-75,30);
				DrawStr(LCD_WIDTH/2-33,35,"Table Name:", A_REVERSE);
				char name[20];
				inputStr(name,LCD_WIDTH/2-60-6,50,20);
				strncpy(cname, name, 20);
				inputpass2:
				ClrScr();
				make_menu_box(40,150,LCD_WIDTH/2-75,30);
				DrawStr(LCD_WIDTH/2-54,35,"5-Letter Password:", A_REVERSE);
				char password[10];
				nullstring(&password);
				inputStr(password, LCD_WIDTH/2-15-6, 50, 5);
				if(strlen(password) != 5) goto inputpass2;
				strncpy(pword, password, 10);
				ClrScr();
				startup();
				return;
			}
			if(r == 2) goto menu;
		}
		char buf[100];
		EM_moveSymFromExtMem(SYMSTR(tablename),HS_NULL);
		strncpy(tname,tablename,10);
		FILE *name = fopen (tablename, "r+t");
		while (strncmp("Table=",buf,6) != 0) fgets(buf, 100, name);
		strncpy(cname, buf+6, 20);
		while (strncmp("Password=",buf,9) != 0) fgets(buf, 100, name);
		char password[10];
		strncpy(password,buf+9,10);
		strncpy(pword,password,10);
		ClrScr();
		make_menu_box(40,150,LCD_WIDTH/2-75,30);
		FontSetSys(F_6x8);
		DrawStr(LCD_WIDTH/2-45,35,"Enter Password:",A_REVERSE);
		char temppass[6];
		nullstring(&temppass);
		inputStr(temppass, LCD_WIDTH/2-15-6, 50, 5);
		if(strncmp(temppass, password, 5) != 0)
		{
			ClrScr();
			make_menu_box(40,150,LCD_WIDTH/2-75,30);
			DrawStr(LCD_WIDTH/2-48,45,"INVALID PASSWORD",A_REVERSE);
			ngetchx();
			ClrScr();
			fclose(name);
			goto menu;
		}
		ClrScr();
		FontSetSys(F_6x8);
		make_menu_box(40,150,LCD_WIDTH/2-75,30);
		DrawStr(LCD_WIDTH/2-48,45,"Loading table...",A_REVERSE);
		int i;
		for(i=0; i<75; i++) {
			fgets(buf, 100, name);
			if(strncmp("objectflag=",buf,11) == 0) (*objectflags)[i] = atoi(&buf[11]);
			fgets(buf, 100, name);
			if(strncmp("objectx=",buf,8) == 0) (*objectx)[i] = atoi(&buf[8]);
			fgets(buf, 100, name);
			if(strncmp("objecty=",buf,8) == 0) (*objecty)[i] = atoi(&buf[8]);
		}
		fgets(buf, 100, name);
		if(strncmp("launcherx=",buf,10) == 0) launcherxy[0] = atoi(&buf[10]);
		fgets(buf, 100, name);
		if(strncmp("launchery=",buf,10) == 0) launcherxy[1] = atoi(&buf[10]);
		fgets(buf, 100, name);
		if(strncmp("cgravityx=",buf,10) == 0) cgravity[0] = atoi(&buf[10]);
		fgets(buf, 100, name);
		if(strncmp("cgravityy=",buf,10) == 0) cgravity[1] = atoi(&buf[10]);
		fclose(name);
		startup();
		return;
	}
	if(r==3)
	{
		ClrScr();
		FontSetSys(F_6x8);
		printf_xy(0,0,"TI Pinball Table Developer");
		FontSetSys(F_4x6);
		printf_xy(0,8,"Written by Jude Nelson");
		printf_xy(0,16,"Contact:  judecn@aol.com");
		printf_xy(0,24,"Sponsored by www.ticalc.org");
		printf_xy(0,32,"and by www.nexusprograms.net");
		printf_xy(0,48,"This is the official TI Pinball Table");
		printf_xy(0,56,"Developer, designed to create external");
		printf_xy(0,64,"pinball tables for TI Pinball 0.2.");
		printf_xy(0,72,"It has no warranty, and therefore any");
		printf_xy(0,80,"undesirable actions this program may take");
		printf_xy(0,88,"are not my responsibility.");
		ngetchx();
		ClrScr();
		FontSetSys(F_6x8);
		printf_xy(0,0,"How to make a Table");
		FontSetSys(F_4x6);
		printf_xy(0,10,"1: From the main menu, select 'New'");
		printf_xy(0,18,"2: Enter the on-calc name of the Table");
		printf_xy(0,26,"3: Enter the in-game name of the Table");
		printf_xy(0,34,"4: Enter the password to modify the Table");
		printf_xy(0,42,"5: Draw the table (see next page)");
		printf_xy(0,50,"6: Save the table (see next page)");
		printf_xy(0,58,"7: Exit this program, and launch TI Pinball");
		ngetchx();
		ClrScr();
		FontSetSys(F_6x8);
		printf_xy(0,0,"How to Draw a Table");
		FontSetSys(F_4x6);
		printf_xy(0,10,"> Use the arrow keys to move an object");
		printf_xy(0,18,"> Press 2nd to place an object on the Table");
		printf_xy(0,26,"> Press F2 to change the type of object");
		printf_xy(0,34,"> Press F3 and F4 to place Flippers");
		printf_xy(0,42,"> Press F5 to place the Ball Launcher");
		printf_xy(0,50,"> Press Diamond to place the Black Hole");
		printf_xy(0,58,"> Press BackSpace while touching an object");
		printf_xy(0,66,"to remove it from the Table");
		printf_xy(0,74,"> Press Mode to access the Options Menu");
		printf_xy(0,82,"where you can Save or Delete your Table");
		ngetchx();
		ClrScr();
		FontSetSys(F_6x8);
		printf_xy(0,0,"The Options Menu");
		FontSetSys(F_4x6);
		printf_xy(0,10,"> The Save Option saves the current table");
		printf_xy(0,18,"to a TBL file on your calculator, which is");
		printf_xy(0,26,"interpreted by TI Pinball.  It is advised");
		printf_xy(0,34,"to have at least 3K of RAM before saving");
		printf_xy(0,42,"> The Clear Table Option deletes all of the");
		printf_xy(0,50,"information stored in the current table");
		printf_xy(0,58,"> The Exit w/o Saving Option exits the ");
		printf_xy(0,66,"Developer without saving.  This is the only");
		printf_xy(0,74,"way to 'undo' a Clear Table operation");
		ngetchx();
		ClrScr();
		goto menu;
	}
	if(r==4)
	{
		cleanup();
		exit(0);
	}
}

void menu(short width, short height, short numterms, short *r)
{
	short x,y;
	height -= 3;
	x = (LCD_WIDTH/2) - (width/2);
	y = (LCD_HEIGHT/2) - (height/2);
	short spacing = 8;
	make_menu_box(height, width, x, y);
	short x2 = x + (width - (width - 15));
	short y2 = y+6;
	FontSetSys(F_4x6);
	if(str1[0] != ' ') {printf_xy(x2, y2+((2-1)*spacing), "%s", str1);}
	if(str2[0] != ' ') {printf_xy(x2, y2+((3-1)*spacing), "%s", str2);}
	if(str3[0] != ' ') {printf_xy(x2, y2+((4-1)*spacing), "%s", str3);}
	if(str4[0] != ' ') {printf_xy(x2, y2+((5-1)*spacing), "%s", str4);}
	if(str5[0] != ' ') {printf_xy(x2, y2+((6-1)*spacing), "%s", str5);}
	if(str6[0] != ' ') {printf_xy(x2, y2+((7-1)*spacing), "%s", str6);}
	if(str7[0] != ' ') {printf_xy(x2, y2+((8-1)*spacing), "%s", str7);}
	if(str8[0] != ' ') {printf_xy(x2, y2+((9-1)*spacing), "%s", str8);}
	if(str9[0] != ' ') {printf_xy(x2, y2+((10-1)*spacing), "%s", str9);}
	if(str0[0] != ' ') {printf_xy(x2, y2+((11-1)*spacing), "%s", str0);}
	short x3 = LCD_WIDTH/2 - (4*strlen(title))/2;
	short y3 = y + 4;
	FontSetSys(F_4x6);
	DrawStr(x3,y3,title,A_XOR);
	drawbox(x+3,y+4,width-6,6,1);
	unsigned char selector[] = {
		0b00110000,
		0b01111000,
		0b11001100,
		0b11001100,
		0b01111000,
		0b00110000
	};
	short spx = x2 - 8;
	short spy = (y2 - 1) + spacing + 1;
	short returnflag = 0;
	short selection = 1;
	Sprite8 (spx, spy, 6, selector, LCD_MEM, SPRT_XOR);
	while(returnflag == 0)
	{
		short k = ngetchx();
		if(k == KEY_UP && spy > (y2-1)+2*spacing)
		{
			Sprite8 (spx, spy, 6, selector, LCD_MEM, SPRT_XOR);
			spy = spy - spacing;
			selection = selection - 1;
			Sprite8 (spx, spy, 6, selector, LCD_MEM, SPRT_XOR);
		}
		if(k == KEY_DOWN && spy < (numterms * spacing) + y2)
		{
			Sprite8 (spx, spy, 6, selector, LCD_MEM, SPRT_XOR);
			spy = spy + spacing;
			selection = selection + 1;
			Sprite8 (spx, spy, 6, selector, LCD_MEM, SPRT_XOR);
		}
		if(k == 13)
		{
			*r = selection;
			returnflag = 1;
		}
	}
}

void drawbox(short x, short y, short width, short height, short fill)
{
	DrawLine(x,y,x+width,y,A_XOR);
	DrawLine(x,y,x,y+height,A_XOR);
	DrawLine(x+width,y,x+width,y+height,A_XOR);
	DrawLine(x,y+height,x+width,y+height,A_XOR);
	if(fill == 1)
	{
		short i;
		for(i=1; i<height; i++)
		{
			DrawLine(x,y+i,x+width,y+i,A_XOR);
		}
	}
}

void getoffset(short curritem, short *x, short *y)
{
	if(curritem == BigBumper) {*x = 16; *y = 16;}
	if(curritem == RectBumper) {*x = 8; *y = 16;}
	if(curritem == SmallBumper) {*x = 8; *y = 8;}
	if(curritem == BigLeftDeltoid) {*x = 32; *y = 32;}
	if(curritem == BigRightDeltoid) {*x = 32; *y = 32;}
	if(curritem == SmallLeftDeltoid) {*x = 16; *y = 16;}
	if(curritem == SmallRightDeltoid) {*x = 16; *y = 16;}
	if(curritem == Teleporter) {*x = 12; *y = 12;}
	if(curritem == Flag) {*x = 8; *y = 8;}
	if(curritem == BallEater) {*x = 16; *y = 12;}
	if(curritem == Accelerator) {*x = 16; *y = 20;}
	if(curritem == BinaryBumper) {*x = 32; *y = 10;}
	if(curritem == Launcher) {*x = 16; *y = 24;}
}

int getcollision(short objectflags[], short objectx[], short objecty[], short objx, short objy, short offsetx, short offsety)
{
	int i, collision = 0;
	for(i=0; i<75; i++) {
		if(objectflags[i] != 0) {
			if(objectflags[i] == BigBumper && objx + offsetx >= objectx[i] && objx <= objectx[i] + 16 && objy + offsety >= objecty[i] && objy <= objecty[i] + 16) collision = 1;
			if(objectflags[i] == RectBumper && objx + offsetx >= objectx[i] && objx <= objectx[i] + 8 && objy + offsety >= objecty[i] && objy <= objecty[i] + 16) collision = 1;
			if(objectflags[i] == Accelerator && objx + offsetx >= objectx[i] && objx <= objectx[i] + 16 && objy + offsety >= objecty[i] && objy <= objecty[i] + 16) collision = 1;
			if(objectflags[i] == SmallBumper && objx + offsetx >= objectx[i] && objx <= objectx[i] + 8 && objy + offsety >= objecty[i] && objy <= objecty[i] + 8) collision = 1;
			if(objectflags[i] == BigRightDeltoid && objx + offsetx >= objectx[i] && objx <= objectx[i] + 32 && objy + offsety >= objecty[i] && objy <= objecty[i] + 32) collision = 1;
			if(objectflags[i] == BigLeftDeltoid && objx + offsetx >= objectx[i] && objx <= objectx[i] + 32 && objy + offsety >= objecty[i] && objy <= objecty[i] + 32) collision = 1;
			if(objectflags[i] == SmallRightDeltoid && objx + offsetx >= objectx[i] && objx <= objectx[i] + 16 && objy + offsety >= objecty[i] && objy <= objecty[i] + 16) collision = 1;
			if(objectflags[i] == SmallLeftDeltoid && objx + offsetx >= objectx[i] && objx <= objectx[i] + 16 && objy + offsety >= objecty[i] && objy <= objecty[i] + 16) collision = 1;
			if(objectflags[i] == BinaryBumper && objx + offsetx >= objectx[i] && objx <= objectx[i] + 32 && objy + offsety >= objecty[i] && objy <= objecty[i] + 10) collision = 1;
			if(objectflags[i] == BallEater && objx + offsetx >= objectx[i] && objx <= objectx[i] + 16 && objy + offsety >= objecty[i] && objy <= objecty[i] + 12) collision = 1;
			if(objectflags[i] == Teleporter && objx + offsetx >= objectx[i] && objx <= objectx[i] + 12 && objy + offsety >= objecty[i] && objy <= objecty[i] + 12) collision = 1;
			if(objectflags[i] == Flag && objx + offsetx >= objectx[i] && objx <= objectx[i] + 8 && objy + offsety >= objecty[i] && objy <= objecty[i] + 8) collision = 1;
			if(objectflags[i] == LeftFlipper && objx + offsetx >= objectx[i] && objx <= objectx[i] + 32 && objy + offsety >= objecty[i] && objy <= objecty[i]+13) collision = 1;
			if(objectflags[i] == RightFlipper && objx + offsetx >= objectx[i] && objx <= objectx[i] + 32 && objy + offsety >= objecty[i] && objy <= objecty[i]+13) collision = 1;
		}
	}
	if(launcherxy[0] != 0 && launcherxy[1] != 0 && objx + offsetx >= launcherxy[0] && objx <= launcherxy[0] + 16 && objy + offsety >= launcherxy[1] - 2 && objy <= launcherxy[1] + 24) collision = 1;
	if(cgravity[0] != 0 && cgravity[1] != 0 && objx + offsetx >= cgravity[0] && objx <= cgravity[0] + 16 && objy + offsety >= cgravity[1] && objy <= cgravity[1] + 16) collision = 1;
	return collision;
}

void getkey(short *objx, short *objy, short *selectx, short *selecty, short (*objectflags)[], short (*objectx)[], short (*objecty)[])
{
	short offsetx, offsety;
	getoffset(curritem, &offsetx, &offsety);
	if(currscreen == DrawScreen) {
		int delay = 35000;
		while(delay != 0) delay--;
		if(_keytest (RR_LEFT) && *objx > 0) {
			drawsprite(*objx, *objy, drawscreen, SPRT_XOR);
			(*objx)--;
			drawsprite(*objx, *objy, drawscreen, SPRT_XOR);
		}
		if(_keytest (RR_RIGHT) && *objx < 160-offsetx) {
			drawsprite(*objx, *objy, drawscreen, SPRT_XOR);
			(*objx)++;
			drawsprite(*objx, *objy, drawscreen, SPRT_XOR);
		}
		if(_keytest (RR_UP) && *objy > 0) {
			drawsprite(*objx, *objy, drawscreen, SPRT_XOR);
			(*objy)--;
			drawsprite(*objx, *objy, drawscreen, SPRT_XOR);
		}
		if(_keytest (RR_DOWN) && *objy < 200-offsety) {
			drawsprite(*objx, *objy, drawscreen, SPRT_XOR);
			(*objy)++;
			drawsprite(*objx, *objy, drawscreen, SPRT_XOR);
		}
		int spriteson = 0;
		if(*objy+(LCD_HEIGHT)/2 < 200 && *objy-(LCD_HEIGHT/2) > 0) {
			FastCopyScreen(drawscreen+30*((*objy)-(LCD_HEIGHT)/2), GetPlane(DARK_PLANE));
			FastCopyScreen(graybuffer+30*((*objy)-(LCD_HEIGHT)/2), GetPlane(LIGHT_PLANE));
			spriteson = 1;
		}
		else if(*objy <= 100 && spriteson == 0) {FastCopyScreen(drawscreen, GetPlane(DARK_PLANE)); FastCopyScreen(graybuffer, GetPlane(LIGHT_PLANE)); spriteson = 1;}
		else if(*objy > 100 && spriteson == 0) {FastCopyScreen(drawscreen+200*30-LCD_HEIGHT*30, GetPlane(DARK_PLANE)); FastCopyScreen(graybuffer+200*30-30*LCD_HEIGHT, GetPlane(LIGHT_PLANE)); spriteson = 1;}
		int collision = 0;
		int breakflag = 0;
		int i;
		if(_keytest (RR_2ND) && numobjects < 75 && putpress == 0)
		{
			collision = getcollision((*objectflags), (*objectx), (*objecty), *objx, *objy, offsetx, offsety);
			if(collision == 0) {
				int j;
				for(j=0; j<75; j++) {
					if((*objectflags)[j] == 0) {
						drawsprite(*objx, *objy, drawscreen, SPRT_XOR);
						(*objectx)[j] = *objx;
						(*objecty)[j] = *objy;
						(*objectflags)[j] = curritem;
						breakflag = 1;
						break;
					}
				}
			}
		}
		collision = 0;
		if(_keytest (RR_F3) && putpress == 0)
		{
			int save_offsetx = offsetx;
			int save_offsety = offsety;
			offsetx = 32;
			offsety = 13;
			collision = getcollision((*objectflags), (*objectx), (*objecty), *objx, *objy, offsetx, offsety);
			if(collision == 0) {
				for(i=65; i<75; i++) {
					if((*objectflags)[i] == 0) {
						(*objectflags)[i] = LeftFlipper;
						(*objectx)[i] = *objx;
						(*objecty)[i] = *objy;
						int save_curritem = curritem;
						curritem = LeftFlipper;
						drawsprite(*objx, *objy, drawscreen, SPRT_XOR);
						curritem = save_curritem;
						offsetx = save_offsetx;
						offsety = save_offsety;
						break;
					}
				}
			}
		}
		if(_keytest (RR_F4) && putpress == 0) {
			int save_offsetx = offsetx;
			int save_offsety = offsety;
			offsetx = 32;
			offsety = 13;
			collision = getcollision((*objectflags), (*objectx), (*objecty), *objx, *objy, offsetx, offsety);
			if(collision == 0) {
				for(i=65; i<75; i++) {
					if((*objectflags)[i] == 0) {
						(*objectflags)[i] = RightFlipper;
						(*objectx)[i] = *objx;
						(*objecty)[i] = *objy;
						int save_curritem = curritem;
						curritem = RightFlipper;
						drawsprite(*objx, *objy, drawscreen, SPRT_XOR);
						curritem = save_curritem;
						offsetx = save_offsetx;
						offsety = save_offsety;
						break;
					}
				}
			}
		}
		collision = 0;
		if(_keytest (RR_F5) && putpress == 0) {
			int save_offsetx = offsetx;
			int save_offsety = offsety;
			offsetx = 16;
			offsety = 24;
			collision = getcollision((*objectflags), (*objectx), (*objecty), *objx, *objy, offsetx, offsety);
			if(collision == 0 && launcherxy[0] == 0 && launcherxy[1] == 0) {
				launcherxy[0] = *objx;
				launcherxy[1] = *objy;
				int save_curritem = curritem;
				curritem = Launcher;
				drawsprite(*objx, *objy, drawscreen, SPRT_XOR);
				curritem = save_curritem;
				offsetx = save_offsetx;
				offsety = save_offsety;
			}
		}
		collision = 0;
		if(_keytest (RR_DIAMOND) && putpress == 0) {
			int save_offsetx = offsetx;
			int save_offsety = offsety;
			offsetx = 16;
			offsety = 16;
			collision = getcollision((*objectflags), (*objectx), (*objecty), *objx, *objy, offsetx, offsety);
			if(collision == 0 && cgravity[0] == 0 && cgravity[1] == 0) {
				cgravity[0] = *objx;
				cgravity[1] = *objy;
				int save_curritem = curritem;
				curritem = BlackHole;
				drawsprite(*objx, *objy, drawscreen, SPRT_XOR);
				curritem = save_curritem;
				offsetx = save_offsetx;
				offsety = save_offsety;
			}
		}
		collision = 0;
		breakflag = 0;
		int save_curritem = curritem;
		if(_keytest (RR_BCKSPC))
		{
			for(i=0; i<75; i++) {
				if((*objectflags)[i] != 0) {
					if((*objectflags)[i] == BigBumper && *objx + offsetx >= (*objectx)[i] && *objx <= (*objectx)[i] + 16 && *objy + offsety >= (*objecty)[i] && *objy <= (*objecty)[i] + 16) {
						collision = 1;
						curritem = BigBumper;
					}
					if((*objectflags)[i] == RectBumper && *objx + offsetx >= (*objectx)[i] && *objx <= (*objectx)[i] + 8 && *objy + offsety >= (*objecty)[i] && *objy <= (*objecty)[i] + 16) {
						collision = 1;
						curritem = RectBumper;
					}
					if((*objectflags)[i] == Accelerator && *objx + offsetx >= (*objectx)[i] && *objx <= (*objectx)[i] + 16 && *objy + offsety >= (*objecty)[i] && *objy <= (*objecty)[i] + 16) {
						collision = 1;
						curritem = Accelerator;
					}
					if((*objectflags)[i] == SmallBumper && *objx + offsetx >= (*objectx)[i] && *objx <= (*objectx)[i] + 8 && *objy + offsety >= (*objecty)[i] && *objy <= (*objecty)[i] + 8) {
						collision = 1;
						curritem = SmallBumper;
					}
					if((*objectflags)[i] == BigRightDeltoid && *objx + offsetx >= (*objectx)[i] && *objx <= (*objectx)[i] + 32 && *objy + offsety >= (*objecty)[i] && *objy <= (*objecty)[i] + 32) {
						collision = 1;
						curritem = BigRightDeltoid;
					}
					if((*objectflags)[i] == BigLeftDeltoid && *objx + offsetx >= (*objectx)[i] && *objx <= (*objectx)[i] + 32 && *objy + offsety >= (*objecty)[i] && *objy <= (*objecty)[i] + 32) {
						collision = 1;
						curritem = BigLeftDeltoid;
					}
					if((*objectflags)[i] == SmallRightDeltoid && *objx + offsetx >= (*objectx)[i] && *objx <= (*objectx)[i] + 16 && *objy + offsety >= (*objecty)[i] && *objy <= (*objecty)[i] + 16) {
						collision = 1;
						curritem = SmallRightDeltoid;
					}
					if((*objectflags)[i] == SmallLeftDeltoid && *objx + offsetx >= (*objectx)[i] && *objx <= (*objectx)[i] + 16 && *objy + offsety >= (*objecty)[i] && *objy <= (*objecty)[i] + 16) {
						collision = 1;
						curritem = SmallLeftDeltoid;
					}
					if((*objectflags)[i] == BinaryBumper && *objx + offsetx >= (*objectx)[i] && *objx <= (*objectx)[i] + 32 && *objy + offsety >= (*objecty)[i] && *objy <= (*objecty)[i] + 10) {
						collision = 1;
						curritem = BinaryBumper;
					}
					if((*objectflags)[i] == BallEater && *objx + offsetx >= (*objectx)[i] && *objx <= (*objectx)[i] + 16 && *objy + offsety >= (*objecty)[i] && *objy <= (*objecty)[i] + 12) {
						collision = 1;
						curritem = BallEater;
					}
					if((*objectflags)[i] == Teleporter && *objx + offsetx >= (*objectx)[i] && *objx <= (*objectx)[i] + 12 && *objy + offsety >= (*objecty)[i] && *objy <= (*objecty)[i] + 12) {
						collision = 1;
						curritem = Teleporter;
					}
					if((*objectflags)[i] == Flag && *objx + offsetx >= (*objectx)[i] && *objx <= (*objectx)[i] + 8 && *objy + offsety >= (*objecty)[i] && *objy <= (*objecty)[i] + 8) {
						collision = 1;
						curritem = Flag;
					}
					if((*objectflags)[i] == LeftFlipper && *objx + offsetx >= (*objectx)[i] && *objx <= (*objectx)[i] + 32 && *objy + offsety >= (*objecty)[i] && *objy <= (*objecty)[i]+13) {
						collision = 1;
						curritem = LeftFlipper;
					}
					if((*objectflags)[i] == RightFlipper && *objx + offsetx >= (*objectx)[i] && *objx <= (*objectx)[i] + 32 && *objy + offsety >= (*objecty)[i] && *objy <= (*objecty)[i]+13) {
						collision = 1;
						curritem = RightFlipper;
					}
				}
				if(collision == 1) {
					if((*objectflags)[i] != 0) drawsprite((*objectx)[i], (*objecty)[i], drawscreen, SPRT_XOR);
					(*objectflags)[i] = 0;
					(*objectx)[i] = 0;
					(*objecty)[i] = 0;
					curritem = save_curritem;
					breakflag = 1;
				}
			if(breakflag == 1) break;
			}
			if(launcherxy[0] != 0 && launcherxy[1] != 0 && *objx + offsetx >= launcherxy[0] && *objx <= launcherxy[0] + 16 && *objy + offsety >= launcherxy[1] - 2 && *objy <= launcherxy[1] + 24) {
				curritem = Launcher;
				drawsprite(launcherxy[0], launcherxy[1], drawscreen, SPRT_XOR);
				launcherxy[0] = 0;
				launcherxy[1] = 0;
				curritem = save_curritem;
			}
			if(cgravity[0] != 0 && cgravity[1] != 0 && *objx + offsetx >= cgravity[0] && *objx <= cgravity[0] + 16 && *objy + offsety >= cgravity[1] && *objy <= cgravity[1] + 16) {
				curritem = BlackHole;
				drawsprite(cgravity[0], cgravity[1], drawscreen, SPRT_XOR);
				cgravity[0] = 0;
				cgravity[1] = 0;
				curritem = save_curritem;
			}
		}
	}
	else if(currscreen == ScrollScreen)
	{
		short delay = 40000;
		while(delay != 0) {
			delay--;
		}
		if(delay == 0) {
			if(_keytest (RR_UP) && *selecty-32 > 0) {
				GraySprite8 (*selectx, *selecty, 8, selector, scrollscreen, SPRT_XOR);
				*selecty -= 32;
				GraySprite8 (*selectx, *selecty, 8, selector, scrollscreen, SPRT_XOR);
			}
			if(_keytest (RR_DOWN) && *selecty+32 < 128*3) {
				GraySprite8 (*selectx, *selecty, 8, selector, scrollscreen, SPRT_XOR);
				*selecty += 32;
				GraySprite8 (*selectx, *selecty, 8, selector, scrollscreen, SPRT_XOR);
			}
			short y = *selecty;
			short spriteson = 0;
			char *address = scrollscreen + 30*(y-(LCD_HEIGHT/2));
			if(*selecty+(LCD_HEIGHT/2) < 128*3 && *selecty-(LCD_HEIGHT/2) > 0)
			{
				FastCopyScreen(address,LCD_MEM);
				spriteson = 1;
			}
			else if(*selecty<(128*3)/2 && spriteson == 0)
			{
				FastCopyScreen(scrollscreen,LCD_MEM);
				spriteson = 1;
			}
			else if(*selecty>(128*3)/2 && spriteson == 0)
			{
				FastCopyScreen(scrollscreen+((30*3*128)-30*LCD_HEIGHT),LCD_MEM);
				spriteson = 1;
			}
			FontSetSys(F_4x6);
			PortSet(LCD_MEM, 239, 127);
			printf_xy(0,0,"Item Screen");
			if(_keytest (RR_2ND)) {
				GrayOn();
				PortSet(drawscreen, 239, 127+128);
				currscreen = DrawScreen;
				drawsprite(*objx, *objy, drawscreen, SPRT_XOR);
				curritem = (*selecty-12)/32 + 1;
				drawsprite(*objx, *objy, drawscreen, SPRT_XOR);
			}
		}
	}
}

void fixstring(char (*string)[])
{
	unsigned int len = strlen((*string)), i;
	for(i=0; i<len+1; i++) {
		if((*string)[i] == '\n') (*string)[i] = 0;
	}
}

void progmenu(short (*objectflags)[], short (*objectx)[], short (*objecty)[], short *objx, short *objy) {
	progmenu:
	ClrScr();
	nullallstrings();
	strncpy(title, " Developer Options ", 19);
	strncpy(str1, "Return", 6);
	strncpy(str2, "Save Table", 10);
	strncpy(str3, "Clear Table", 11);
	strncpy(str4, "Exit w/o Saving", 15);
	short r;
	menu(150, 55, 4, &r);
	if(r==2) {
		ClrScr();
		FontSetSys(F_6x8);
		make_menu_box(40,150,LCD_WIDTH/2-75,30);
		DrawStr(LCD_WIDTH/2-27,35,"Saving...",A_REVERSE);
		if(launcherxy[0] == 0 && launcherxy[1] == 0) {
			ClrScr();
			FontSetSys(F_6x8);
			make_menu_box(40,150,LCD_WIDTH/2-75,30);
			DrawStr(LCD_WIDTH/2-48,35,"Error in Saving!", A_REVERSE);
			FontSetSys(F_4x6);
			DrawStr(LCD_WIDTH/2-50,50,"You didn't place the launcher!", A_NORMAL);
			ngetchx();
			goto progmenu;
		}
		if(cgravity[0] == 0 && cgravity[1] == 0) {
			ClrScr();
			FontSetSys(F_6x8);
			make_menu_box(40,150,LCD_WIDTH/2-75,30);
			DrawStr(LCD_WIDTH/2-48,35,"Error in Saving!",A_REVERSE);
			FontSetSys(F_4x6);
			DrawStr(LCD_WIDTH/2-54,50,"You didn't place the Black Hole!", A_NORMAL);
			ngetchx();
			goto progmenu;
		}
		int i;
		SymDel(SYMSTR(tname));
		fixstring(&cname);
		fixstring(&pword);
		FILE *opentbl = fopen(tname, "wb");
		fprintf(opentbl, "tipinball table file=%s\n",tname);
		fprintf(opentbl, "Table=%s\n",cname);
		fprintf(opentbl, "Password=%s\n",pword);
		for(i=0; i<75; i++) {
			fprintf(opentbl, "objectflag=%d\n", (*objectflags)[i]);
			fprintf(opentbl, "objectx=%d\n", (*objectx)[i]);
			fprintf(opentbl, "objecty=%d\n", (*objecty)[i]);
		}
		fprintf(opentbl, "launcherx=%d\n", launcherxy[0]);
		fprintf(opentbl, "launchery=%d\n", launcherxy[1]);
		fprintf(opentbl, "cgravityx=%d\n", cgravity[0]);
		fprintf(opentbl, "cgravityy=%d\n", cgravity[1]);
		fputc(0,opentbl);
		fputs("TBL",opentbl);
		fputc(0,opentbl);
		fputc(OTH_TAG,opentbl);
		FontSetSys(F_6x8);
		DrawStr(LCD_WIDTH/2-69,50,"File Successfully Saved",A_NORMAL);
		ngetchx();
		fclose(opentbl);
		goto progmenu;
	}
	if(r==3) {
		nullallstrings();
		ClrScr();
		strncpy(title, " Are You Sure? ", 15);
		strncpy(str1, "Yes", 3);
		strncpy(str2, "No",2);
		short r;
		menu(150, 40, 2, &r);
		if(r==1) {
			int i;
			cgravity[0] = 0;
			cgravity[1] = 0;
			launcherxy[0] = 0;
			launcherxy[1] = 0;
			for(i=0; i<75; i++) {
				(*objectflags)[i] = 0;
				(*objectx)[i] = 0;
				(*objecty)[i] = 0;
			}
			PortSet(drawscreen, 239, 127+128);
			clearfullscreen();
			PortSet(drawscreen, 239, 127+128);
			curritem = BigBumper;
			drawsprite(*objx, *objy, drawscreen, SPRT_XOR);
			PortSet(LCD_MEM,239,127);
			ClrScr();
			FontSetSys(F_6x8);
			make_menu_box(40,150,LCD_WIDTH/2-75,30);
			DrawStr(LCD_WIDTH/2-36,45,"Table Erased",A_REVERSE);
			ngetchx();
			goto progmenu;
		}
		if(r==2) goto progmenu;
	}
	if(r==4) {
		SetIntVec(AUTO_INT_1, DUMMY_HANDLER);
		GrayOn();
		cleanup();
		exit(0);
	}
}

void _main(void)
{
	ClrScr();
	logo();
play_again:
	driver();
	goto play_again;
}

void startup(void)
{
	if(initilized == 0) {
		save_vec_auto_int_1 = GetIntVec(AUTO_INT_1);
		SetIntVec(AUTO_INT_1, DUMMY_HANDLER);
		if(grayflag == 1) GrayOn();
		scrollscreen = malloc(3*3840);
		if(!scrollscreen)
		{
			clrscr();
			FontSetSys(F_8x10);
			printf("Not Enough Memory");
			SetIntVec(AUTO_INT_1, save_vec_auto_int_1);
			ngetchx();
			exit(0);
		}
		drawscreen = malloc(2*3840);
		if(!drawscreen)
		{
			clrscr();
			FontSetSys(F_8x10);
			printf("Not Enough Memory");
			SetIntVec(AUTO_INT_1, save_vec_auto_int_1);
			free(scrollscreen);
			ngetchx();
			exit(0);
		}
		graybuffer = malloc(2*3840);
		if(!graybuffer)
		{
			clrscr();
			FontSetSys(F_8x10);
			printf("Not Enough Memory");
			SetIntVec(AUTO_INT_1, save_vec_auto_int_1);
			free(scrollscreen);
			free(drawscreen);
			ngetchx();
			exit(0);
		}
		initilized = 1;
		memset(scrollscreen, 0, 3*3840);
		memset(drawscreen, 0, 2*3840);
		memset(graybuffer, 0, 2*3840);
	}
}

void cleanup(void)
{	
	if(initilized == 1) {
		clearfullscreen();
		GrayOff();
		SetIntVec(AUTO_INT_1, save_vec_auto_int_1);
		PortSet(LCD_MEM, 239, 127);
		free(scrollscreen);
		free(drawscreen);
		free(graybuffer);
		initilized = 0;
	}
}

//this is the primary driver function, as it controls all of the other functions
//in the game.  All of the primary game variables are defined and declared here,
//and are passed to the other functions.
void driver(void)
{
	short objectflags[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	short objectx[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	short objecty[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	int trueflag = 1;
	launcherxy[0] = 0;
	launcherxy[1] = 0;
	cgravity[0] = 0;
	cgravity[1] = 0;
	short objx = 40, objy = 40;
	short selectx = 0, selecty = 12;
	sys_menu(&objectflags, &objectx, &objecty);
	currscreen = DrawScreen;
	initscreens(&objectflags, &objectx, &objecty);
	PortSet(drawscreen, 239, 127+128);
	drawsprite(objx, objy, drawscreen, SPRT_XOR);
	while(trueflag == 1) {
		if(!_keytest (RR_2ND) && !_keytest (RR_F3) && !_keytest (RR_F4) && !_keytest (RR_F5) && !_keytest (RR_DIAMOND)) putpress = 0;
		else putpress = 1;
		getkey(&objx, &objy, &selectx, &selecty, &objectflags, &objectx, &objecty);
		if(_keytest (RR_F1) && currscreen != DrawScreen) {
			currscreen = DrawScreen;
			PortSet(LCD_MEM, 239, 127);
			ClrScr();
			PortSet(GetPlane(LIGHT_PLANE),239,127);
			ClrScr();
			PortSet(drawscreen, 239, 127+128);
			if(grayflag == 1) GrayOn();
		}
		if(_keytest (RR_F2) && currscreen != ScrollScreen) {
			currscreen = ScrollScreen;
			PortSet(LCD_MEM, 239, 127);
			ClrScr();
			PortSet(scrollscreen, 239, 127+256);
			GrayOff();
		}
		if(_keytest (RR_MODE)) {
			PortSet(LCD_MEM, 239, 127);
			GrayOff();
			ClrScr();
			SetIntVec(AUTO_INT_1, save_vec_auto_int_1);
			progmenu(&objectflags, &objectx, &objecty, &objx, &objy);
			SetIntVec(AUTO_INT_1, DUMMY_HANDLER);
			ClrScr();
			GrayOn();
			PortSet(drawscreen, 239, 127+128);
		}
	}
	cleanup();
}