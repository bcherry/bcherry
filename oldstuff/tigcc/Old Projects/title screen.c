// C Source File
// Created 13/Nov/03; 17:08:36

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define MIN_AMS 100           // Compile for AMS 1.00 or higher

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

#include "title.h"


void drawTitle(int planes) {
		int x,y;
    int tilewidth  = 32;
    int tileheight = 32;
    int vcount     = 3;
    int hcount     = 5;
    int i;
    clrscr();
    //if (!GrayMode(GRAY_ON)) return;
    memset(GetPlane(0),0,LCD_SIZE);
    memset(GetPlane(1),0,LCD_SIZE);
    for (y=0;y<vcount;y++) {
        for (x=0;x<hcount;x++) { 
            int offset = (y*hcount+x)*tileheight;
            for (i=0;i<planes;i++) {
                Sprite32(x*tilewidth,y*tileheight,tileheight,&title[i][offset],GetPlane(i),SPRT_OR);
            }
        }
    }
    
    //while (!kbhit());
    //ngetchx();
    //GrayMode(GRAY_OFF);
}

inline void drawPointer(POSITION pos) {
	Sprite8(pos.x,pos.y*15 + 20,8,pointer_sprite,GrayGetPlane(DARK_PLANE),SPRT_XOR);
	//Sprite8(pos.x,pos.y,8,pointer_sprite,GrayGetPlane(LIGHT_PLANE),SPRT_OR);
}

void drawWords(void) {
	FontSetSys(F_8x10);
	GraySetAMSPlane(LIGHT_PLANE);
	DrawStr(20,20,"Play", A_NORMAL);
	DrawStr(20,35,"Help", A_NORMAL);
	DrawStr(20,50,"Options", A_NORMAL);
	DrawStr(20,65,"Credits", A_NORMAL);
	DrawStr(20,80,"High Scores", A_NORMAL);
	GraySetAMSPlane(DARK_PLANE);
	DrawStr(20,20,"Play", A_NORMAL);
	DrawStr(20,35,"Help", A_NORMAL);
	DrawStr(20,50,"Options", A_NORMAL);
	DrawStr(20,65,"Credits", A_NORMAL);
	DrawStr(20,80,"High Scores", A_NORMAL);
	FontSetSys(F_6x8);
}

/*int accept(void) {
	if (_rowread(0x02) & 0x01) return 1;
	return 0;
}*/

// Main Function
void _main(void)
{
	INT_HANDLER interrupt1 = GetIntVec(AUTO_INT_1);  // this will save auto int 1
	//SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
	GrayOn();
	drawTitle(2);
	ngetchx();
	
	/*int keys[8];
	GetKeyMasks(keys);*/
	
  int key = 0;
  drawTitle(1);
  drawWords();
  POSITION pointer = {10,0};
  drawPointer(pointer);
  while (1) {
  	key = ngetchx();
  	if (key == KEY_ENTER) break;
  	if (key) drawPointer(pointer);
  	if (key == KEY_UP && pointer.y > 0) pointer.y--;
  	if (key == KEY_DOWN && pointer.y < 4) pointer.y++;
  	if (key) drawPointer(pointer);
  	
  	
  	/*key = _rowread(ARROW_ROW);
  	if (accept()) break;*/
  	//drawTitle(1);  
	  /*if (key) drawPointer(pointer);
  	if (key & 0x01 && pointer.y > 0) pointer.y--;
  	if (key & 0x04 && pointer.y < 5) pointer.y++;
  	if (key) drawPointer(pointer);*/
  	
  }
  
 	GrayOff();  
 //	SetIntVec(AUTO_INT_1,interrupt1);
	
}
