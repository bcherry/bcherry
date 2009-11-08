// C Source File
// Created 1/10/2005; 4:10:11 PM

#include <tigcclib.h>
#include "Moving.h"
#include "extgraph.h"

void delay(int);

// Main Function
void _main(void)
{
	clrscr();
	INT_HANDLER int_1 = GetIntVec(AUTO_INT_1);
	INT_HANDLER int_5 = GetIntVec(AUTO_INT_5);
	SetIntVec(AUTO_INT_1, DUMMY_HANDLER);
	SetIntVec(AUTO_INT_5, DUMMY_HANDLER);
	GrayOn();
	int x = 0, y = 0;
	GraySprite8_XOR(x,y,8,block_l,block_d,GrayGetPlane(0),GrayGetPlane(1));
	while(!_keytest(RR_ESC)) {
		GraySprite8_XOR(x,y,8,block_l,block_d,GrayGetPlane(0),GrayGetPlane(1));
		if (_keytest(RR_LEFT) && x > 0) x--;
		if (_keytest(RR_RIGHT) && x < LCD_WIDTH - 8) x++;
		if (_keytest(RR_DOWN) && y < LCD_HEIGHT - 8) y++;
		if (_keytest(RR_UP) && y > 0) y--;
		GraySprite8_XOR(x,y,8,block_l,block_d,GrayGetPlane(0),GrayGetPlane(1));
		delay(100);
	}
	GrayOff();
	SetIntVec(AUTO_INT_1, int_1);
	SetIntVec(AUTO_INT_5, int_5);
}

void delay(int n){
	int i, ii;
	for (i = 0; i < n; i++)
		for (ii = 0; ii < 100; ii++);
}