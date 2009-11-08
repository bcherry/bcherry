// C Source File
// Created 3/13/2005; 5:31:56 PM

#include <tigcclib.h>

const unsigned char square[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

void _main(void) {

   INT_HANDLER int1 = GetIntVec(AUTO_INT_1);
   INT_HANDLER int5 = GetIntVec(AUTO_INT_5);
   SetIntVec(AUTO_INT_1, DUMMY_HANDLER);
   SetIntVec(AUTO_INT_5, DUMMY_HANDLER);
   GrayOn();  ////  everything above is just magic, you need it but
//don't need to understand it.
   clrscr();

   Sprite8(0,0,8,square,GetPlane(0),SPRT_XOR);
   Sprite8(8,0,8,square,GetPlane(1),SPRT_XOR);
   Sprite8(16,0,8,square,GetPlane(0),SPRT_XOR);
   Sprite8(16,0,8,square,GetPlane(1),SPRT_XOR);

   while(!_keytest(RR_ENTER));

   GrayOff();
   SetIntVec(AUTO_INT_1, int1);
   SetIntVec(AUTO_INT_5, int5);

}