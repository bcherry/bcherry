/******************************************************************************
*
* project name:    ExtGraph
* initial date:    12/04/2001
* author:          thomas.nussbaumer@gmx.net
* description:     test program for scrolling functions
*
* $Id: demo1.c,v 1.6 2002/02/25 20:43:00 tnussb Exp $
*
*******************************************************************************/

#define NO_EXIT_SUPPORT
#define OPTIMIZE_ROM_CALLS
#define USE_TI89 
#if defined(USE_TI89) && defined(USE_TI92P)
#error cannot produce both versions at once
#elif defined(USE_TI89)
#define C89_92(x,y) (x)
#elif defined(USE_TI92P)
#define C89_92(x,y) (y)
#else
/*#error Please specify either -DUSE_TI89 or -DUSE_TI92P on
commandline*/
#endif

#include <tigcclib.h>
#include "extgraph.h"
  // NOTE: this path is just for this demo !!
                                 //       if the extgraph library is correctly
                                 //       installed you should use:
                                 //
                                 //       #include <extgraph.h>


//=============================================================================
// simple demo program which demonstrates an earthquake effect
//
// can you screenshot this effect ??? ;-)
//=============================================================================
void _main(void) {
    LCD_BUFFER screen;
    short      old_random = 0;
    short      new_random = 0;

    if (!CheckHWMatch()) {
        ST_helpMsg("ERROR: can only run on a "DESIRED_CALCTYPE);
        return;
    }

    LCD_save(screen);

    OSSetSR(0x700);

    do {
        old_random = new_random;
        do {
            new_random = random(4);
        }
        while (new_random == old_random);


        //---------------------------------------------------------------------
        // if/else if/else chain produces smaller code than case !!
        //---------------------------------------------------------------------

#if defined(USE_TI89)
        if (!new_random)        ScrollLeft160(LCD_MEM,100);
        else if (new_random==1) ScrollRight160(LCD_MEM,100);
        else if (new_random==2) ScrollUp160(LCD_MEM,100);
        else                    ScrollDown160(LCD_MEM,100);
#else
        if (!new_random)        ScrollLeft240(LCD_MEM,128);
        else if (new_random==1) ScrollRight240(LCD_MEM,128);
        else if (new_random==2) ScrollUp240(LCD_MEM,128);
        else                    ScrollDown240(LCD_MEM,128);
#endif
    }
    while (!(_rowread(~1<<9)&0xFF));

    OSSetSR(0);

    LCD_restore(screen);
    GKeyFlush();
    ST_showHelp(EXTGRAPH_VERSION_PWDSTR);
}


//#############################################################################
// Revision History
//#############################################################################
//
// $Log: demo1.c,v $
// Revision 1.6  2002/02/25 20:43:00  tnussb
// is build now for each calctype separately to reduce size(Lionel Debroux)
//
// Revision 1.5  2002/02/22 15:06:13  tnussb
// outputs now the "powered by ExtGraph" string at the end of the program
//
// Revision 1.4  2002/02/22 13:04:24  tnussb
// (1) Bugfix: bad _rowread ! (doesn't suit on a TI-92P) (Lionel Debroux)
// (2) trap 12 usage replaced again with save OSSetSR()
//
// Revision 1.3  2002/02/11 10:30:42  tnussb
// (1) uses now USE_TI89/USE_TI92P directives (Lionel Debroux)
// (2) disables all interrupts for speed + direct keyboard testing (Lionel Debroux)
//
// Revision 1.2  2002/02/11 10:14:26  tnussb
// compiler directives NO_EXIT_SUPPORT and OPTIMIZE_ROM_CALLS added
//
// Revision 1.1  2001/06/20 21:50:19  Thomas Nussbaumer
// initial check-in

