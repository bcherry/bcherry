// C Source File
// Created 6/21/2003; 9:56:25 PM

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS
#define USE_V200

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files
#include "extgraph.h"
#include "polysnd2.h"
#include "cool1.h"
#include "cool2.h"

#define maxkeywait 3

DEFINE_INT_HANDLER (myint2)
{
	pokeIO (0x60001B,0);
}

// Main Function
void _main(void)
{
	short width = 150;
	short startx = 0;
	short y;
	short shipx=76;
	const short shipy = 92;
	unsigned short cycles = 0;
	char ship[8]=
{
0b00011000,
0b00011000,
0b00111100,
0b00111100,
0b01111110,
0b01111110,
0b11111111,
0b11111111,
};
	char scr[3840];
	short left = 0;
	short right = 0;
	INT_HANDLER save1 = GetIntVec(AUTO_INT_1);
	INT_HANDLER save2 = GetIntVec(AUTO_INT_2);
	SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
	SetIntVec(AUTO_INT_2,myint2);

	pSnd_EnableSound ();
	pSnd_InstallSound ();

	pSnd_PlayMode (STEREO);
	pSnd_SetTempo (cool1_tempo);

	memset(scr,0x00,3840);
	while(_rowread(0)) continue;

	for(y=0;y<100;y++)
	{
		FastDrawHLine(scr,0,startx,y,A_XOR);
		FastDrawHLine(scr,startx+width,159,y,A_XOR);
		startx+=(random(2)-1);
		if(startx<0) startx = 0;
		if(startx+width>=160) startx = 160-width;
	}

	Sprite8_XOR(shipx,shipy,8,ship,scr);
	
	pSnd_PlaySound_voice1 (cool1);
	pSnd_PlaySound_voice2 (cool2);
	
	pSnd_SetState (ALLVOICES);
	
	while(!_keytest(RR_ESC))
	{
		if(_keytest(RR_ESC)) break;
		Sprite8_XOR(shipx,shipy,8,ship,scr);
		ScrollDown160((void*)scr,100);
		FastDrawHLine(scr,0,startx,0,A_XOR);
		FastDrawHLine(scr,startx+width,159,0,A_XOR);
		if(_keytest(RR_LEFT)) left++;
		if(left==maxkeywait)
		{
			left = 0;
			if(shipx) shipx--;
		}
		if(_keytest(RR_RIGHT)) right++;
		if(right==maxkeywait)
		{
			right = 0;
			if(shipx<159) shipx++;
		}
		Sprite8_XOR(shipx,shipy,8,ship,scr);
		FastCopyScreen(scr,LCD_MEM);
		//Cool Collision Detection
		if(EXT_GETPIX(scr,shipx+3,shipy-1)||EXT_GETPIX(scr,shipx+4,shipy-1)||
		EXT_GETPIX(scr,shipx-1,shipy+7)||EXT_GETPIX(scr,shipx+8,shipy+7)) //Hit
			break;
		startx+=(random(5)-2);
		if(startx<0) startx = 0;
		if(startx+width>=160) startx = 160-width;
		cycles++;
		width = 150-(cycles/((300-width)));
		
		if(!pSnd_VoiceState())	{pSnd_PlaySound_voice1 (cool1); pSnd_PlaySound_voice2 (cool2); pSnd_SetState (ALLVOICES);}
	}
	
	while(_rowread(0)) continue;

	pSnd_UninstallSound ();
	pSnd_DisableSound ();

	SetIntVec(AUTO_INT_1,save1);
	SetIntVec(AUTO_INT_2,save2);

	ClrScr();
	GKeyFlush();
	printf_xy(20,20,"Score: %u",cycles);
	ngetchx();

}
