// C Source File
// Created 15/06/2003; 22:47:10

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define USE_KERNEL

#include <tigcclib.h>         // Include All Header Files
#include "polysnd2.h"

INT_HANDLER OldInt1 = NULL;
INT_HANDLER OldInt2 = NULL;

unsigned char channel1[] = {_c4,6,pause,18,_d4,6,pause,18,_c4,3,pause,3,_c4,3,pause,3,_c4,3,pause,9,_d4,6,pause,18,_c4,6,pause,18,_d4,6,pause,12,_c4,3,pause,9,_c4,3,pause,9,_c4,3,pause,3,_d4,3,pause,9,_d4,3,pause,3,_d4,3,pause,3,_c4,6,pause,18,_d4,6,pause,18,_c4,3,pause,3,_c4,3,pause,3,_c4,3,pause,9,_d4,6,pause,18,_c4,6,pause,18,_d4,6,pause,12,_c4,3,pause,9,_c4,3,pause,9,_c4,3,pause,3,_d4,3,pause,9,_d4,3,pause,3,_d4,3,pause,3,_c4,6,pause,18,_d4,6,pause,18,_c4,3,pause,3,_c4,3,pause,3,_c4,3,pause,9,_d4,6,pause,18,_c4,6,pause,18,_d4,6,pause,12,_c4,3,pause,9,_c4,3,pause,9,_c4,3,pause,3,_d4,3,pause,9,_d4,3,pause,3,_d4,3,pause,3,_c4,6,pause,18,_d4,6,pause,18,_c4,3,pause,3,_c4,3,pause,3,_c4,3,pause,9,_d4,6,pause,18,_c4,6,pause,18,_d4,6,pause,12,_c4,3,pause,9,_c4,3,pause,9,_c4,3,pause,3,_d4,3,pause,9,_d4,3,pause,3,_d4,3,pause,3,_c4,6,pause,18,_d4,6,pause,18,_c4,3,pause,3,_c4,3,pause,3,_c4,3,pause,9,_d4,6,pause,18,_c4,6,pause,18,_d4,6,pause,12,_c4,3,pause,9,_c4,3,pause,9,_c4,3,pause,3,_d4,3,pause,9,_d4,3,pause,3,_d4,3,pause,3,_c4,6,pause,18,_d4,6,pause,18,_c4,3,pause,3,_c4,3,pause,3,_c4,3,pause,9,_d4,6,pause,18,_c4,6,pause,18,_d4,6,pause,12,_c4,3,pause,9,_c4,3,pause,9,_c4,3,pause,3,_d4,3,pause,9,_d4,3,pause,3,_d4,3,pause,3,_c4,6,pause,18,_d4,6,pause,18,_c4,3,pause,3,_c4,3,pause,3,_c4,3,pause,9,_d4,6,pause,18,_c4,6,pause,18,_d4,6,pause,12,_c4,3,pause,9,_c4,3,pause,9,_c4,3,pause,3,_d4,3,pause,9,_d4,3,pause,3,_d4,3,pause,3,_c4,6,pause,18,_d4,6,pause,18,_c4,3,pause,3,_c4,3,pause,3,_c4,3,pause,9,_d4,6,pause,18,_c4,6,pause,18,_d4,6,pause,12,_c4,3,pause,9,_c4,3,pause,9,_c4,3,pause,3,_d4,3,pause,9,_d4,3,pause,3,_d4,3,pause,3,_d5,1,_c4,1,_cd5,1,_d5,1,_c4,1,_cd5,1,_c4,3,pause,3,_b4,3,pause,3,_b4,3,pause,3,_d4,1,_d5,1,_d4,1,_d5,1,_d4,1,_d5,1,_d4,3,pause,3,_b4,3,pause,3,_b4,3,pause,3,_c4,1,_d5,1,_c4,1,_d5,1,_c4,1,_d5,1,_c4,1,_d5,1,_c4,1,_d5,1,_c4,1,_d5,1,_c4,3,pause,3,_b4,3,pause,3,_d4,1,_b4,1,_d4,1,_b4,1,_d4,1,_b4,1,_d4,3,pause,3,_b3,1,_a4,1,_b3,1,_a4,1,_b3,1,_a4,1,_b3,1,_a4,1,_b3,1,_a4,1,_b3,1,_a4,1,255};
unsigned char channel2[] = {pause,192,_c5,3,pause,3,_c6,3,pause,3,_a4,3,pause,3,_a5,3,pause,3,_ad4,3,pause,3,_ad5,3,pause,63,_c5,3,pause,3,_c6,3,pause,3,_a4,3,pause,3,_a5,3,pause,3,_ad4,3,pause,3,_ad5,3,pause,63,_f4,3,pause,3,_f5,3,pause,3,_d4,3,pause,3,_d5,3,pause,3,_dd4,3,pause,3,_dd5,3,pause,63,_f4,3,pause,3,_f5,3,pause,3,_d4,3,pause,3,_d5,3,pause,3,_dd4,3,pause,3,_dd5,3,pause,51,_dd5,2,pause,2,_d5,2,pause,2,_cd5,2,pause,2,_c5,3,pause,9,_dd5,3,pause,9,_d5,3,pause,9,_gd4,3,pause,9,_g4,3,pause,9,_cd5,3,pause,9,_c5,2,pause,2,_fd5,2,pause,2,_f5,2,pause,2,_e5,2,pause,2,_ad5,2,pause,2,_a5,2,pause,2,_gd5,3,pause,5,_dd5,3,pause,5,_b4,3,pause,5,_ad4,3,pause,5,_a4,3,pause,5,_gd4,3,pause,245,_c5,3,pause,3,_c6,3,pause,3,_a4,3,pause,3,_a5,3,pause,3,_ad4,3,pause,3,_ad5,3,pause,63,_c5,3,pause,3,_c6,3,pause,3,_a4,3,pause,3,_a5,3,pause,3,_ad4,3,pause,3,_ad5,3,pause,63,_f4,3,pause,3,_f5,3,pause,3,_d4,3,pause,3,_d5,3,pause,3,_dd4,3,pause,3,_dd5,3,pause,63,_f4,3,pause,3,_f5,3,pause,3,_d4,3,pause,3,_d5,3,pause,3,_dd4,3,pause,3,_dd5,3,pause,51,_dd5,2,pause,2,_d5,2,pause,2,_cd5,2,pause,2,_c5,3,pause,9,_dd5,3,pause,9,_d5,3,pause,9,_gd4,3,pause,9,_g4,3,pause,9,_cd5,3,pause,9,_c5,2,pause,2,_fd5,2,pause,2,_f5,2,pause,2,_e5,2,pause,2,_ad5,2,pause,2,_a5,2,pause,2,_gd5,3,pause,5,_dd5,3,pause,5,_b4,3,pause,5,_ad4,3,pause,5,_a4,3,pause,5,_gd4,3,pause,3,255};

DEFINE_INT_HANDLER (myint2)
{
	pokeIO (0x60001B,0);
}

void _main(void)
{
	OldInt1 = GetIntVec (AUTO_INT_1);
	OldInt2 = GetIntVec (AUTO_INT_2);
	SetIntVec (AUTO_INT_1, DUMMY_HANDLER);
  SetIntVec (AUTO_INT_2, myint2);
  	
	pSnd_EnableSound ();
	pSnd_InstallSound ();
	
	pSnd_PlayMode (STEREO);
	pSnd_SetTempo (75);
		
	pSnd_PlaySound_voice1 (channel1);
	pSnd_PlaySound_voice2 (channel2);
	
	pSnd_SetState (ALLVOICES);
	
	while ((_keytest (RR_ESC)!=TRUE) && (pSnd_VoiceState()));
		
	pSnd_UninstallSound ();
	pSnd_DisableSound ();
	
	SetIntVec (AUTO_INT_1, OldInt1);
	SetIntVec (AUTO_INT_2, OldInt2);
}
