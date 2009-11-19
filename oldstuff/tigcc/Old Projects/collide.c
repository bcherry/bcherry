// C Source File
// Created 19/Dec/03; 19:56:34

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files
#include "extgraph.h"

enum OBS {NONE, BALL/*...*/};

typedef struct {
	int type;
	int mass;
	int x;
	int y;
	int x_vec;
	int y_vec;
	void *sprite;
	int h;
	int w;
	// perhaps the sprite used to define or sizes
	// but for now all i need to worry about is two 8x8 balls
} OBJECT;

unsigned char ball[] = {0x3C,0x7E,0xFF,0xFF,0xFF,0xFF,0x7E,0x3C};


// delay program
inline void delay(unsigned int number) {
	unsigned short loop, loop2;
	for (loop=0;loop<number;loop++) {
		for (loop2=0;loop2<10000;loop2++);
	}
}

// Main Function
void _main(void)
{
	//OBJECT ball_1 = {BALL,10,0,0,1,1};
	//OBJECT ball_2 = {BALL,10,152,0,1,1};
	OBJECT objects[] = {{BALL,10,0,0,2,2,ball,8,8},{BALL,20,152,0,1,1,ball,8,8}};
	int i,ii;
	INT_HANDLER interrupt1 = GetIntVec(AUTO_INT_1);
	SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
	clrscr();
	Sprite8(objects[0].x,objects[0].y,objects[0].h,objects[0].sprite,LCD_MEM,SPRT_XOR);
	Sprite8(objects[1].x,objects[1].y,objects[1].h,objects[1].sprite,LCD_MEM,SPRT_XOR);
	while(!(_rowread(~0x40)&0x01)){
		Sprite8(objects[0].x,objects[0].y,objects[0].h,objects[0].sprite,LCD_MEM,SPRT_XOR);
		Sprite8(objects[1].x,objects[1].y,objects[1].h,objects[1].sprite,LCD_MEM,SPRT_XOR);
		for(i=0;i<=1;i++){
			// for _complete_ accuracy i should probably do a for loop here too
			// that would test each point along the way as the object moves
			objects[i].x+=objects[i].x_vec;
			objects[i].y+=objects[i].y_vec;
			if(objects[i].x>=152){objects[i].x=152;objects[i].x_vec*=-1;}
			if(objects[i].x<=0){objects[i].x=0;objects[i].x_vec*=-1;}
			if(objects[i].y>=92){objects[i].y=92;objects[i].y_vec*=-1;}
			if(objects[i].y<=0){objects[i].y=0;objects[i].y_vec*=-1;}
		}
		for(i=0;i<=1;i++){
			for(ii=i+1;ii<=1;ii++){
				if(TestCollide8(objects[i].x,objects[i].y,objects[ii].x,objects[ii].y,8,objects[i].sprite,objects[ii].sprite)){
					//float t1,t2,t2,t4,s1,s2,p1,p2,n1,n2,r;
					//float t1=(float)objects[i].x_vec;
					//float t2=(float)objects[i].y_vec;
					//float t3=(float)objects[ii].x_vec;
					//float t4=(float)objects[ii].y_vec;
					//r=objects[i].mass/objects[ii].mass;
					//float s1=sqrt(objects[i].x_vec*objects[i].x_vec+objects[i].y_vec*objects[i].y_vec);
					//float s2=sqrt(objects[ii].x_vec*objects[ii].x_vec+objects[ii].y_vec*objects[ii].y_vec);
					//float p1=objects[i].mass*sqrt(objects[i].x_vec*objects[i].x_vec+objects[i].y_vec*objects[i].y_vec);
					//float p2=objects[ii].mass*(objects[ii].x_vec*objects[ii].x_vec+objects[ii].y_vec*objects[ii].y_vec);
					//float n1=objects[ii].mass*(objects[ii].x_vec*objects[ii].x_vec+objects[ii].y_vec*objects[ii].y_vec)/objects[i].mass;
					//float n2=objects[i].mass*sqrt(objects[i].x_vec*objects[i].x_vec+objects[i].y_vec*objects[i].y_vec)/objects[ii].mass;
					objects[i].x_vec*=sqrt(objects[i].x_vec*objects[i].x_vec+objects[i].y_vec*objects[i].y_vec)/(objects[ii].mass*(objects[ii].x_vec*objects[ii].x_vec+objects[ii].y_vec*objects[ii].y_vec)/objects[i].mass);
					objects[i].y_vec*=sqrt(objects[i].x_vec*objects[i].x_vec+objects[i].y_vec*objects[i].y_vec)/(objects[ii].mass*(objects[ii].x_vec*objects[ii].x_vec+objects[ii].y_vec*objects[ii].y_vec)/objects[i].mass);
					objects[ii].x_vec*=(objects[ii].x_vec*objects[ii].x_vec+objects[ii].y_vec*objects[ii].y_vec)/(objects[i].mass*sqrt(objects[i].x_vec*objects[i].x_vec+objects[i].y_vec*objects[i].y_vec)/objects[ii].mass);
					objects[ii].y_vec*=(objects[ii].x_vec*objects[ii].x_vec+objects[ii].y_vec*objects[ii].y_vec)/(objects[i].mass*sqrt(objects[i].x_vec*objects[i].x_vec+objects[i].y_vec*objects[i].y_vec)/objects[ii].mass);
					//objects[i].x_vec=(short)t1;
					//objects[i].y_vec=(short)t2;
					//objects[ii].x_vec=(short)t3;
					//objects[ii].y_vec=(short)t4;
				}
			}
		}
		Sprite8(objects[0].x,objects[0].y,objects[0].h,objects[0].sprite,LCD_MEM,SPRT_XOR);
		Sprite8(objects[1].x,objects[1].y,objects[1].h,objects[1].sprite,LCD_MEM,SPRT_XOR);
		delay(20);
	}
	SetIntVec(AUTO_INT_1,interrupt1);	
}
