// C Source File
// Created 15/06/2003; 22:47:10

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#include <tigcclib.h>         // Include All Header Files
#include "polysnd2.h"

INT_HANDLER OldInt1 = NULL;
INT_HANDLER OldInt2 = NULL;

unsigned char channel1[] = {pause,8,_ad6,33,pause,33,_f6,6,pause,8,_f6,6,pause,7,_ad6,6,pause,7,_gd6,3,pause,4,_fd6,3,pause,4,_gd6,46,pause,47,_ad6,33,pause,34,_fd6,6,pause,7,_fd6,6,pause,7,_ad6,6,pause,8,_a6,3,pause,3,_g6,3,pause,4,_a6,44,pause,46,_cd5,1,_f5,1,_cd5,1,_f5,1,_cd5,1,_f5,1,_cd5,1,_f5,1,_cd5,1,_f5,1,_cd5,1,_f5,1,_cd5,48,pause,46,_cd5,54,pause,55,_ad6,13,pause,14,_f6,19,pause,21,_ad6,6,pause,7,_ad6,3,pause,4,_c7,3,pause,4,_d7,3,pause,3,_dd7,3,pause,4,_f7,53,pause,54,_ad6,13,pause,13,_f6,20,pause,20,_ad6,6,pause,8,_ad6,3,pause,3,_c7,3,pause,4,_d7,3,pause,4,_dd7,3,pause,3,_f7,53,pause,54,_ad6,13,pause,14,_f6,20,pause,20,_ad6,6,pause,7,_ad6,3,pause,4,_c7,3,pause,3,_d7,3,pause,4,_dd7,3,pause,4,_f7,33,pause,33,_f7,6,pause,8,_f7,4,pause,4,_fd7,4,pause,5,_gd7,4,pause,5,_ad7,35,pause,36,_ad7,4,pause,4,_ad7,4,pause,5,_gd7,4,pause,6,_fd7,4,pause,5,_gd7,8,pause,10,_fd7,4,pause,5,_f7,26,pause,27,_f7,13,pause,14,_dd7,6,pause,7,_dd7,3,pause,4,_f7,3,pause,3,_fd7,26,pause,28,_f7,6,pause,7,_dd7,6,pause,7,_cd7,6,pause,8,_cd7,3,pause,3,_dd7,3,pause,4,_f7,26,pause,27,_dd7,6,pause,8,_cd7,6,pause,7,_e6,13,pause,14,_e6,6,pause,7,_e6,3,pause,4,_f6,3,pause,3,_g6,6,pause,8,_g6,3,pause,3,_a6,3,pause,4,_ad6,6,pause,7,_c7,6,pause,8,_a5,6,pause,7,_a5,3,pause,4,_a5,3,pause,3,_gd5,6,pause,8,_gd5,3,pause,3,_gd5,3,pause,4,_g5,6,pause,7,_g5,3,pause,4,_g5,3,pause,4,_fd5,6,pause,7,_fd5,6,pause,7,_ad6,13,pause,14,_f6,20,pause,20,_ad6,6,pause,7,_ad6,3,pause,4,_c7,3,pause,4,_d7,3,pause,3,_dd7,3,pause,4,_f7,33,pause,34,_f7,6,pause,7,_f7,4,pause,5,_fd7,4,pause,4,_gd7,4,pause,6,_cd7,40,pause,40,_e7,13,pause,13,_dd7,13,pause,14,_c7,26,pause,27,_a6,13,pause,14,_e4,4,pause,4,_ad4,4,pause,5,_cd5,4,pause,5,_e5,4,pause,5,_ad5,4,pause,5,_cd6,4,pause,5,_e6,26,pause,28,_f6,13,pause,13,_f5,4,pause,5,_f5,4,pause,5,_f5,4,pause,5,_f5,26,pause,27,_e4,4,pause,5,_ad4,4,pause,5,_cd5,4,pause,4,_e5,4,pause,5,_ad5,4,pause,5,_cd6,4,pause,6,_e6,26,pause,27,_f6,13,pause,14,_f5,4,pause,4,_f5,4,pause,5,_f5,4,pause,5,_f5,26,pause,28,_fd6,39,pause,41,_b6,13,pause,13,_ad6,13,pause,14,_f6,26,pause,27,_cd6,13,pause,14,_e6,13,pause,14,_e6,6,pause,7,_e6,3,pause,4,_f6,3,pause,3,_g6,6,pause,8,_g6,3,pause,3,_a6,3,pause,4,_ad6,6,pause,7,_c7,6,pause,8,_a5,6,pause,7,_a5,3,pause,4,_a5,3,pause,3,_gd5,6,pause,8,_gd5,3,pause,3,_gd5,3,pause,4,_g5,6,pause,7,_g5,3,pause,4,_g5,3,pause,4,_fd5,6,pause,7,_fd5,7,pause,8,255};
unsigned char channel2[] = {pause,255,pause,176,_f5,6,pause,7,_f5,3,pause,4,_ad4,3,pause,3,_f5,6,pause,8,_f5,3,pause,3,_ad4,3,pause,4,_f5,6,pause,7,_f5,3,pause,4,_ad4,3,pause,4,_f5,3,pause,3,_ad4,3,pause,4,_f5,3,pause,4,_ad4,3,pause,3,_f5,6,pause,8,_f5,3,pause,3,_ad4,3,pause,4,_f5,6,pause,7,_f5,3,pause,4,_ad4,3,pause,4,_f5,6,pause,7,_f5,3,pause,4,_ad4,3,pause,3,_f5,3,pause,4,_ad4,3,pause,4,_f5,4,pause,5,_ad4,3,pause,3,_f5,6,pause,8,_f5,3,pause,3,_ad4,3,pause,4,_f5,6,pause,7,_f5,3,pause,4,_ad4,3,pause,4,_f5,6,pause,7,_f5,3,pause,4,_ad4,3,pause,3,_f5,3,pause,4,_ad4,3,pause,4,_f5,3,pause,3,_ad4,3,pause,4,_dd5,6,pause,7,_ad5,6,pause,8,_ad5,3,pause,3,_c6,3,pause,4,_d6,3,pause,4,_dd6,3,pause,3,_f6,6,pause,8,_dd5,3,pause,3,_gd4,3,pause,4,_dd5,3,pause,4,_gd4,3,pause,3,_dd5,3,pause,4,_gd4,3,pause,4,_cd5,6,pause,7,_cd5,3,pause,4,_fd4,3,pause,3,_cd5,6,pause,8,_cd5,3,pause,3,_fd4,3,pause,4,_cd5,6,pause,7,_cd5,3,pause,4,_fd4,3,pause,4,_cd5,3,pause,3,_fd4,3,pause,4,_cd5,3,pause,4,_fd4,3,pause,3,_c5,6,pause,8,_ad5,6,pause,7,_ad5,3,pause,4,_c6,3,pause,3,_d6,3,pause,4,_dd6,3,pause,4,_f6,6,pause,7,_c5,3,pause,4,_f4,3,pause,3,_c5,3,pause,4,_f4,3,pause,4,_c5,3,pause,3,_f4,3,pause,4,_d6,13,pause,14,_d6,4,pause,4,_d6,4,pause,5,_c6,4,pause,5,_d6,10,pause,10,_d6,3,pause,4,_d6,3,pause,4,_dd6,3,pause,3,_f6,3,pause,4,_g6,3,pause,4,_gd6,10,pause,10,_ad6,3,pause,3,_ad6,3,pause,4,_c7,3,pause,4,_d7,3,pause,3,_dd7,3,pause,4,_f7,13,pause,14,_gd6,4,pause,4,_ad6,4,pause,5,_c7,4,pause,5,_fd6,10,pause,10,_fd6,3,pause,4,_fd6,3,pause,4,_gd6,3,pause,3,_ad6,3,pause,4,_c7,3,pause,4,_cd7,8,pause,9,_cd7,4,pause,5,_cd7,4,pause,6,_c7,4,pause,4,_ad6,4,pause,5,_cd7,8,pause,9,_ad6,4,pause,6,_gd6,4,pause,4,_gd6,4,pause,5,_fd6,4,pause,4,_gd6,8,pause,10,_gd6,4,pause,5,_gd6,4,pause,6,_fd6,4,pause,4,_gd6,4,pause,5,_fd6,6,pause,7,_fd6,3,pause,4,_f6,3,pause,3,_fd6,6,pause,8,_fd6,3,pause,4,_gd6,3,pause,3,_ad6,13,pause,14,_gd6,6,pause,7,_fd6,6,pause,7,_f6,6,pause,8,_f6,3,pause,3,_dd6,3,pause,4,_f6,6,pause,7,_f6,3,pause,4,_fd6,3,pause,4,_gd6,13,pause,13,_fd6,6,pause,8,_f6,6,pause,7,_c7,6,pause,7,_c7,3,pause,4,_d7,3,pause,4,_e7,26,pause,27,_g7,13,pause,14,_f7,6,pause,7,_f6,3,pause,4,_f6,3,pause,3,_f6,6,pause,8,_f6,3,pause,3,_f6,3,pause,4,_f6,6,pause,7,_f6,3,pause,4,_f6,3,pause,4,_f6,6,pause,7,_f6,6,pause,7,_d6,13,pause,14,_d6,4,pause,5,_d6,4,pause,4,_c6,4,pause,6,_d6,10,pause,10,_d6,3,pause,3,_d6,3,pause,4,_dd6,3,pause,4,_f6,3,pause,3,_g6,3,pause,4,_gd6,10,pause,10,_ad6,3,pause,4,_ad6,3,pause,3,_c7,3,pause,4,_d7,3,pause,4,_dd7,3,pause,3,_f7,13,pause,14,_gd6,4,pause,5,_ad6,4,pause,4,_c7,4,pause,6,_ad7,40,pause,40,_cd8,13,pause,13,_c8,13,pause,14,_a7,26,pause,27,_f7,13,pause,14,_fd7,39,pause,41,_ad7,13,pause,14,_a7,13,pause,13,_f7,26,pause,28,_f7,13,pause,13,_fd7,39,pause,41,_ad7,13,pause,14,_a7,13,pause,14,_f7,26,pause,27,_d7,13,pause,14,_dd7,40,pause,40,_fd7,13,pause,13,_f7,13,pause,14,_cd7,26,pause,27,_ad6,13,pause,14,_c7,6,pause,7,_c7,3,pause,4,_d7,3,pause,4,_e7,26,pause,27,_g7,13,pause,14,_f7,6,pause,7,_f6,3,pause,4,_f6,3,pause,3,_f6,6,pause,8,_f6,3,pause,3,_f6,3,pause,4,_f6,6,pause,7,_f6,3,pause,4,_f6,3,pause,4,_f6,6,pause,7,_f6,9,pause,10,255};

DEFINE_INT_HANDLER (myint2) {pokeIO (0x60001B,0);}

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
