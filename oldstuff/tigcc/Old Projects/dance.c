// C Source File
// Created 16/Dec/03; 20:11:26

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files
#include "extgraph.h"					// Include the extgraph library
#include "dance.h"						// Include the "dance" header file

enum places{UP_X=80,DOWN_X=90,LEFT_X=100,RIGHT_X=110};
static int steps[110];
enum dirs{UP,DOWN,LEFT,RIGHT};
unsigned short step=0;
unsigned short score=0;

// delay program
inline void delay(unsigned int number) {
	unsigned short loop, loop2;
	for (loop=0;loop<number;loop++) {
		for (loop2=0;loop2<10000;loop2++);
	}
}

inline void fillSteps(void){
	int i;
	for (i=0;i==109;i++){
		steps[i]=0;
	}
}

inline void drawGoal(void){
	Sprite8(UP_X,5,8,up_inv,LCD_MEM,SPRT_XOR);
	Sprite8(DOWN_X,5,8,down_inv,LCD_MEM,SPRT_XOR);
	Sprite8(LEFT_X,5,8,left_inv,LCD_MEM,SPRT_XOR);
	Sprite8(RIGHT_X,5,8,right_inv,LCD_MEM,SPRT_XOR);
}

inline void pushStep(int dir){
	if (dir==UP){steps[step]=KEY_UP;Sprite8(UP_X,92,8,up_arrow,LCD_MEM,SPRT_XOR);}
	else if(dir==DOWN){steps[step]=KEY_DOWN;Sprite8(DOWN_X,92,8,down_arrow,LCD_MEM,SPRT_XOR);}
	else if(dir==LEFT){steps[step]=KEY_LEFT;Sprite8(LEFT_X,92,8,left_arrow,LCD_MEM,SPRT_XOR);}
	else if(dir==RIGHT){steps[step]=KEY_RIGHT;Sprite8(RIGHT_X,92,8,right_arrow,LCD_MEM,SPRT_XOR);}
}

inline void danceTest(int key){
	drawGoal();
	unsigned char test_sprite[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	if(key==KEY_UP){
		Sprite8Get(UP_X,5,8,LCD_MEM,test_sprite);
		if(TestCollide8(UP_X,5,UP_X,5,8,test_sprite,up_arrow)){score++;printf_xy(0,10,"hit up");}
	} else if(key==KEY_DOWN){
		Sprite8Get(DOWN_X,5,8,LCD_MEM,test_sprite);
		if(TestCollide8(DOWN_X,5,DOWN_X,5,8,test_sprite,down_arrow)){score++;printf_xy(0,10,"hit down");}
	} else if(key==KEY_LEFT){
		Sprite8Get(LEFT_X,5,8,LCD_MEM,test_sprite);
		if(TestCollide8(LEFT_X,5,LEFT_X,5,8,test_sprite,left_arrow)){score++;printf_xy(0,10,"hit left");}
	} else if(key==KEY_RIGHT){
		Sprite8Get(RIGHT_X,5,8,LCD_MEM,test_sprite);
		if(TestCollide8(RIGHT_X,5,RIGHT_X,5,8,test_sprite,right_arrow)){score++;printf_xy(0,10,"hit right");}
	}
	drawGoal();
}

// Main Function
void _main(void)
{
	unsigned short key=0;
	void	*kbq=kbd_queue();
	fillSteps();
	int timer=0;
	score=0;
	step=0;
	randomize();
	clrscr();
	drawGoal();
	while(key!=KEY_ESC){
		timer++;
		if(!OSdequeue(&key,kbq)){
			if(key==KEY_ESC){printf("%d",score);ngetchx();break;}
			else danceTest(key);
		}
		drawGoal();
		ScrollUp240(LCD_MEM,100);
		drawGoal();
		if(timer>=random(15)+15){
			timer=0;
			step++;
			if(step<=100)pushStep(random(4));
			else if(step>105){printf_xy(0,0,"%d",score);ngetchx();break;}			
		}
		
		delay(15);
	}
}
