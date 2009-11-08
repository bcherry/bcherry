// C Source File
// Created 29/Nov/03; 21:11:39

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files
#include "extgraph.h"
//#include "JudeCN.h"
#include "met_lev.h"
#include "met_sprt.h"
#include "c_sprites.h"

/*============== Global Defines ===============*/
// put any #define stuff here or enumerations
#define GRAVITY 4
#define JUMP_POWER -3
#define DELAY 30
enum states {LEFT_REG,RIGHT_REG,LEFT_JUMP,RIGHT_JUMP,LEFT_FALL,RIGHT_FALL,LEFT_FLIP_1,LEFT_FLIP_2,LEFT_FLIP_3,LEFT_FLIP_4,RIGHT_FLIP_1,RIGHT_FLIP_2,RIGHT_FLIP_3,RIGHT_FLIP_4,
						 LEFT_RUN_1,LEFT_RUN_2,LEFT_RUN_3,LEFT_RUN_4,RIGHT_RUN_1,RIGHT_RUN_2,RIGHT_RUN_3,RIGHT_RUN_4,LEFT_KNEEL,RIGHT_KNEEL,LEFT_HURT,RIGHT_HURT,MORPH_1,MORPH_2,MORPH_3}; 
enum faces {NONE,LEFT,RIGHT,UP,DOWN,UP_LEFT,UP_RIGHT,DOWN_LEFT,DOWN_RIGHT};

/*=============== Global Types ================*/
// space for typedefs
typedef struct {
	int x_pos;
	int y_pos;
	int width;
	int height;
	int HP;
	void * sprites;
	// anything else i would need to know about an enemy
	// this is a general enemy structure, but i may decide
	// to have a seperate structure for each enemy type, such
	// as typedef struct {...} ENEMY_PIRATE or something
} ENEMY;

typedef struct {
	int x_pos;
	int y_pos;
	int width;
	int height;
	int type;
} TILE;

typedef struct {
	int x;
	int y;
	int x_o;
	int y_o;
} MAP_PTR;

typedef struct {
	char x_pos;
	char y_pos;
	char width;
	char height;	
	char face;
	char ground;
	char x_change;
	char falling;
	char state;
	char flip;
	char morph;
	char jumping;
	char energy_tanks_max;
	unsigned int energy_current;
	unsigned char missiles_max;
	unsigned char missiles_ammo;
	char charge_beam_got;
	char charger;
	char switcher;
	// this is the structure that defines samus aran's
	// attributes.  Again, this is hypothetical, and may
	// change.
} SAMUS;

/*============= Global Variables ==============*/
SAMUS samus = {0,0,16,18,RIGHT,0,0,1,RIGHT_FALL,0,0,0,0,99,0,0,0,0,0};
MAP_PTR ptr = {0,0,0,0};
TILE tile_stack[100];
int jump_power=3;
									 
/*=========== Function Prototypes =============*/
inline void drawSamus(void);
inline void loadBackground(void);
inline void pushTile(int,int,int,int,int,int);
inline void drawBackground(void);
inline void clearTileStack(void);
inline void drawTile(TILE);
inline void CLRSCR(int);
inline void spritesInit(void);
inline int samusHit(TILE);
inline void pushShot(void);
inline int collideTest(int,int,int,int,int,int,int,int);

/*========= The Interface Routines ===========*/
// routines for drawing menus, hi-scores, help, etc
short int quit(void) {
	if (_rowread(~0x40)&0x01) {
		return 1;
	}	
	return 0;
}

/*======= Setup and Closing Routines =========*/
//initialize and deinitialize grayscale, the interrupts, etc
inline void CLRSCR(int num) {
	if(num>1){
		clrscr();
		GraySetAMSPlane(LIGHT_PLANE);
		clrscr();
		GraySetAMSPlane(DARK_PLANE);
		clrscr();
	}
	if (num==1||num==3){
		memset(light_vscreen, 0x00, 3840);
		memset(dark_vscreen, 0x00, 3840);
		memset(light_buff, 0x00, 3840);
		memset(dark_buff, 0x00, 3840);
	}
}

