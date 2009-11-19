// This contains the HW2 status.
// 0 for HW1s, 1 for HW2s. This is used because they are
// different for sound.
#include <tigcclib.h>

short hardwarever;

// This works through Auto-Int-5, as an always-going-counter.
volatile short count;
DEFINE_INT_HANDLER(count_int)
{
++count;
}


//=======================================================
// VTI detection routine (thanx, Julien Muchembled)
//
// this routine utilizes a bug in VTI: you can set even non-existing flags in
// the status register and query them later!
//========================================================
extern short IsVTI(void);
asm("IsVTI: trap #12; /* enter supervisor mode. returns old (%sr) in %d0.w */ move.w #0x3000,%sr; /* set a non-existing flag in %sr (but keep s-flag !!) */ swap %d0; /* save %d0.w content in upper part of %d0 */ move.w %sr,%d0; /* get %sr content and check for non-existing flag */ btst #12,%d0; /* this non-existing flag can only be set on the VTI */ bne __VTI; swap %d0; /* restore old %sr content and return 0 */ move.w %d0,%sr; moveq #0,%d0; rts;__VTI: swap %d0; /* restore old %sr content and return 1 */ move.w %d0,%sr; moveq #1,%d0; rts;");
short VTI;

//Duration is in seconds*350.
void rest(long duree)
{
short start = count;
while (count - start < duree);
}

//Thanks to Francois Leiber for the beep() code.
//However, it has been slightly modified with ASM
//so it works with VTI's sound feature.
//Duration is in a special form:  (5400/tempo)*duration
//(5400 is 27x200, i.e. a quarter note is 27 long at tempo 200)
//Tempo is in Beats Per Minute.
//Duration is the note length in relation to a quarter note.
//For example, an eighth note is 0.5, a quarter note 1, a whole note 4, etc.
void beep(short freq, long duree)
{

long int loop, maxloop ;
short int delay;
short int i, j;
if(VTI) duree=(duree*15)>>4;
maxloop = freq*duree/50;
j = peekIO(0x60000E);
freq = (hardwarever ? 364000 : 278000)/freq ;
asm("bset.b #6,0x60000C");
for (loop = 0 ; loop < maxloop ; loop++)
{
asm("bchg.b #0,0x60000E;bchg.b #1,0x60000E;");
for (i = 0 ; i <= freq ; i--) i += 2 ;
}
asm("bclr.b #0,0x60000E;bclr.b #1,0x60000E;move.b #0x8D,0x60000C;");
for(delay=0;delay<(100*duree);delay+=2) delay--;
}

//An example of a main function that uses the beep code (from Midi68k)
void _main(void)
{
INT_HANDLER save_int_4 = GetIntVec(AUTO_INT_4);
INT_HANDLER save_int_1 = GetIntVec(AUTO_INT_1);
INT_HANDLER save_int_5 = GetIntVec(AUTO_INT_5);
short int total = sizeof(music)/sizeof(music[0]); //Determines the # elements in the music array
short int current=0;
/* Checks the calculator hardware version */
short *hwptr;
hwptr = (short *)FL_getHardwareParmBlock();
hardwarever = ((*hwptr)==20)?0:1;
/*-----------------------------------------*/
VTI=IsVTI();
//Directs ints to nothing
SetIntVec(AUTO_INT_4, DUMMY_HANDLER);
SetIntVec(AUTO_INT_5, DUMMY_HANDLER);
//Auto Int 1 is called 350 times per second.
SetIntVec(AUTO_INT_1, count_int);
//Get the link port ready
OSLinkReset();
OSLinkOpen();
clrscr();
//Display some information
printf("\n\nBeethoven-Moonlight Sonata");
printf("\n\n\nCreated by Midi68k\nBy Malcolm Smith\nhttp://jesystems.cjb.net\n\nHOLD Esc to Quit.");

while( !(_keytest (RR_ESC)) && current<total)
{
//If the frequency is 0, it is a rest.
if(!music[current][1]) rest(music[current][0]);
//Otherwise, play the note
else if(music[current][0]>0) beep(music[current][1],music[current][0]);
current++;
}
SetIntVec(AUTO_INT_4, save_int_4);
SetIntVec(AUTO_INT_1, save_int_1);
SetIntVec(AUTO_INT_5, save_int_5);
OSLinkReset();
GKeyFlush();
OSTimerRestart(APD_TIMER);
}
