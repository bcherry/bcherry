// C Source File
// Created 3/27/2005; 7:41:41 PM

#include "all.h"


void textBox(const char *string) {
	GrayFastFillRect_R(GetPlane(0),GetPlane(1), LCD_WIDTH/2 - 4*strlen(string) - 10,LCD_HEIGHT/2-15,LCD_WIDTH/2+4*strlen(string) + 10,LCD_HEIGHT/2+15,3);
	GrayDrawStrExt(0,LCD_HEIGHT/2-5,string,A_XOR | A_CENTERED,F_8x10);
}
