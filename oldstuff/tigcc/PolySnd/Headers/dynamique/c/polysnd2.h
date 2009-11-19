//PolySnd v2.0
//------------
//(c) 2002-2003 by Geoffrey Anneheim
//geoffrey.anneheim@wanadoo.fr

#ifdef DOORS
#ifndef __H_polysnd2
#define __H_polysnd2

#define NOVOICE 													0
#define VOICE1														1
#define VOICE2														2
#define ALLVOICES 												3

#define BEEP_VOICE1												1
#define BEEP_VOICE2												16
#define FX_VOICE1													2
#define FX_VOICE2													32
#define SOUND_VOICE1											8
#define SOUND_VOICE2											128
#define WAV_VOICE1												4
#define WAV_VOICE2												64

#define MONO															0
#define STEREO														1
 
 
#define pSnd_EnableSound 		  						polysnd2__0000
#define pSnd_DisableSound									polysnd2__0001
#define pSnd_InstallSound									polysnd2__0002
#define pSnd_UninstallSound 							polysnd2__0003

#define pSnd_Initialize										polysnd2__0004

#define pSnd_SetTempo_voice1							polysnd2__0005
#define pSnd_SetTempo_voice2							polysnd2__0006
#define pSnd_GetTempo_voice1							polysnd2__0007
#define pSnd_GetTempo_voice2							polysnd2__0008

#define pSnd_SetState											polysnd2__0009
#define pSnd_SetLoop											polysnd2__000A

#define pSnd_SetBeep_voice1								polysnd2__000B
#define pSnd_SetBeep_voice2								polysnd2__000C

#define pSnd_PlayFX_voice1								polysnd2__000D
#define pSnd_PlayFX_voice2								polysnd2__000E

#define pSnd_GetFrequency_voice1					polysnd2__000F
#define pSnd_GetFrequency_voice2					polysnd2__0010

#define pSnd_PortStatus										polysnd2__0011

#define pSnd_Interrupt5										polysnd2__0012

#define pSnd_PlaySound_voice1							polysnd2__0013
#define pSnd_PlaySound_voice2							polysnd2__0014

#define pSnd_GetLength_voice1							polysnd2__0015
#define pSnd_GetLength_voice2							polysnd2__0016

#define pSnd_GetTime											polysnd2__0017
#define pSnd_GetState											polysnd2__0018
#define pSnd_GetMode											polysnd2__0019
#define pSnd_GetLoop											polysnd2__001A

#define pSnd_Time_voice1									polysnd2__001B
#define pSnd_Time_voice2									polysnd2__001C
#define pSnd_SetTime_voice1								polysnd2__001D
#define pSnd_SetTime_voice2								polysnd2__001E

#define pSnd_VoiceState										polysnd2__001F
#define pSnd_SetVoiceState								polysnd2__0020

#define pSnd_SetIntMask										polysnd2__0021

#define pSnd_AddNotes_voice1							polysnd2__0022
#define pSnd_AddNotes_voice2							polysnd2__0023

#define pSnd_PlayWAV_voice1								polysnd2__0024
#define pSnd_PlayWAV_voice2								polysnd2__0025

#define pSnd_Sample_voice1								polysnd2__0026
#define pSnd_Sample_voice2								polysnd2__0027

#define pSnd_GetNote_voice1								polysnd2__0028
#define pSnd_GetNote_voice2								polysnd2__0029

#define pSnd_Ptridle_voice1								polysnd2__002A
#define pSnd_Ptridle_voice2								polysnd2__002B
#define pSnd_time_counter_voice1					polysnd2__002C
#define pSnd_time_counter_voice2					polysnd2__002D
#define pSnd_note_time_counter_voice1			polysnd2__002E
#define pSnd_note_time_counter_voice2			polysnd2__002F
#define pSnd_SkipData_voice1							polysnd2__0030
#define pSnd_SkipData_voice2							polysnd2__0031

#define pSnd_PlayMode											polysnd2__0032

#define pSnd_Setlength_voice1							polysnd2__0033
#define pSnd_SetLength_voice2							polysnd2__0034

