// C Source File
// Created 1/13/2004; 3:54:21 PM

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files
#include "extgraph.h"
#include "tictac.h"

// the array of possible winning combinations
unsigned short win_check[] = {0b111000000,0b000111000,0b000000111,0b100100100,0b010010010,0b001001001,0b100010001,0b001010100};
// the array of the board, in three parts: the x's, the o's and the master with both.  obviously this should be defaulted to empty...
unsigned short maps[] = {0,0,0};
unsigned short wins[] = {0,0,0};
// the number used to designate the current player
short player = 1;
// a structure for position
typedef struct {
	char row;
	char col;
	char str;
} POSITION;
POSITION pos = {0,0,0};
// the virtual screens
void *v_screen_l=NULL;
void *v_screen_d=NULL;
// the extra int handler to back up int1, because of use of atexit, this needs to be global
// for now we will initialize it to NULL
INT_HANDLER interrupt1=NULL;
INT_HANDLER interrupt5=NULL;
// the prototypes
void doMove(void);
void doWin(void);
void doCat(void);
void postMark(short,short,char);
void drawGrid(void);
void flushMaps(void);
void doAgain();
void drawTitle(char);
//void doAI(void);
//void postStats(void);
// the function that will be called upon normal termination of the program
atexit_t exitStuff(void){
	player=0;
	GrayOff();
	SetIntVec(AUTO_INT_1,interrupt1);
	SetIntVec(AUTO_INT_5,interrupt5);
	free(v_screen_l);
	free(v_screen_d);
	return 0;
}
// a function that handles the processing of a move selection and tests win conditions
void doMove(void){
	int i;
	// if there is nothing already in the spot...
	if(!(0b100000000>>(pos.row*3+pos.col)&maps[2])){
		// update both the current player's map and the master map
		maps[player%2]^=0b100000000>>(pos.row*3+pos.col);
		maps[2]^=0b100000000>>(pos.row*3+pos.col);
		// draw the "x" or "o", and erase the "player:" symbol
		postMark(pos.col*20+4,pos.row*20+4,player%2);
		postMark(120,30,2);
		// test to see if this player has just won...
		for(i=0;i<=7;i++){
			if((maps[player%2]&win_check[i])==win_check[i]){
				int ii;
				pos.row=pos.col=0;
				for(ii=0;ii<=8;ii++){
					if(0b100000000>>ii&win_check[i])postMark(pos.col*20+4,pos.row*20+4,3);
					pos.col++;
					if(pos.col>2){pos.col=0;pos.row++;}
				}
				doWin();
			}
		}
		// test if the board is full, which means a cats' game
		// note that i dont need to check that no one has one, because if i got this far without already
		// discovering a winner, then no one has won
		if(maps[2]==511)doCat();
		// advance the player number
		// this number will go up, and i use the modulus (%) with 2 see whose turn it is, even is "o", odd is "x"
		player++;
	}
}
// the following AI code is unfinished
/*void doAI(void){
	int i;
	int ii;
	POSITION tests[] = {{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0},{0,0,0}};
	for(i=0;i<=7;i++){
		for(ii=0;ii<=8;ii++){
			if(!(0b100000000>>(tests[i].row*3+tests[i].col)&maps[2])){
				if((maps[player%2]^0b100000000>>(tests[i].row*3+tests[i].col))
			}
			tests[i].col++;
			if(tests[i].col>2){tests[i].col=0;tests[i].row++;}
		}
	}
}*/
// a function that displays the winner and then exits, the "doAgain" function seems to have some unfixed bugs that
// i cant explain, so for now i will not use tha, and thus each time you run the program, you will only get one game
// i would like to fix this, so the original "doAgain" function is at the end of the code
// to use it, just comment out the lines in the next two function that have the "/**/" on each side and remove
// the comments on the doAgain(); line.  Thats all you need to use it.  I hope someone will be able to fix the errors for me :)
void doWin(void){
	FontSetSys(F_8x10);
	postMark(80,0,player%2);
	wins[player%2]++;
	DrawStr(100,3,"WINS!",A_NORMAL);
	//postStats();
	/**/while(!_keytest(RR_ESC));/**/
	//doAgain();
}
void doCat(void){
	FontSetSys(F_8x10);
	wins[2]++;
	DrawStr(72,3,"CATS' GAME!",A_NORMAL);
	//postStats();
	/**/while(!_keytest(RR_ESC));/**/
	//doAgain();
}
/*void postStats(void){
	DrawStr(72,
}*/
// a function that draws a mark on the coordiates given, a 0 is "o", a 1 is "x", and a 2 is " "
void postMark(short x, short y, char mark){
	switch(mark){
		case 0:
			GraySprite16_OR(x,y,16,o_l,o_d,GetPlane(0),GetPlane(1));
			break;
		case 1:
			GraySprite16_OR(x,y,16,x_l,x_l,GetPlane(0),GetPlane(1));
			break;
		case 2:
			GraySprite16_AND(x,y,16,blank,blank,GetPlane(0),GetPlane(1));
			break;
		case 3:
			GraySprite16_OR(x,y,16,full,full,GetPlane(0),GetPlane(0));
			break;
	}	
}
// a function to draw the title screen and background
void drawTitle(char n){
	int x,y;
  int tilewidth  = 32;
  int tileheight = 10;
  int vcount     = 10;
  int hcount     = 5;
  int i;
  if(n==1){
  	memset(v_screen_l,0xff,LCD_SIZE);
  	memset(v_screen_d,0xff,LCD_SIZE);
    for (y=0;y<vcount;y++) {
    	for (x=0;x<hcount;x++) { 
      	int offset = (y*hcount+x)*tileheight;
        	Sprite32(x*tilewidth,y*tileheight,tileheight,&title[0][offset],v_screen_l,SPRT_XOR);
        	Sprite32(x*tilewidth,y*tileheight,tileheight,&title[1][offset],v_screen_d,SPRT_XOR);
      }
   	}
	} else if(n==2){
  	memset(v_screen_l,0x00,LCD_SIZE);
   	memset(v_screen_d,0x00,LCD_SIZE);
    for (y=0;y<vcount;y++) {
    	for (x=0;x<hcount;x++) { 
     		int offset = (y*hcount+x)*tileheight;
     	  for (i=0;i<2;i++) {
     	    Sprite32(x*tilewidth,y*tileheight,tileheight,&title[i][offset],v_screen_l,SPRT_OR);
     	  }
     	}
		}
	}
	FastCopyScreen(v_screen_l,GetPlane(0));
	FastCopyScreen(v_screen_d,GetPlane(1));
}
// draws the grid
void drawGrid(void){
	GraySprite32_OR(4,20,4,horiz_grid_l,horiz_grid_l,GetPlane(0),GetPlane(1));
	GraySprite32_OR(36,20,4,horiz_grid_r,horiz_grid_r,GetPlane(0),GetPlane(1));
	GraySprite32_OR(4,40,4,horiz_grid_l,horiz_grid_l,GetPlane(0),GetPlane(1));
	GraySprite32_OR(36,40,4,horiz_grid_r,horiz_grid_r,GetPlane(0),GetPlane(1));
	GraySprite8_OR(20,4,56,vert_grid,vert_grid,GetPlane(0),GetPlane(1));
	GraySprite8_OR(40,4,56,vert_grid,vert_grid,GetPlane(0),GetPlane(1));
}
// resets the maps to empty
void flushMaps(void){
	int i;
	for(i=0;i<=2;i++){
		maps[i]=0;
	}
}

