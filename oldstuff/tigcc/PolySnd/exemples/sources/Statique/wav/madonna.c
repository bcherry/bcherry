// C Source File
// Created 15/06/2003; 22:47:10

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#include <tigcclib.h>         // Include All Header Files
#include "polysnd2.h"
#include "sound.h"

INT_HANDLER OldInt1 = NULL;
INT_HANDLER OldInt2 = NULL;

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
	
	pSnd_PlayMode (MONO);
	pSnd_Samples (8000);
		
	pSnd_PlayWAV_voice1 (_son,sizeof(_son));
	
	pSnd_SetState (ALLVOICES);
	
	while ((_keytest (RR_ESC)!=TRUE) && (pSnd_GetState()==ALLVOICES));
		
	pSnd_UninstallSound ();
	pSnd_DisableSound ();
	
	SetIntVec (AUTO_INT_1, OldInt1);
	SetIntVec (AUTO_INT_2, OldInt2);
}