#define pSnd_SetInterrupt5Rate						polysnd2__0035


extern void pSnd_EnableSound (void);
extern void pSnd_DisableSound (void);
extern void pSnd_InstallSound (void);
extern void pSnd_UninstallSound (void);

extern void pSnd_Initialize (void);

__attribute__((regparm)) extern void pSnd_SetTempo_voice1 (unsigned short asm("d0"));
__attribute__((regparm)) extern void pSnd_SetTempo_voice2 (unsigned short asm("d0"));
extern unsigned short pSnd_GetTempo_voice1 (void);
extern unsigned short pSnd_GetTempo_voice2 (void);

__attribute__((regparm)) extern void pSnd_SetState (unsigned char asm("d0"));
__attribute__((regparm)) extern void pSnd_SetLoop (unsigned char asm("d0"));

__attribute__((regparm)) extern void pSnd_SetBeep_voice1 (unsigned short asm("d0"),unsigned char asm ("d1"));
__attribute__((regparm)) extern void pSnd_SetBeep_voice2 (unsigned short asm("d0"),unsigned char asm ("d1"));
__attribute__((regparm)) extern void pSnd_PlayFX_voice1 (unsigned short *asm("a0"));
__attribute__((regparm)) extern void pSnd_PlayFX_voice2 (unsigned short *asm("a0"));

extern unsigned short pSnd_GetFrequency_voice1 (void);
extern unsigned short pSnd_GetFrequency_voice2 (void);

extern unsigned char pSnd_PortStatus (void);

__attribute__((regparm)) extern void pSnd_Interrupt5 (INT_HANDLER asm("a0"));

__attribute__((regparm)) extern void pSnd_PlaySound_voice1 (unsigned char *asm("a0"));
__attribute__((regparm)) extern void pSnd_PlaySound_voice2 (unsigned char *asm("a0"));

extern unsigned char pSnd_GetLength_voice1 (void);
extern unsigned char pSnd_GetLength_voice2 (void);

__attribute__((regparm)) extern unsigned short pSnd_GetTime (unsigned char*asm("a0"),unsigned short asm("d2"));
extern unsigned char pSnd_GetState (void);
extern unsigned char pSnd_GetMode (void);
extern unsigned char pSnd_GetLoop (void);

extern unsigned short pSnd_Time_voice1 (void);
extern unsigned short pSnd_Time_voice2 (void);
__attribute__((regparm)) extern void pSnd_SetTime_voice1 (unsigned short asm("d0"));
__attribute__((regparm)) extern void pSnd_SetTime_voice2 (unsigned short asm("d0"));

extern unsigned char pSnd_VoiceState (void);
__attribute__((regparm)) extern void pSnd_SetVoiceState (unsigned char asm("d0"));

__attribute__((regparm)) extern void pSnd_SetIntMask (unsigned short asm("d0"));

__attribute__((regparm)) extern void pSnd_AddNotes_voice1 (unsigned char asm("d0"));
__attribute__((regparm)) extern void pSnd_AddNotes_voice2 (unsigned char asm("d0"));

__attribute__((regparm)) extern void pSnd_PlayWAV_voice1 (unsigned char *asm("a0"), unsigned short asm("d0"));
__attribute__((regparm)) extern void pSnd_PlayWAV_voice2 (unsigned char *asm("a0"), unsigned short asm("d0"));

__attribute__((regparm)) extern void pSnd_Sample_voice1 (unsigned short asm("d0"));
__attribute__((regparm)) extern void pSnd_Sample_voice2 (unsigned short asm("d0"));

extern unsigned char pSnd_GetNote_voice1 (void);
extern unsigned char pSnd_GetNote_voice2 (void);

