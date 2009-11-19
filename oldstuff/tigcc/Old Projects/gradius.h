// C Header File
// Created 27/Oct/03; 21:13:54
#ifndef _GRADIUS_H_
#define _GRADIUS_H_

/* Constants */
enum ArrowKeys {UP,DOWN,LEFT,RIGHT,SECOND,SHIFT,DIAMOND,ALPHA};
enum KeyMatrix {TI89_ESCROW = ~0x0040, TI89_ESCKEY = 0x0001, ARROW_ROW = ~0x0001};
enum rTypeConstants {R_TYPE_HT = 8, R_TYPE_WIDTH = 16};
enum laserConstants {LASER_HT = 8, LASER_WIDTH = 8};
enum missileConstants {MISSILE_HT = 8, MISSILE_WIDTH = 8};

/* Type Def'ns */
typedef struct {
	unsigned short int x : 8, y : 8;
} POSITION;

/* Sprite Def'ns */
// the laser
static unsigned char laserLight[] = {0x18,0x66,0x42,0x81,0x81,0x42,0x66,0x18};
static unsigned char laserMid[]   = {0x00,0x18,0x24,0x5A,0x5A,0x24,0x18,0x00};
static unsigned char laserDark[]  = {0x00,0x00,0x18,0x24,0x24,0x18,0x00,0x00};

// a missile that points down and right
static unsigned char missileLight[] = {0x28,0x18,0x04,0x16,0x0A,0x07,0x02,0x02};
static unsigned char missileMid[]   = {0xC4,0x00,0x12,0x00,0x11,0x08,0x05,0x05};

// a r-type ship 4 level grayscale sprite
static unsigned int r_type_light[] = {0x0000,0x8F00,0xAFF8,0x1F8E,0x6A02,0x6080,0x0000,0x0000};
static unsigned int r_type_mid[]   = {0x0000,0x2030,0x5004,0x6040,0x04FD,0x1747,0x0800,0x0000};
static unsigned int r_type_dark[]  = {0x0000,0x0000,0x0000,0x0030,0x0000,0x0000,0x0000,0x0000};

// flat ground
static unsigned char ground_flat[] = {0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

// ground like this >> 
static unsigned char ground_down[] = {0x80,0x40,0x20,0x10,0x08,0x04,0x02,0x01};

// ground like this >> /
static unsigned char ground_up[]   = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

/*// the various kinds of blocks in the game
static unsigned char ground1_dark[] = {0xFF,0x08,0x82,0x28,0x00,0x40,0x00,0x00};
static unsigned char ground1_light[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

static unsigned char block1_dark[] = {0x7E,0xC3,0x81,0x81,0x81,0x81,0xC3,0x7E};
static unsigned char block1_light[] = {0x7E,0xC3,0x9F,0xBF,0xBF,0xFF,0xFF,0x7E};*/

/* Function Prototypes */
void getKeyMasks(short int *);

inline void drawLaser(POSITION);
inline void eraseLaser(POSITION);
inline void moveLaser(POSITION, POSITION);
inline void drawRType(POSITION);
inline void eraseRType(POSITION);
inline void moveRType(POSITION, POSITION);
inline void drawMissile(POSITION);
inline void eraseMissile(POSITION);
inline void moveMissile(POSITION, POSITION);

short int blowWall(POSITION, int[][]);
short int detectWall(POSITION, int[][]);
void Draw_Map(int[][], POSITION, POSITION, int, POSITION, int);
short int quit(void);
void delay(void);

#endif