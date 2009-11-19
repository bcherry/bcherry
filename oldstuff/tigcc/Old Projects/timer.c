// C Source File
// Created 06/Nov/03; 21:06:08
#define SAVE_SCREEN
#define USE_TI89

#include <tigcclib.h>

int _ti89, _ti92plus;  // produce code for TI89 and TI92plus

INT_HANDLER oldint5 = NULL;

volatile int mseconds50 = 0; // counts in 50ms steps (not exact 50ms, but 1000/19 ms)
volatile int seconds    = 0; // counts seconds
volatile int minutes    = 0; // counts minutes
volatile int hours      = 0; // counts hours
volatile int resettime  = 1; // used to signal a reset clock
volatile int running    = 1; // used to signal running or stopped state


DEFINE_INT_HANDLER (myint5handler) {
    if (resettime) {
        mseconds50=seconds=minutes=hours=resettime=0;
    }
    //------------------------------------------
    // count interrupts only if running is not 0
    //------------------------------------------
    else if (running) {
        mseconds50++;
        if (mseconds50 == 20) {
            mseconds50 = 0;
            seconds++;
            if (seconds == 60) {
                seconds = 0;
                minutes++;
                if (minutes == 60) {
                   minutes = 0;
                   hours++;
                   if (hours == 24) hours = 0;
                }
            }
        }
    }
    ExecuteHandler(oldint5);
}


void _main(void) {
    int leave_program = 0;
    
    ClrScr();
    running   = 1;
    resettime = 1;   

    oldint5 = GetIntVec(AUTO_INT_5);
    SetIntVec(AUTO_INT_5, myint5handler);

    //-------------------------------------------
    // print some usage information to the screen
    //-------------------------------------------
    FontSetSys(F_6x8);
    printf_xy(0, 0, "[ESC]   end");
    printf_xy(0,10, "[ENTER] run/stop");
    printf_xy(0,20, "[CLEAR] reset");

    //----------------------------------------------
    // loop until leave_program variable is set to 1
    //----------------------------------------------
    while (!leave_program) {
        //-----------------------------------------------------
        // NOTE that it is not very efficient to draw the clock
        // each time even it has not changed, but we don't
        // bother about this in our simple example !!!
        //-----------------------------------------------------
        FontSetSys(F_8x10);
        if (TI89) printf_xy(30, 50, "%02d:%02d:%02d.%03d",hours,minutes,seconds,mseconds50*50);
        else      printf_xy(70, 64, "%02d:%02d:%02d.%03d",hours,minutes,seconds,mseconds50*50);
				FontSetSys(F_6x8);
				
        if (kbhit()) {
            int input = ngetchx();
            switch(input) {
                //----------------------------------------------------
                // set leave_program to 1 if user has pressed [ESCAPE]
                //----------------------------------------------------
                case KEY_ESC: 
                    leave_program = 1;
                    break;
                //----------------------------------------------------
                // toggle running state if user has pressed [ENTER]
                //----------------------------------------------------
                case KEY_ENTER: 
                    running = !running;
                    break;
                //------------------------------------------------------------
                // set the resettime variable to 1 if user has pressed [CLEAR]
                // to signalize the interrupt handler to reset the clock
                // variables
                //------------------------------------------------------------
                case KEY_CLEAR:
                    resettime = 1;
                    break;
            }
        }
    }

    SetIntVec(AUTO_INT_5, oldint5);
    GKeyFlush();
}