__attribute__((regparm)) extern void *pSnd_Ptridle_voice1 (void *asm("a0"));
__attribute__((regparm)) extern void *pSnd_Ptridle_voice2 (void *asm("a0"));
__attribute__((regparm)) extern unsigned long pSnd_time_counter_voice1 (unsigned long asm("d0"));
__attribute__((regparm)) extern unsigned long pSnd_time_counter_voice2 (unsigned long asm("d0"));
__attribute__((regparm)) extern unsigned long pSnd_note_time_counter_voice1 (unsigned long asm("d0"));
__attribute__((regparm)) extern unsigned long pSnd_note_time_counter_voice2 (unsigned long asm("d0"));
extern void pSnd_SkipData_voice1 (void);
extern void pSnd_SkipData_voice2 (void);

__attribute__((regparm)) extern void pSnd_PlayMode (unsigned char asm("d0"));

__attribute__((regparm)) extern void pSnd_SetLength_voice1 (unsigned char asm("d0"));
__attribute__((regparm)) extern void pSnd_SetLength_voice2 (unsigned char asm("d0"));

__attribute__((regparm)) extern void pSnd_SetInterrupt5Rate (unsigned short asm("d0"));


#define pSnd_SetTempo(tempo)																	{pSnd_SetTempo_voice1(tempo); pSnd_SetTempo_voice2(tempo);}
#define pSnd_AddNotes(addnts)																	{pSnd_AddNotes_voice1(addnts); pSnd_AddNotes_voice2(addnts);}
#define pSnd_Samples(sample)																	{pSnd_Sample_voice1(sample); pSnd_Sample_voice2(sample);}
#define pSnd_SetTime(time)																		{pSnd_SetTime_voice1(time); pSnd_SetTime_voice2(time);}
 
#define USE_GRAY_WITH_POLYSND																	asm("move.w #0x2400,__gray_plane_index+6")


/*******************************
 Enumération des notes pour MIDI
 *******************************/

/*-----------
 | OCTAVE 0 |
 ------------*/
#define _c0									1
#define _cd0								2
#define _db0								2
#define _d0									3
#define _dd0								4
#define _eb0								4
#define _e0									5
#define _f0									6
#define _fd0								7
#define _gb0								7
#define _g0									8
#define _gd0								9
#define _ab0								9
#define _a0									10
#define _ad0								11
#define _bb0								11
#define _b0									12 


/*-----------
 | OCTAVE 1 |
 ------------*/
#define _c1									13
#define _cd1								14
#define _db1								14
#define _d1									15
#define _dd1								16
#define _eb1								16
#define _e1									17
#define _f1									18
#define _fd1								19
#define _gb1								19
#define _g1									20
#define _gd1								21
#define _ab1								21
#define _a1									22
#define _ad1								23
#define _bb1								23
#define _b1									24 


/*-----------
 | OCTAVE 2 |
 ------------*/
#define _c2									25
#define _cd2								26
#define _db2								26
#define _d2									27
#define _dd2								28
#define _eb2								28
#define _e2									29
#define _f2									30
#define _fd2								31
#define _gb2								31
#define _g2									32
#define _gd2								33
#define _ab2								33
#define _a2									34
#define _ad2								35
#define _bb2								35
#define _b2									36 


/*-----------
 | OCTAVE 3 |
 ------------*/
#define _c3									37
#define _cd3								38
#define _db3								38
#define _d3									39
#define _dd3								40
#define _eb3								40
#define _e3									41
#define _f3									42
#define _fd3								43
#define _gb3								43
#define _g3									44
#define _gd3								45
#define _ab3								45
#define _a3									46
#define _ad3								47
#define _bb3								47
#define _b3									48 


/*-----------
 | OCTAVE 4 |
 ------------*/
#define _c4									49
#define _cd4								50
#define _db4								50
#define _d4									51
#define _dd4								52
#define _eb4								52
#define _e4									53
#define _f4									54
#define _fd4								55
#define _gb4								55
#define _g4									56
#define _gd4								57
#define _ab4								57
#define _a4									58
#define _ad4								59
#define _bb4								59
#define _b4									60 


/*-----------
 | OCTAVE 5 |
 ------------*/
#define _c5									61
#define _cd5								62
#define _db5								62
#define _d5									63
#define _dd5								64
#define _eb5								64
#define _e5									65
#define _f5									66
#define _fd5								67
#define _gb5								67
#define _g5									68
#define _gd5								69
#define _ab5								69
#define _a5									70
#define _ad5								71
#define _bb5								71
#define _b5									72
 