// delay program
inline void delay(unsigned int number) {
	unsigned short loop, loop2;
	for (loop=0;loop<number;loop++) {
		for (loop2=0;loop2<10000;loop2++);
	}
}
/*========== The Sprite Routines =============*/
// guess what goes here! YOURE RIGHT!!! the sprite drawing functions
inline void drawSamus(void) {		
	if (samus.state == RIGHT_REG) {
		unsigned short samus_sprt[] = {0x0000,0x0180,0x0780,0x0440,0x06C0,0x0640,0x0740,0x02EC,0x01FC,0x02C0,0x0060,0x0040,0x02D0,0x0280,0x0400,0x0D40,0x0140,0x1140,
									 								 0x0000,0x0180,0x0780,0x06C0,0x0440,0x07C0,0x06E0,0x024C,0x02FC,0x0040,0x02C0,0x00E0,0x0370,0x05E0,0x02C0,0x0DC0,0x1C80,0x1D80};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == LEFT_REG) {			
		unsigned short samus_sprt[] = {0x0000,0x0180,0x01E0,0x0220,0x0360,0x0260,0x02E0,0x3740,0x3F80,0x0340,0x0600,0x0200,0x0B40,0x0140,0x0020,0x02B0,0x0280,0x0288,
									 								 0x0000,0x0180,0x01E0,0x0360,0x0220,0x03E0,0x0760,0x3240,0x3F40,0x0200,0x0340,0x0700,0x0EC0,0x07A0,0x0340,0x03B0,0x0138,0x01B8};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == RIGHT_JUMP) {
		unsigned short samus_sprt[] = {0x0180,0x01C0,0x06C0,0x0240,0x0BB0,0x07E8,0x07C0,0x0340,0x0040,0x02D0,0x01A0,0x01C0,0x02C0,0x00A0,0x0400,0x0300,0x0600,0x0200,
									 								 0x01C0,0x02C0,0x0620,0x03E0,0x07F0,0x0FD8,0x0760,0x03B0,0x02B0,0x03C0,0x01E0,0x01C0,0x0380,0x0380,0x0600,0x0600,0x0400,0x0200};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == LEFT_JUMP) {
		unsigned short samus_sprt[] = {0x0180,0x0380,0x0360,0x0240,0x0DD0,0x17E0,0x03E0,0x02C0,0x0200,0x0B40,0x0580,0x0380,0x0340,0x0500,0x0020,0x00C0,0x0060,0x0040,
									 								 0x0380,0x0340,0x0460,0x07C0,0x0FE0,0x1BF0,0x06E0,0x0DC0,0x0D40,0x03C0,0x0780,0x0380,0x01C0,0x01C0,0x0060,0x0060,0x0020,0x0040};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == RIGHT_FALL) { 
		unsigned short samus_sprt[] = {0x0000,0x0180,0x03C0,0x07C0,0x05C0,0x07C0,0x07C0,0x07C0,0x03F0,0x0308,0x01F8,0x01E0,0x03E0,0x0A60,0x00A0,0x0060,0x0400,0x0000,
									 								 0x0000,0x01C0,0x03C0,0x0400,0x0640,0x07C0,0x00C0,0x04C0,0x0300,0x03F0,0x01C8,0x01D0,0x02A0,0x0EC0,0x0660,0x0440,0x0400,0x0000};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == LEFT_FALL) {
		unsigned short samus_sprt[] = {0x0000,0x0180,0x03C0,0x03E0,0x03A0,0x03E0,0x03E0,0x03E0,0x0FC0,0x10C0,0x1F80,0x0780,0x07C0,0x0650,0x0500,0x0600,0x0020,0x0000,
									 								 0x0000,0x0380,0x03C0,0x0020,0x0260,0x03E0,0x0300,0x0320,0x00C0,0x0FC0,0x1380,0x0B80,0x0540,0x0370,0x0660,0x0220,0x0020,0x0000};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == RIGHT_FLIP_1) {
		unsigned short samus_sprt[] = {0x0000,0x0000,0x0000,0x0000,0x0180,0x0B40,0x0B70,0x0678,0x07B8,0x05A4,0x01F0,0x00C4,0x007C,0x00C8,0x0000,0x0000,0x0000,0x0000,
									 								 0x0000,0x0000,0x0000,0x0000,0x0180,0x0EC0,0x16F0,0x03F8,0x057C,0x051C,0x01E4,0x01B4,0x00BC,0x00F8,0x0000,0x0000,0x0000,0x0000};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == RIGHT_FLIP_2) {
		unsigned short samus_sprt[] = {0x0000,0x0000,0x0000,0x0000,0x0600,0x0800,0x1380,0x05C0,0x11D0,0x1E20,0x0EE0,0x1F18,0x0330,0x00A8,0x0000,0x0000,0x0000,0x0000,
									 								 0x0000,0x0000,0x0000,0x0000,0x0600,0x0B00,0x1E80,0x13C0,0x07F0,0x12C0,0x1FF0,0x17E8,0x0234,0x0168,0x0000,0x0000,0x0000,0x0000};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == RIGHT_FLIP_3) {
		unsigned short samus_sprt[] = {0x0000,0x0000,0x0000,0x0000,0x02D0,0x0388,0x07A4,0x0694,0x0270,0x1D70,0x1120,0x0880,0x0600,0x0A00,0x0400,0x0000,0x0000,0x0000,
									 								 0x0000,0x0000,0x0000,0x0000,0x03B0,0x0118,0x0754,0x03FC,0x0F68,0x12F0,0x1E60,0x0F40,0x0E80,0x0E00,0x0400,0x0000,0x0000,0x0000};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == RIGHT_FLIP_4) {
		unsigned short samus_sprt[] = {0x0000,0x0000,0x0000,0x0000,0x0000,0x0018,0x01E0,0x0178,0x01DC,0x0FC4,0x1F30,0x15F0,0x0060,0x0960,0x0380,0x0000,0x0000,0x0000,
									 								 0x0000,0x0000,0x0000,0x0000,0x0010,0x0008,0x01D8,0x0038,0x03E4,0x0B3C,0x1F60,0x1E70,0x0FE0,0x0CE0,0x0380,0x0000,0x0000,0x0000};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == LEFT_FLIP_1) {
		unsigned short samus_sprt[] = {0x0000,0x0000,0x0000,0x0000,0x0180,0x02D0,0x0ED0,0x1E60,0x1DE0,0x25A0,0x0F80,0x2300,0x3E00,0x1300,0x0000,0x0000,0x0000,0x0000,
									 								 0x0000,0x0000,0x0000,0x0000,0x0180,0x0370,0x0F68,0x1FC0,0x3EA0,0x38A0,0x2780,0x2D80,0x3D00,0x1F00,0x0000,0x0000,0x0000,0x0000};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == LEFT_FLIP_2) {
		unsigned short samus_sprt[] = {0x0000,0x0000,0x0000,0x0000,0x0060,0x0010,0x01C8,0x03A0,0x0B88,0x0478,0x0770,0x18F8,0x0CC0,0x1500,0x0000,0x0000,0x0000,0x0000,
									 								 0x0000,0x0000,0x0000,0x0000,0x0060,0x00D0,0x0178,0x03C8,0x0FE0,0x0348,0x0FF8,0x17E8,0x2C40,0x1680,0x0000,0x0000,0x0000,0x0000};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == LEFT_FLIP_3) {
		unsigned short samus_sprt[] = {0x0000,0x0000,0x0000,0x0000,0x0B40,0x11C0,0x25E0,0x2960,0x0E40,0x0EB8,0x0488,0x0110,0x0060,0x0050,0x0020,0x0000,0x0000,0x0000,
									 								 0x0000,0x0000,0x0000,0x0000,0x0DC0,0x1880,0x2AE0,0x3FC0,0x16F0,0x0F48,0x0678,0x02F0,0x0170,0x0070,0x0020,0x0000,0x0000,0x0000};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == LEFT_FLIP_4) {
		unsigned short samus_sprt[] = {0x0000,0x0000,0x0000,0x0000,0x0000,0x1800,0x0780,0x1E80,0x3B80,0x23F0,0x0CF8,0x0FA8,0x0600,0x0690,0x01C0,0x0000,0x0000,0x0000,
									 								 0x0000,0x0000,0x0000,0x0000,0x0800,0x1000,0x1B80,0x1C00,0x27C0,0x3CD0,0x06F8,0x0E78,0x07F0,0x0730,0x01C0,0x0000,0x0000,0x0000};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == RIGHT_RUN_2) {
		unsigned short samus_sprt[] = {0x0000,0x0100,0x0400,0x0E40,0x0700,0x05C0,0x0F7F,0x07BF,0x0180,0x0440,0x0480,0x02C0,0xE440,0xE030,0xE000,0xC038,0x8018,0x0012,
									 								 0x0000,0x0180,0x0780,0x0C40,0x0CC0,0x1F20,0x0F90,0x07FF,0x02C0,0x0480,0x01C0,0x04F0,0xE470,0xBC10,0x8030,0x8028,0x8010,0x000E};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
		//samus.state=RIGHT_RUN_2;
	} else if (samus.state == RIGHT_RUN_3) {
		unsigned short samus_sprt[] = {0x0000,0x0180,0x0240,0x0040,0x0140,0x06C0,0x07CF,0x03DF,0x12C0,0x18C0,0x56E0,0x56F0,0x0200,0x0420,0x0010,0x0020,0x001C,0x0004,
									 								 0x0000,0x01C0,0x03C0,0x0280,0x0680,0x0760,0x0793,0x02EF,0x1240,0x0A40,0x7DC0,0x5DD0,0x0630,0x0610,0x0020,0x0030,0x0014,0x001C};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
		//samus.state=RIGHT_RUN_3;
	} else if (samus.state == RIGHT_RUN_4) {
		unsigned short samus_sprt[] = {0x0600,0x0800,0x0100,0x1100,0x1D00,0x1C0C,0x0E7C,0x0A00,0x0300,0x0A00,0x1D00,0x1480,0x2C80,0x2200,0x0800,0x0C00,0x0000,0x0600,
									 								 0x0600,0x0F00,0x0D00,0x1B00,0x1E80,0x0E7C,0x0BBC,0x0E00,0x0900,0x0900,0x0E80,0x1B80,0x2700,0x2600,0x0C00,0x0E00,0x0C00,0x0600};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	//	samus.state=RIGHT_RUN_4;
	} else if (samus.state == RIGHT_RUN_1) {
		unsigned short samus_sprt[] = {0x0100,0x05C0,0x0280,0x0040,0x05E0,0x0703,0x07BF,0x0400,0x00C0,0x0400,0x06F0,0x04A0,0x00C0,0x20C0,0x30C0,0x6060,0x3000,0x1800,
									 								 0x0180,0x07C0,0x0500,0x04C0,0x06B0,0x073F,0x07DF,0x0700,0x0400,0x0720,0x0ED0,0x1CA0,0x18C0,0x3140,0x6080,0x4060,0x2000,0x1800};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
		//samus.state=RIGHT_RUN_1;
	} else if (samus.state == LEFT_RUN_2) {
		//samus.state=LEFT_RUN_2;
		unsigned short samus_sprt[] = {0x0000,0x0080,0x0020,0x0270,0x00E0,0x03A0,0xFEF0,0xFDE0,0x0180,0x0220,0x0120,0x0340,0x0227,0x0C07,0x0007,0x1C03,0x1801,0x4800,
									 								 0x0000,0x0180,0x01E0,0x0230,0x0330,0x04F8,0x09F0,0xFFE0,0x0340,0x0120,0x0380,0x0F20,0x0E27,0x083D,0x0C01,0x1401,0x0801,0x7000};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == LEFT_RUN_3) {
		unsigned short samus_sprt[] = {0x0000,0x0180,0x0240,0x0200,0x0280,0x0360,0xF3E0,0xFBC0,0x0348,0x0318,0x076A,0x0F6A,0x0040,0x0420,0x0800,0x0400,0x3800,0x2000,
									 								 0x0000,0x0380,0x03C0,0x0140,0x0160,0x06E0,0xC9E0,0xF740,0x0248,0x0250,0x03BE,0x0BBA,0x0C60,0x0860,0x0400,0x0C00,0x2800,0x3800};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
		//samus.state=LEFT_RUN_3;
	} else if (samus.state == LEFT_RUN_4) {
		unsigned short samus_sprt[] = {0x0060,0x0010,0x0080,0x0088,0x00B8,0x3038,0x3E70,0x0050,0x00C0,0x0050,0x00B8,0x0128,0x0134,0x0044,0x0010,0x0030,0x0000,0x0060,
									 								 0x0060,0x00F0,0x00B0,0x00D8,0x0178,0x3E70,0x3DD0,0x0070,0x0090,0x0090,0x0170,0x01D8,0x00E4,0x0064,0x0030,0x0070,0x0030,0x0060};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	//	samus.state=LEFT_RUN_4;
	} else if (samus.state == LEFT_RUN_1) {
		unsigned short samus_sprt[] = {0x0080,0x03A0,0x0140,0x0200,0x07A0,0xC0E0,0xFDE0,0x0020,0x0300,0x0020,0x0F60,0x0520,0x0300,0x0304,0x030C,0x0606,0x000C,0x0018,
									 								 0x0180,0x03E0,0x00A0,0x0320,0x0D60,0xFCE0,0xFBE0,0x00E0,0x0020,0x04E0,0x0B70,0x0538,0x0318,0x028C,0x0106,0x0602,0x0004,0x0018};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	//	samus.state=LEFT_RUN_1;
	} else if (samus.state == RIGHT_KNEEL) {
		unsigned short samus_sprt[] = {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0700,0x0180,0x0980,0x0880,0x0CC0,0x07D0,0x0798,0x04F0,0x0120,0x0C40,0x0BC0,0x0F80,
									 								 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0780,0x0780,0x0A80,0x0F80,0x07C0,0x04F0,0x07F8,0x01F0,0x04E0,0x0EC0,0x0F40,0x0FE0};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == LEFT_KNEEL) {
		unsigned short samus_sprt[] = {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x00E0,0x0180,0x0190,0x0110,0x0330,0x0BE0,0x19E0,0x0F20,0x0480,0x0230,0x03D0,0x01F0,
									 								 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x01E0,0x01E0,0x0150,0x01F0,0x03E0,0x0F20,0x1FE0,0x0F80,0x0720,0x0370,0x02F0,0x07F0};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == RIGHT_HURT) {
		unsigned short samus_sprt[] = {0x0000,0x00E0,0x0208,0x07E8,0x0DE8,0x02B0,0x0750,0x0370,0x09D0,0x0FB0,0x07E0,0x0140,0x0290,0x0598,0x0538,0x0810,0x0000,0x0000,
									 								 0x0000,0x0080,0x01D0,0x0010,0x0390,0x0540,0x0020,0x0090,0x0620,0x0200,0x0000,0x0480,0x0120,0x0260,0x0200,0x0020,0x0020,0x0000};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == LEFT_HURT) {
		unsigned short samus_sprt[] = {0x0000,0x0700,0x1040,0x17E0,0x17B0,0x0D40,0x0AE0,0x0EC0,0x0B90,0x0DF0,0x07E0,0x0280,0x0940,0x19A0,0x1CA0,0x0810,0x0000,0x0000,
									 								 0x0000,0x0100,0x0B80,0x0800,0x09C0,0x02A0,0x0400,0x0900,0x0460,0x0040,0x0000,0x0120,0x0480,0x0640,0x0040,0x0400,0x0400,0x0000};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,light_vscreen,dark_vscreen,SPRT_XOR);
	} else if (samus.state == MORPH_1) {
		unsigned short samus_sprt[] = {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0080,0x0240,0x01E0,0x0200,0x0220,0x0140,
									 								 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x03C0,0x05A0,0x0610,0x05D0,0x0420,0x0220};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,GetPlane(LIGHT_PLANE),GetPlane(DARK_PLANE),SPRT_XOR);
	} else if (samus.state == MORPH_2) {
		unsigned short samus_sprt[] = {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0580,0x02C0,0x0360,0x0020,0x0140,
									 								 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x03C0,0x0660,0x0530,0x0490,0x0480,0x02A0};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,GetPlane(LIGHT_PLANE),GetPlane(DARK_PLANE),SPRT_XOR);
	} else if (samus.state == MORPH_3) {
		unsigned short samus_sprt[] = {0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x01C0,0x05C0,0x0640,0x01C0,0x0180,
									 								 0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x03C0,0x0420,0x0030,0x05B0,0x0420,0x0260};
		m_Sprite16(samus.x_pos,samus.y_pos,18,samus_sprt,GetPlane(LIGHT_PLANE),GetPlane(DARK_PLANE),SPRT_XOR);
	}	
}

