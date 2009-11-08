// C Source File
// Created 22/Oct/03; 17:22:16

// grayscale program

// compile only for the 89
#define USE_TI89

#define SAVE_SCREEN

#include <tigcclib.h>

void _main(void) {

	unsigned char out[] = {0x3C,0x42,0x81,0x81,0x81,0x81,0x42,0x3c};
	unsigned char mid[] = {0x00,0x3C,0x7E,0x66,0x66,0x7E,0x3C,0x00};
	unsigned char ins[] = {0x00,0x00,0x00,0x18,0x18,0x00,0x00,0x00};
	
	if (!GrayOn())
		return;
	GraySetAMSPlane (LIGHT_PLANE);
	ClrScr();
	Sprite8(20,20,8,out,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
	Sprite8(20,20,8,ins,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
	Sprite8(40,20,8,out,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
	Sprite8(40,20,8,ins,GrayGetPlane(LIGHT_PLANE),SPRT_XOR);
	GraySetAMSPlane (DARK_PLANE);
	ClrScr();
	Sprite8(20,20,8,out,GrayGetPlane(DARK_PLANE),SPRT_XOR);
	Sprite8(20,20,8,mid,GrayGetPlane(DARK_PLANE),SPRT_XOR);
	Sprite8(40,20,8,ins,GrayGetPlane(DARK_PLANE),SPRT_XOR);
	Sprite8(40,20,8,mid,GrayGetPlane(DARK_PLANE),SPRT_XOR);
	ngetchx();
	GrayOff();
}