/*-----------
 | OCTAVE 6 |
 ------------*/
#define _c6									73
#define _cd6								74
#define _db6								74
#define _d6									75
#define _dd6								76
#define _eb6								76
#define _e6									77
#define _f6									78
#define _fd6								79
#define _gb6								79
#define _g6									80
#define _gd6								81
#define _ab6								81
#define _a6									82
#define _ad6								83
#define _bb6								83
#define _b6									84 


/*-----------
 | OCTAVE 7 |
 ------------*/
#define _c7									85
#define _cd7								86
#define _db7								86
#define _d7									87
#define _dd7								88
#define _eb7								88
#define _e7									89
#define _f7									90
#define _fd7								91
#define _gb7								91
#define _g7									92
#define _gd7								93
#define _ab7								93
#define _a7									94
#define _ad7								95
#define _bb7								95
#define _b7									96


/*-----------
 | OCTAVE 8 |
 ------------*/
#define _c8									97
#define _cd8								98
#define _db8								98
#define _d8									99
#define _dd8								100
#define _eb8								100
#define _e8									101
#define _f8									102
#define _fd8								103
#define _gb8								103
#define _g8									104
#define _gd8								105
#define _ab8								105
#define _a8									106
#define _ad8								107
#define _bb8								107
#define _b8									108


/*-----------
 | OCTAVE 9 |
 ------------*/
#define _c9									109
#define _cd9								110
#define _db9								110
#define _d9									111
#define _dd9								112
#define _eb9								112
#define _e9									113
#define _f9									114
#define _fd9								115
#define _gb9								115
#define _g9									116
#define _gd9								117
#define _ab9								117
#define _a9									118
#define _ad9								119
#define _bb9								119
#define _b9									120
  
/*************************************
 Enumération des notes norme française
 *************************************/

/*-----------
 | OCTAVE 0 |
 ------------*/
#define do_0								1
#define dod_0								2
#define reb_0								2
#define re_0								3
#define red_0								4
#define mib_0								4
#define mi_0								5
#define fa_0								6
#define fad_0								7
#define solb_0							7
#define sol_0								8
#define sold_0							9
#define lab_0								9
#define la_0								10
#define lad_0								11
#define sib_0								11
#define si_0								12 


/*-----------
 | OCTAVE 1 |
 ------------*/
#define do_1								13
#define dod_1								14
#define reb_1								14
#define re_1								15
#define red_1								16
#define mib_1								16
#define mi_1								17
#define fa_1								18
#define fad_1								19
#define solb_1							19
#define sol_1								20
#define sold_1							21
#define lab_1								21
#define la_1								22
#define lad_1								23
#define sib_1								23
#define si_1								24 

 
 /*-----------
 | OCTAVE 2 |
 ------------*/
#define do_2								25
#define dod_2								26
#define reb_2								26
#define re_2								27
#define red_2								28
#define mib_2								28
#define mi_2								29
#define fa_2								30
#define fad_2								31
#define solb_2							31
#define sol_2								32
#define sold_2							33
#define lab_2								33
#define la_2								34
#define lad_2								35
#define sib_2								35
#define si_2								36 


/*-----------
 | OCTAVE 3 |
 ------------*/
#define do_3								37
#define dod_3								38
#define reb_3								38
#define re_3								39
#define red_3								40
#define mib_3								40
#define mi_3								41
#define fa_3								42
#define fad_3								43
#define solb_3							43
#define sol_3								44
#define sold_3							45
#define lab_3								45
#define la_3								46
#define lad_3								47
#define sib_3								47
#define si_3								48 


/*-----------
 | OCTAVE 4 |
 ------------*/
#define do_4								49
#define dod_4								50
#define reb_4								50
#define re_4								51
#define red_4								52
#define mib_4								52
#define mi_4								53
#define fa_4								54
#define fad_4								55
#define solb_4							55
#define sol_4								56
#define sold_4							57
#define lab_4								57
#define la_4								58
#define lad_4								59
#define sib_4								59
#define si_4								60 