/*=========== The Stack Routines =============*/
inline void putScreen(void) {
	CLRSCR(1);
	loadBackground();
	drawSamus();
	// draw enemies and everything else
	FastCopyScreen(light_vscreen,GetPlane(LIGHT_PLANE));
	FastCopyScreen(dark_vscreen,GetPlane(DARK_PLANE));	
}
inline void loadBackground(void) {
	//clearTileStack();
	int x_temp = ptr.x;
	int y_temp = ptr.y;
	int ii, i, x=0, y=0, place = 0;
	for (ii = y_temp; ii <= y_temp + 13; ii++) {
		for (i = x_temp + 40 * ii; i <= x_temp + 40 * ii + 21; i++) {
			if (tiles_1[i] != EMPTY) {pushTile(x-ptr.x_o,y-ptr.y_o,8,8,tiles_1[i],place);place++;/*drawTile((TILE){x,y,8,8,tiles_1[i]});*/}
			x+=8;
			//if (x >= 160) {x=0;y+=8;}
		}
		y+=8;
		x=0;
	}
	drawBackground();
}

inline void drawTile(TILE tile) {
	switch (tile.type) {
			case BLOCK:
				m_Sprite8(tile.x_pos,tile.y_pos,8,block,light_vscreen,dark_vscreen,SPRT_XOR);
				break;
			// more cases here
			//...
			//...
	}
}