// Main Function
void _main(void)
{
	// this if statement only matters if using "doAgain", as the stuff inside should only be called once during execution
	if(player<2){
		atexit((atexit_t)exitStuff);
		v_screen_l=malloc(3840);
		v_screen_d=malloc(3840);
		interrupt1=GetIntVec(AUTO_INT_1);
		interrupt5=GetIntVec(AUTO_INT_5);
		SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
		SetIntVec(AUTO_INT_5,DUMMY_HANDLER);
		GrayOn();
		randomize();
		player=random(2);
		drawTitle(1);
		while(!_keytest(RR_ENTER));
	}
	drawTitle(2);
	flushMaps();	
	drawGrid();
	DrawStr(110,20,"Player:",A_REVERSE);
	player++;
	while(1){
		//pos.row=pos.col=-1;
		postMark(120,30,player%2);
		if(_keytest(RR_1)){pos.row=2;pos.col=0;doMove();}
		else if(_keytest(RR_2)){pos.row=2;pos.col=1;doMove();}
		else if(_keytest(RR_3)){pos.row=2;pos.col=2;doMove();}
		else if(_keytest(RR_4)){pos.row=1;pos.col=0;doMove();}
		else if(_keytest(RR_5)){pos.row=1;pos.col=1;doMove();}
		else if(_keytest(RR_6)){pos.row=1;pos.col=2;doMove();}
		else if(_keytest(RR_7)){pos.row=0;pos.col=0;doMove();}
		else if(_keytest(RR_8)){pos.row=0;pos.col=1;doMove();}
		else if(_keytest(RR_9)){pos.row=0;pos.col=2;doMove();}
		else if(_keytest(RR_CLEAR)){player++;postMark(120,30,2);}
		else if(_keytest(RR_ESC))exit(0);
		//if(pos.row>=0&&pos.col>=0)doMove();
	}
}
// the function for replaying the game withou quitting
// it doesnt work quite right
void doAgain(void){
	DrawStr(80,20,"Play Again?",A_REVERSE);
	DrawStr(80,28,"Enter - Yes",A_REVERSE);
	DrawStr(80,36,"ESC - No",A_REVERSE);
	while(!_keytest(RR_ENTER)&&!_keytest(RR_ESC));
	if(_keytest(RR_ENTER))_main();
	else exit(0);
}