/*-----------
 | OCTAVE 5 |
 ------------*/
#define do_5								61
#define dod_5								62
#define reb_5								62
#define re_5								63
#define red_5								64
#define mib_5								64
#define mi_5								65
#define fa_5								66
#define fad_5								67
#define solb_5							67
#define sol_5								68
#define sold_5							69
#define lab_5								69
#define la_5								70
#define lad_5								71
#define sib_5								71
#define si_5								72
 

/*-----------
 | OCTAVE 6 |
 ------------*/
#define do_6								73
#define dod_6								74
#define reb_6								74
#define re_6								75
#define red_6								76
#define mib_6								76
#define mi_6								77
#define fa_6								78
#define fad_6								79
#define solb_6							79
#define sol_6								80
#define sold_6							81
#define lab_6								81
#define la_6								82
#define lad_6								83
#define sib_6								83
#define si_6								84 


/*-----------
 | OCTAVE 7 |
 ------------*/
#define do_7								85
#define dod_7								86
#define reb_7								86
#define re_7								87
#define red_7								88
#define mib_7								88
#define mi_7								89
#define fa_7								90
#define fad_7								91
#define solb_7							91
#define sol_7								92
#define sold_7							93
#define lab_7								93
#define la_7								94
#define lad_7								95
#define sib_7								95
#define si_7								96


/*-----------
 | OCTAVE 8 |
 ------------*/
#define do_8								97
#define dod_8								98
#define reb_8								98
#define re_8								99
#define red_8								100
#define mib_8								100
#define mi_8								101
#define fa_8								102
#define fad_8								103
#define solb_8							103
#define sol_8								104
#define sold_8							105
#define lab_8								105
#define la_8								106
#define lad_8								107
#define sib_8								107
#define si_8								108


/*-----------
 | OCTAVE 9 |
 ------------*/
#define do_9								109
#define dod_9								110
#define reb_9								110
#define re_9								111
#define red_9								112
#define mib_9								112
#define mi_9								113
#define fa_9								114
#define fad_9								115
#define solb_9							115
#define sol_9								116
#define sold_9							117
#define lab_9								117
#define la_9								118
#define lad_9								119
#define sib_9								119
#define si_9								120
  
#define pause								00
#define rest								00


/******************************************************************************
 Voici la liste des durées. Cette liste peut être modifiée.
 Par défaut 32=noire.
 Durées étalonnées avec précision en fonction du tempo.

 Ronde = 4 temps
 Blanche = 2 temps
 Blanche pointée = 2+1/2
 Noire pointée = 1+1/2 temps
 Noire = 1 temps
 Croche pointée = 1/2+1/4 temps
 Croche = 1/2 temps
 Double croche = 1/4 temps
 Triple croche = 1/8 temps
 Triolet = 1/3 temps
********************************************************************************/

#define _r									128
#define _bp									80
#define _b									64
#define _np									48	
#define _n									32
#define _cp									24
#define _c									16
#define _cc									8
#define _ccc								4
#define _cccc								2
#define _tri								11

#define ronde								128
#define blanche_pointee			80
#define blanche							64
#define noire_pointee				48	
#define noire								32
#define croche_pointee			24
#define croche							16
#define double_croche				8
#define triple_croche				4
#define quadruple_croche 		2
#define triolet							11

#define _soupir							34
#define _soupirp						48
#define soupir							34
#define soupir_pointe				48

#define whole								128	//Ronde
#define half								80	//Blanche
#define quarter							34	//Noire
#define eighth							16	//Croche
#define sixteenth						8	//Double croche
#define thirtysecond				4	//Triple croche
#define sixtyfourth					2	//Quadruple croche

#define half3								21
#define quarter3						10
#define eighth3							5
#define sixteenth3					2
#define thirtysecond3				1
#define sixtyfourth3				0
#endif
#else
#error: Vous devez être en mode KERNEL pour pouvoir utiliser la librairie PolySnd v2.0
#endif