inline void pushTile(int x, int y, int w, int h, int type, int place) {
	tile_stack[place] = (TILE){x,y,w,h,type};
}

inline void drawBackground(void) {
	int i, done = 0;
	for (i=0;i<=100;i++) {
		switch (tile_stack[i].type) {
			case BLOCK:
				m_Sprite8(tile_stack[i].x_pos,tile_stack[i].y_pos,8,block,/*GetPlane(LIGHT_PLANE)*/light_vscreen,/*GetPlane(DARK_PLANE)*/dark_vscreen,SPRT_XOR);
				//Sprite8(tile_stack[i].x_pos,tile_stack[i].y_pos,8,block,LCD_MEM,SPRT_XOR);
				break;
			case EMPTY:
				done = 1;
				break;
		}
	if (done) break;
	}
}

inline void clearTileStack(void) {
	int i;
	for (i = 0; i == 100; i++) {
		if (tile_stack[i].type == EMPTY) break;
		tile_stack[i] = (TILE){0,0,0,0,0};
	}
}

inline void pushShot(void){
	//push a shot to the stack
}

inline int samusHit(TILE t){	
	if (collideTest(t.x_pos,t.y_pos,t.height,t.width,samus.x_pos+samus.x_change,samus.y_pos+(samus.jumping>0)*jump_power+(samus.falling!=0)*GRAVITY,samus.height,samus.width)){//hits a tile in the end
		if (samus.falling&&collideTest(t.x_pos,t.y_pos,t.height,t.width,samus.x_pos,samus.y_pos+GRAVITY,samus.height,samus.width))return DOWN;
		if (samus.jumping&&collideTest(t.x_pos,t.y_pos,t.height,t.width,samus.x_pos,samus.y_pos+jump_power,samus.height,samus.width))return UP;
		if (samus.x_change&&collideTest(t.x_pos,t.y_pos,t.height,t.width,samus.x_pos+samus.x_change,samus.y_pos,samus.height,samus.width)){if(samus.x_change>0)return RIGHT; else if(samus.x_change<0)return LEFT;}
		if (samus.falling&&collideTest(t.x_pos,t.y_pos,t.height,t.width,samus.x_pos+samus.x_change,samus.y_pos+GRAVITY,samus.height,samus.width)){if(samus.x_change>0)return DOWN_RIGHT; else if(samus.x_change<0)return DOWN_LEFT;}
		if (samus.jumping&&collideTest(t.x_pos,t.y_pos,t.height,t.width,samus.x_pos+samus.x_change,samus.y_pos+jump_power,samus.height,samus.width)){if(samus.x_change>0)return UP_RIGHT; else if(samus.x_change<0)return UP_LEFT;}
	}
	return 0;
}

inline int collideTest(int x0, int y0, int h0, int w0, int x1, int y1, int h1, int w1){
	if(x0<x1+w1&&x0+w0>x0&&y0+h0>y1&&y0<y1+h1) return 1;
	return 0;
}
/*=========== THE MAIN FUNCTION!!! ===========*/
void _main(void) 
{
	
	//initializations
	samus = (SAMUS){0,0,16,18,RIGHT,0,0,1,RIGHT_FALL,0,0,0,0,99,0,0,0,0,0};
	ptr = (MAP_PTR){0,0,0,0};
	INT_HANDLER interrupt1;
	int i;
	int dir;
	//int test_ground=0;
	//int test_falling=1;
	SAMUS test_samus={0,0,16,18,RIGHT,0,0,1,RIGHT_FALL,0,0,0,0,99,0,0,0,0,0};
	//int hit_bottom=0;
	interrupt1 = GetIntVec(AUTO_INT_1);
	SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
	GraySpriteInit(3840);
	CLRSCR(3);
	//menu goes here
	
	
	//this starts the overall loop
	while (!quit()/*esc*/) {
		//area switching stuff goes one here
		//then the real action begins!
		while (!quit()/*esc*/) {
			//draw the screen:
			putScreen();
			// collision detections
			
			if(samus.falling){if(samus.face==LEFT)samus.state=LEFT_FALL;else if(samus.face==RIGHT)samus.state=RIGHT_FALL;}
			if(samus.ground&&!samus.x_change&&!samus.falling){if(samus.face==LEFT)samus.state=LEFT_REG;if(samus.face==RIGHT)samus.state=RIGHT_REG;}
			if(samus.jumping&&!samus.flip){if(samus.face==LEFT)samus.state=LEFT_JUMP;if(samus.face==RIGHT)samus.state=RIGHT_JUMP;}
			//if(samus.flip){if(samus.state++==LEFT_FLIP_3)samus.state=LEFT_FLIP_1;else if(samus.state++==RIGHT_FLIP_3)samus.state=RIGHT_FLIP_1;}
			//if(samus.ground&&samus.x_change>0){samus.face=RIGHT;if(samus.state++==RIGHT_RUN_4)samus.state=RIGHT_RUN_1;}
			//if(samus.ground&&samus.x_change<0){samus.face=LEFT;if(samus.state++==LEFT_RUN_4)samus.state=LEFT_RUN_1;}	
			//the movement/key reading routines
			if(/*!samus.jumping&&!samus.falling*/samus.ground)samus.x_change/=2;//this is so that if you are moving horizontally, you decelerate.  this will be undone later if you press the direction
			if(samus.x_change<2&&samus.x_change>-2)samus.x_change=0;
			if(_rowread(~0x01)&0x02/*left*/){
				if(samus.ground&&!samus.jumping&&!samus.falling&&samus.face==LEFT){
					if(samus.state<LEFT_RUN_1||samus.state>LEFT_RUN_4){samus.state=LEFT_RUN_1;samus.switcher=0;}
					else if(samus.state>=LEFT_RUN_1&&samus.state<LEFT_RUN_4){samus.switcher++;if(samus.switcher>1){samus.state++;samus.switcher=0;}}
					else if(samus.state==LEFT_RUN_4){samus.switcher++;if(samus.switcher>1){samus.state=LEFT_RUN_1;samus.switcher=0;}}
				} else if(!samus.flip&&(samus.jumping||samus.falling)){samus.face=LEFT;samus.state=LEFT_FALL;}
				if(samus.face==RIGHT&&!samus.jumping&&!samus.falling){samus.face=LEFT;samus.x_change=0;}
				//else if(samus.x_change>(-6*(!(_rowread(~0x01)&0x80/*alpha*/))-10*(_rowread(~0x01)&0x80/*alpha*/)))samus.x_change-=2;
				else if(!(_rowread(~0x01)&0x80)&&samus.x_change>-6)samus.x_change-=2;
				else if(_rowread(~0x01)&0x80&&samus.x_change>-10)samus.x_change-=2;
			}
			if(_rowread(~0x01)&0x08/*right*/){
				if(samus.ground&&!samus.jumping&&!samus.falling&&samus.face==RIGHT){
					if(samus.state<RIGHT_RUN_1||samus.state>RIGHT_RUN_4){samus.state=RIGHT_RUN_1;samus.switcher=0;}
					else if(samus.state>=RIGHT_RUN_1&&samus.state<RIGHT_RUN_4){samus.switcher++;if(samus.switcher>1){samus.state++;samus.switcher=0;}}
					else if(samus.state==RIGHT_RUN_4){samus.switcher++;if(samus.switcher>1){samus.state=RIGHT_RUN_1;samus.switcher=0;}}
				} else if(!samus.flip&&(samus.jumping||samus.falling)){samus.face=RIGHT;samus.state=RIGHT_FALL;}
				if(samus.face==LEFT&&!samus.jumping&&!samus.falling){samus.face=RIGHT;samus.x_change=0;}
				//else if(samus.x_change>(6*(!(_rowread(~0x01)&0x80/*alpha*/)))+(10*(_rowread(~0x01)&0x80/*alpha*/)))samus.x_change+=2;
				else if(_rowread(~0x01)&0x80&&samus.x_change<10)samus.x_change+=2;
				else if(/*!(_rowread(~0x01)&0x80)&&*/samus.x_change<6)samus.x_change+=2;				
			}
			if(_rowread(~0x01)&0x10/*2nd*/)pushShot();//shooting or bombing
			if(_rowread(~0x01)&0x40/*diamond*/){//jumping
				if(samus.ground&&!samus.x_change&&!samus.jumping&&!samus.falling){samus.jumping=1;samus.ground=0;samus.flip=0;}
				else if(samus.ground&&!samus.jumping&&!samus.falling&&samus.x_change){samus.jumping=1;samus.flip=1;samus.ground=0;}
				else if(samus.jumping){samus.jumping+=1;}				
				/*if(samus.jumping)samus.jumping++;
				if(samus.ground){
					samus.jumping=1;
					if(samus.x_change){
							if(samus.face==LEFT)samus.state=LEFT_FLIP_1;
							if(samus.face==RIGHT)samus.state=RIGHT_FLIP_1;
							samus.flip=1;
					}
				}*/
			}
			if(!(_rowread(~0x01)&0x40&&samus.jumping&&!samus.flip)){samus.falling=1;samus.jumping=0;}
			if(samus.jumping>10){samus.jumping=0;samus.falling=1;}
			if(samus.flip){
				if(samus.face==LEFT){
					if(samus.state<LEFT_FLIP_1||samus.state>LEFT_FLIP_4)samus.state=LEFT_FLIP_1;
					else if(samus.state>=LEFT_FLIP_1&&samus.state<LEFT_FLIP_4)samus.state+=1;
					else if(samus.state==LEFT_FLIP_4)samus.state=LEFT_FLIP_1;
				} else if(samus.face==RIGHT){
					if(samus.state<RIGHT_FLIP_1||samus.state>RIGHT_FLIP_4)samus.state=RIGHT_FLIP_1;
					else if(samus.state>=RIGHT_FLIP_1&&samus.state<RIGHT_FLIP_4)samus.state+=1;
					else if(samus.state==RIGHT_FLIP_4)samus.state=RIGHT_FLIP_1;
				}
			}
			//run through each tile in the tile stack
			test_samus=samus;
			//test_samus.falling=1;
			//test_samus.ground=0;
			//samus.falling=1;//its easier to disprove stuff than to prove it.  So we start assuming she is falling, and
			//samus.ground=0;//if she hits the ground, than she is not falling
			if(samus.jumping)test_samus.falling=0;
			for (i=0;i<=100;i++){
				if(tile_stack[i].type==EMPTY)break;
				dir=samusHit(tile_stack[i]);
				if(dir==RIGHT||dir==UP_RIGHT/*||dir==DOWN_RIGHT*/){
					test_samus.x_change=0;
					test_samus.x_pos=tile_stack[i].x_pos-samus.width;
					if(samus.state>=RIGHT_RUN_1&&samus.state<=RIGHT_RUN_4)samus.state=RIGHT_REG;
					//ptr.x_o+=(test_samus.x_pos-samus.x_pos);					
				}
				if(dir==LEFT||dir==UP_LEFT||dir==DOWN_LEFT){
					test_samus.x_change=0;
					test_samus.x_pos=tile_stack[i].x_pos+tile_stack[i].width;
					if(samus.state>=LEFT_RUN_1&&samus.state<=LEFT_RUN_4)samus.state=LEFT_REG;
					//ptr.x_o+=(test_samus.x_pos-samus.x_pos);
				}
				if(dir==DOWN||dir==DOWN_LEFT||dir==DOWN_RIGHT){
					if(!samus.ground)test_samus.x_change=0;		
					test_samus.flip=0;
					test_samus.falling=0;
					test_samus.ground=1;
					test_samus.y_pos=tile_stack[i].y_pos-samus.height;
					//ptr.y_o+=(test_samus.y_pos-samus.y_pos);
					//hit_bottom=1;
				}
				if(dir==UP||dir==UP_LEFT||dir==UP_RIGHT){
					if(samus.jumping){test_samus.jumping=0;test_samus.falling=1;}
					test_samus.y_pos=tile_stack[i].y_pos+tile_stack[i].height;
					//ptr.y_o+=(test_samus.y_pos-samus.y_pos);
				}
				tile_stack[i]=(TILE){0,0,0,0,EMPTY};
			}
			//test_falling=0;
			samus=test_samus;
			//if(!hit_bottom&&!samus.jumping)samus.falling=1;
			samus.x_pos+=samus.x_change;
			ptr.x_o+=samus.x_change;
			if(samus.jumping<10)jump_power=-1;
			if(samus.jumping<7)jump_power=-2;
			if(samus.jumping<4)jump_power=-3;
			if(samus.jumping){samus.y_pos+=jump_power;ptr.y_o+=jump_power;}
			if(samus.falling){samus.y_pos+=GRAVITY;ptr.y_o+=GRAVITY;}
			while(ptr.x_o>=8){//all of the screen scrolling stuff
				ptr.x_o-=8;
				ptr.x++;
			}
			while(ptr.x_o<0){
				ptr.x_o+=8;
				ptr.x--;
			}
			while(ptr.y_o>=8){
				ptr.y_o-=8;
				ptr.y++;
			}
			while(ptr.y_o<0){
				ptr.y_o+=8;
				ptr.y--;
			}            
			if(ptr.y<0)ptr.y=0;
			if(ptr.y>=8)ptr.y=8;
			if(ptr.x<0)ptr.x=0;
			if(ptr.x>=20)ptr.x=20;
			//the morphing stuff would go here maybe
			/*if(samus.x_pos>110&&ptr.x<20)ptr.x++;
			if(samus.x_pos<40&&ptr.x>0)ptr.x--;
			if(samus.y_pos>60&&ptr.y<8)ptr.y++;
			if(samus.y_pos<40&&ptr.y>0)ptr.y++;*/
			delay(DELAY);
		}
	}			
	GraySpriteDeInit();
	SetIntVec(AUTO_INT_1,interrupt1);
}	
