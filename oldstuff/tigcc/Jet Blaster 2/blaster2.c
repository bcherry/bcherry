/*				JET BLASTER MACH 2
			This is source is open to you
			to modify, reproduce, etc. as
			long as credit is given where
			credit is due.  All code contained
			herein is credited to Ben Cherry,
			unless otherwise specified.
				Created January 2004 by Ben Cherry      */

//#define OPTIMIZE_ROM_CALLS
#define SAVE_SCREEN
#include <tigcclib.h> //Includes the TIGCC library
#include "blaster2.h" //Includes the header file "blaster2", which contains sprites
#include "extgraph.h" //Includes the extgraph library
#include "c_sprites.h" //Includes a library of special sprite routines
short _ti89;
short _ti92plus;
/*#### ENUMERATIONS & DEFINES ####*/
#define JET_START_X 1
#define JET_START_Y 3
#define BOOST_SPEED 3
#define LEVEL_LENGTH 160
#define CANNON_SPEED 6
#define MISSILE_SPEED 4
#define BLOCK_VALUE 50
enum shotTypes{NONE,UP_MISSILE,DOWN_MISSILE,CANNON,EX_1,EX_2,EX_3,EX_4,EX_5,EX_6};
enum blockTypes{CASH_100=-6,CASH_250,CASH_500,CASH_1000,SHIELD,BOOST,NONE2,BROKEN_2,BROKEN_1,REGULAR,INDESTRUCTIBLE,MINE_1,MINE_2,MINE_3,MINE_4,MINE_5,MINE_6,MINE_7,MINE_8,MINE_9,MINE_10,MINE_11,MINE_12,MINE_13,MINE_14,MINE_15,MINE_16,MINE_17,MINE_18};
enum speeds{NONE3,SLOW,NORMAL,FAST};
enum places{NONE4,PLAY,OPTIONS,HELP,EXIT,SPEED,BACK,WINGS,THRUST,RAPID,WEAPONS,SPECIAL,JET_MOD,MISSILE,QUIT,YES,NO,NEW,CON};
/*############### GLOBAL VARIABLES & TYPEDEFS #################*/
typedef struct {
	short x;// array x-coordinate
	short y;// array y-coordinate
	short xo;// x offset
	short yo;// y offset
	unsigned short cannon;// cannon strength, 1-4
	unsigned short missile;// missile strength, 0-2
	short jc;// used to limit rapid fire speed for cannon
	short jm;// used to limit rapid fire speed for missiles
	unsigned short boost;// amount of boost in tanks
	unsigned short boost_max;// boost tank capacity, 0,50, or 100
	unsigned char exhaust;// variable used to animate the exhaust, EX_1-EX_6
	unsigned char shield;// shield strength
	unsigned char shield_max;// shield capacity
	unsigned char move_rate_v;// number of pixels the jet moves vertically, 2 or 4
	unsigned char move_rate_h;// number of pixels the jet moves horizontally, 2 or 4
	unsigned char repeat_rate_c;// number of cycles before fire again for cannon
	unsigned char repeat_rate_m;// number of cycles before fire again for missiles
	unsigned long cash;// the player's money supply, 0-999999
	unsigned long score;// the player's score, 0-big
} JET;
typedef struct {
	short x;
	short y;
	short xo;
	short yo;
	unsigned short type;
} SHOT;
typedef struct {
	short x;// array x-coordiate in the level
	short o;// offset of the screen
	short lvl;// level number, 0-9
} PTR;
typedef struct {
	char f1;
	char f2;
	char f3;
	char f4;
	char f5;
	char second;
	char shift;
	char esc;
	char up;
	char down;
	char left;
	char right;
	char diamond;
	char alpha;
	char clear;
	char enter;
	char plus;
	char minus;
} KEYS;
typedef struct {
	unsigned long score;
	char name[10];
} HI_SCORE;
typedef struct {
	JET jet;
	PTR ptr;
	HI_SCORE hiScores[6];
	char speed;
} SAVE;
SAVE save;
KEYS keys;
JET jet;
SHOT shot_stack[25];// an array to put shots in
SHOT ex_stack[25];// an array to put big explosions in
char death_marker=0;// this variable is wierd, but using it fixes bugs...
char map[11][LEVEL_LENGTH];// the map
PTR ptr={0,0,1};// the map pointer
char speed=NORMAL;// the game speed, Slow,Normal, or Fast
INT_HANDLER interrupt1=NULL;// to store auto-int-1 in
INT_HANDLER interrupt5=NULL;// to store auto-int-5 in
/*########## PROTOTYPES (in alphabetical order) #############*/
void clearScreens(void);// clears the virtual screens
void delay(short);// slows the program down
void doDeath(void);// called when the ship crashes
void doGame(void);// the true main function, is actually a copy of the original _main function, but i decided to have doGame instead so i can call it repeatedly, and end up with a spiral of death as more and more functions get nested and layered together, something i never wanted to do, but had to do due to bug problems, and i guess it is better this way...
void doHelp(void);// displays the help screen
void doJet(void);// moves and collison detects the jet
void doJetModShop(void);// displays the Jet Mod Shop
void doMenu(void);// Displays the main menu
void doMines(void);// runs through the explosions and does them
void doOptions(void);// displays the options menu
void doPause(void);// pauses the game
void doQuit(void);// brings up the "are you sure you want to quit?" menu
void doShots(void);// manages missiles and cannons
void doSpecialShop(void);// displays the Special Shop
void doWeaponShop(void);// Displays the weapons shop
void doWin(void);// called at the end of the game
void drawMap(void);// draws the blocks onto the screen
void drawStats(void);// draws the stat bar on the bottom of the screen
void flushStacks(void);// empties the shot and explosion stacks
void gotoShop(void);// displays the Jet Shop
void initialize(void);// called at the beginning of the program
void loadGame(void);
short makePickup(short);// makes a pickup when a block is destroyed
void newHiScore(void);
void prepareGame(void);
void pushMine(short,short,short,short);// pushes an explosion to the stack when a mine is destroyed
void randomizeMap(void);// makes a random map
void resetGame(void);// resets the jet and everything to default
void saveGame(void);
void shiftMap(void);// scrolls the level
void showHiScores(void);
void testOffsetJet(void);// manages the offset for the jet
SHOT testOffsetShot(SHOT);// manages the offset for a given shot
/*###################### THE _MAIN FUNCTION #########################*/
void _main(void){
	initialize();
//	INTRO_OrageStudio();
	doMenu();
}
/* This function is the real heart of the program */
void doGame(void){
	delay(100);// the 100 delay is usually enough to stop any keypresses and keep them from interfering with another function
						 // as such, i call it at the beginning and end of EVERY function that involves _keytest()
	jet.x=JET_START_X,jet.xo=0,
	jet.y=JET_START_Y,jet.yo=0,
	ptr.x=0,ptr.o=0,
	jet.boost=jet.boost_max,
	jet.shield=jet.shield_max,
	jet.jc=0,jet.jm=0,
	jet.exhaust=EX_1;
	keys=(KEYS){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	flushStacks();
	clearScreens();
	randomizeMap();
	while(1){
		shiftMap();
		drawMap();
		drawStats();
		doShots();
		doMines();
		doJet();
		FastCopyScreen(light_vscreen,GetPlane(0));
		FastCopyScreen(dark_vscreen,GetPlane(1));
		clearScreens();
		if(_keytest(RR_ESC)||keys.esc)gotoShop();
		if(_keytest(RR_F1)||keys.f1)doPause();
		if(_keytest(RR_F5)||keys.f5)exit(0);
		if(_keytest(RR_PLUS)||keys.plus)OSContrastUp();
		if(_keytest(RR_MINUS)||keys.minus)OSContrastDn();
		if(_keytest(RR_CLEAR)||keys.clear){ptr.lvl++;gotoShop();}// this is a cheat
		if(_keytest(RR_F2))keys.second=1;
		if(_keytest(RR_F3)){jet.score=200000;doWin();}
		if(death_marker)doDeath();
	}
}
/*################# The Main Loop Functions in Order ###############*/
void shiftMap(void){
	ptr.o+=speed;
	jet.xo+=speed;
	if((_keytest(RR_ALPHA)||keys.alpha)&&jet.boost){ptr.o+=BOOST_SPEED;jet.xo+=BOOST_SPEED;}
	testOffsetJet();
	while(ptr.o>=8){ptr.o-=8;ptr.x++;}
	while(ptr.o<0){ptr.o+=8;ptr.x--;}
	if(ptr.x>LEVEL_LENGTH){
		if(++ptr.lvl>9)doWin();
		gotoShop();
	}
}
void drawMap(void){
	int i,ii;
	for(i=ptr.x;i<=ptr.x+20;i++){
		if(i>LEVEL_LENGTH-1)break;
		for(ii=0;ii<=10;ii++){
				if(map[ii][i]){
					if(map[ii][i]<=CASH_1000)m_Sprite8((i-ptr.x)*8-ptr.o,ii*8,8,cash_pickup,light_vscreen,dark_vscreen,SPRT_XOR);
					else if(map[ii][i]==SHIELD)m_Sprite8((i-ptr.x)*8-ptr.o,ii*8,8,shield_pickup,light_vscreen,dark_vscreen,SPRT_XOR);
					else if(map[ii][i]==BOOST)m_Sprite8((i-ptr.x)*8-ptr.o,ii*8,8,boost_pickup,light_vscreen,dark_vscreen,SPRT_XOR);
					else if(map[ii][i]==BROKEN_2)m_Sprite8((i-ptr.x)*8-ptr.o,ii*8,8,broken_2,light_vscreen,dark_vscreen,SPRT_XOR);
					else if(map[ii][i]==BROKEN_1)m_Sprite8((i-ptr.x)*8-ptr.o,ii*8,8,broken_1,light_vscreen,dark_vscreen,SPRT_XOR);
					else if(map[ii][i]==REGULAR)m_Sprite8((i-ptr.x)*8-ptr.o,ii*8,8,regular,light_vscreen,dark_vscreen,SPRT_XOR);
					else if(map[ii][i]==INDESTRUCTIBLE)m_Sprite8((i-ptr.x)*8-ptr.o,ii*8,8,indestructible,light_vscreen,dark_vscreen,SPRT_XOR);
					else if(map[ii][i]<=MINE_6)m_Sprite8((i-ptr.x)*8-ptr.o,ii*8,8,mine_1,light_vscreen,dark_vscreen,SPRT_XOR);
					else if(map[ii][i]<=MINE_12)m_Sprite8((i-ptr.x)*8-ptr.o,ii*8,8,mine_2,light_vscreen,dark_vscreen,SPRT_XOR);
					else if(map[ii][i]<=MINE_18)m_Sprite8((i-ptr.x)*8-ptr.o,ii*8,8,mine_3,light_vscreen,dark_vscreen,SPRT_XOR);
					if(map[ii][i]>=MINE_1){if(++map[ii][i]>MINE_18)map[ii][i]=MINE_1;}
			}
		}
	}
}
void doJet(void){
	int i,ii;
	int boosting=((_keytest(RR_ALPHA)||keys.alpha)&&jet.boost);
	if(_keytest(RR_UP)||keys.up){jet.yo-=jet.move_rate_v;testOffsetJet();if(jet.y*8+jet.yo<0)jet.y=0,jet.yo=0;}
	if(_keytest(RR_DOWN)||keys.down){jet.yo+=jet.move_rate_v;testOffsetJet();if(jet.y*8+jet.yo>80)jet.y=10,jet.yo=0;}
	if((_keytest(RR_LEFT)||keys.left)&&!boosting){jet.xo-=jet.move_rate_h;testOffsetJet();if(jet.x*8+jet.xo<(ptr.x+1)*8+ptr.o)jet.x=ptr.x+1,jet.xo=ptr.o;}
	if((_keytest(RR_RIGHT)||keys.right)&&!boosting){jet.xo+=jet.move_rate_h;testOffsetJet();if(jet.x*8+jet.xo>(ptr.x+18)*8+ptr.o)jet.x=ptr.x+18,jet.xo=ptr.o;}
	if(_keytest(RR_UP)||keys.up)m_Sprite16((jet.x-ptr.x)*8+(jet.xo-ptr.o),jet.y*8+jet.yo,8,jet_up,light_vscreen,dark_vscreen,SPRT_XOR);
	if(_keytest(RR_DOWN)||keys.down)m_Sprite16((jet.x-ptr.x)*8+(jet.xo-ptr.o),jet.y*8+jet.yo,8,jet_down,light_vscreen,dark_vscreen,SPRT_XOR);
	if(!_keytest(RR_UP)&&!keys.up&&!_keytest(RR_DOWN)&&!keys.down)m_Sprite16((jet.x-ptr.x)*8+(jet.xo-ptr.o),jet.y*8+jet.yo,8,jet_reg,light_vscreen,dark_vscreen,SPRT_XOR);
	if(boosting){
		if(jet.exhaust<=EX_2){
		//	m_Sprite16((jet.x-ptr.x)*8+(jet.xo-ptr.o),jet.y*8+jet.yo-4,16,boost_overlay_1,light_vscreen,dark_vscreen,SPRT_XOR);
			m_Sprite16((jet.x-ptr.x)*8+(jet.xo-ptr.o)-16,jet.y*8+jet.yo,8,boost_exhaust_1,light_vscreen,dark_vscreen,SPRT_XOR);
		}
		else if(jet.exhaust<=EX_4){
		//	m_Sprite16((jet.x-ptr.x)*8+(jet.xo-ptr.o),jet.y*8+jet.yo-4,16,boost_overlay_2,light_vscreen,dark_vscreen,SPRT_XOR);
			m_Sprite16((jet.x-ptr.x)*8+(jet.xo-ptr.o)-16,jet.y*8+jet.yo,8,boost_exhaust_2,light_vscreen,dark_vscreen,SPRT_XOR);
		}
		else if(jet.exhaust<=EX_6){
		//	m_Sprite16((jet.x-ptr.x)*8+(jet.xo-ptr.o),jet.y*8+jet.yo-4,16,boost_overlay_3,light_vscreen,dark_vscreen,SPRT_XOR);
			m_Sprite16((jet.x-ptr.x)*8+(jet.xo-ptr.o)-16,jet.y*8+jet.yo,8,boost_exhaust_3,light_vscreen,dark_vscreen,SPRT_XOR);
		}
		jet.boost--;
	} else {
		if(jet.exhaust<=EX_2)m_Sprite8((jet.x-ptr.x)*8+(jet.xo-ptr.o)-8,jet.y*8+jet.yo,8,reg_exhaust_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(jet.exhaust<=EX_4)m_Sprite8((jet.x-ptr.x)*8+(jet.xo-ptr.o)-8,jet.y*8+jet.yo,8,reg_exhaust_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(jet.exhaust<=EX_6)m_Sprite8((jet.x-ptr.x)*8+(jet.xo-ptr.o)-8,jet.y*8+jet.yo,8,reg_exhaust_3,light_vscreen,dark_vscreen,SPRT_XOR);
	}
	if(jet.shield&&!boosting){
		if(jet.exhaust<=EX_2)m_Sprite16((jet.x-ptr.x)*8+(jet.xo-ptr.o)+4,jet.y*8+jet.yo-4,16,shield_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(jet.exhaust<=EX_4)m_Sprite16((jet.x-ptr.x)*8+(jet.xo-ptr.o)+4,jet.y*8+jet.yo-4,16,shield_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(jet.exhaust<=EX_6)m_Sprite16((jet.x-ptr.x)*8+(jet.xo-ptr.o)+4,jet.y*8+jet.yo-4,16,shield_3,light_vscreen,dark_vscreen,SPRT_XOR);
	}
	if(++jet.exhaust>EX_6)jet.exhaust=EX_1;
	char shield_down=0;
	for(i=0;i<=2;i++){
		if(jet.x+i>LEVEL_LENGTH-1)continue;
		if(i==2&&jet.xo==0)break;
		for(ii=0;ii<=1;ii++){
			if(ii==1&&jet.yo==0)break;
			if(jet.y+ii>10)continue;
			if(map[jet.y+ii][jet.x+i]>0)
			{
				if(boosting||jet.shield)
				{
					if(map[jet.y+ii][jet.x+i]>=MINE_1&&map[jet.y+ii][jet.x+i]<=MINE_18)
						{pushMine(jet.x+i,jet.xo,jet.y+ii,jet.yo);map[jet.y+ii][jet.x+i]=0;jet.score+=(BLOCK_VALUE*ptr.lvl);}
					else
						{map[jet.y+ii][jet.x+i]=makePickup(map[jet.y+ii][jet.x+i]);m_Sprite16((jet.x+i-ptr.x)*8-ptr.o-4,(jet.y+ii)*8-4,16,mine_ex_4,light_vscreen,dark_vscreen,SPRT_XOR);jet.score+=(BLOCK_VALUE*ptr.lvl);}
					if(!boosting)shield_down=1;
				} else if(!_keytest(RR_F4)&&!keys.f4)death_marker=1;
			} else if(map[jet.y+ii][jet.x+i]<0){
				if(map[jet.y+ii][jet.x+i]==CASH_100)jet.cash+=100;
				else if(map[jet.y+ii][jet.x+i]==CASH_250)jet.cash+=250;
				else if(map[jet.y+ii][jet.x+i]==CASH_500)jet.cash+=500;
				else if(map[jet.y+ii][jet.x+i]==CASH_1000)jet.cash+=1000;
				else if(map[jet.y+ii][jet.x+i]==SHIELD&&jet.shield<jet.shield_max)jet.shield++;
				else if(map[jet.y+ii][jet.x+i]==BOOST){jet.boost+=10;if(jet.boost>100)jet.boost=100;}
				map[jet.y+ii][jet.x+i]=0;
				if(jet.cash>999999)jet.cash=999999;
			}
		}
	}
	if(shield_down&&!death_marker)jet.shield--;
}
void doShots(void){
	int i,ii,iii;
	if((_keytest(RR_2ND)||keys.second)&&!jet.jc){
		for(i=0;i<=24;i++){
			if(!shot_stack[i].type){
				shot_stack[i].type=CANNON;
				shot_stack[i].x=jet.x+2;
				shot_stack[i].xo=jet.xo-(CANNON_SPEED+speed);
				shot_stack[i].y=jet.y;
				shot_stack[i].yo=jet.yo;
				shot_stack[i]=(SHOT)testOffsetShot(shot_stack[i]);
				jet.jc=jet.repeat_rate_c;
				break;
			}
		}
	} else if(jet.jc)jet.jc--;
	if((_keytest(RR_SHIFT)||keys.shift)&&!jet.jm&&jet.missile){
		for(i=0;i<=24;i++){
			if(!shot_stack[i].type){
				shot_stack[i].type=UP_MISSILE;
				shot_stack[i].x=jet.x;
				shot_stack[i].xo=jet.xo-(MISSILE_SPEED+speed)+4;
				shot_stack[i].y=jet.y;
				shot_stack[i].yo=jet.yo+MISSILE_SPEED;
				shot_stack[i]=(SHOT)testOffsetShot(shot_stack[i]);
				jet.jm=jet.repeat_rate_m;
				break;
			}
		}
	}
	else if((_keytest(RR_DIAMOND)||keys.diamond)&&!jet.jm&&jet.missile){
		for(i=0;i<=24;i++){
			if(!shot_stack[i].type){
				shot_stack[i].type=DOWN_MISSILE;
				shot_stack[i].x=jet.x;
				shot_stack[i].xo=jet.xo-(MISSILE_SPEED+speed)+4;
				shot_stack[i].y=jet.y+1;
				shot_stack[i].yo=jet.yo-MISSILE_SPEED;
				shot_stack[i]=(SHOT)testOffsetShot(shot_stack[i]);
				jet.jm=jet.repeat_rate_m;
				break;
			}
		}
	} else if(jet.jm)jet.jm--;
	for(i=0;i<=24;i++){
		if(shot_stack[i].type==UP_MISSILE||shot_stack[i].type==DOWN_MISSILE){
			if(shot_stack[i].type==UP_MISSILE)shot_stack[i].yo-=MISSILE_SPEED;
			else if(shot_stack[i].type==DOWN_MISSILE)shot_stack[i].yo+=MISSILE_SPEED;
			shot_stack[i].xo+=(MISSILE_SPEED+speed);
			shot_stack[i]=(SHOT)testOffsetShot(shot_stack[i]);
			if(shot_stack[i].x*8+shot_stack[i].xo>(ptr.x+20)*8+ptr.o||shot_stack[i].y*8+shot_stack[i].yo<0){shot_stack[i].type=0;continue;}
			for(ii=0;ii<=1;ii++){
				if(ii==1&&shot_stack[i].xo==0)break;
				for(iii=0;iii<=1;iii++){
					if(iii==1&&shot_stack[i].yo==0)break;
					if(map[shot_stack[i].y+iii][shot_stack[i].x+ii]>0){
						if(map[shot_stack[i].y+iii][shot_stack[i].x+ii]<4)
							{map[shot_stack[i].y+iii][shot_stack[i].x+ii]=makePickup(map[shot_stack[i].y+iii][shot_stack[i].x+ii]);m_Sprite16((shot_stack[i].x+ii-ptr.x)*8-ptr.o-4,(shot_stack[i].y+iii)*8-4,16,mine_ex_4,light_vscreen,dark_vscreen,SPRT_XOR);jet.score+=(BLOCK_VALUE*ptr.lvl);}
						if(map[shot_stack[i].y+iii][shot_stack[i].x+ii]>=MINE_1&&map[shot_stack[i].y+iii][shot_stack[i].x+ii]<=MINE_18){
							map[shot_stack[i].y+iii][shot_stack[i].x+ii]=0;
							jet.score+=(BLOCK_VALUE*ptr.lvl);
							pushMine(shot_stack[i].x+ii,shot_stack[i].xo,shot_stack[i].y+iii,shot_stack[i].yo);
						}
						if(map[shot_stack[i].y+iii][shot_stack[i].x+ii]==INDESTRUCTIBLE&&jet.missile==2){
							map[shot_stack[i].y+iii][shot_stack[i].x+ii]=makePickup(map[shot_stack[i].y+iii][shot_stack[i].x+ii]);m_Sprite16((shot_stack[i].x+ii-ptr.x)*8-ptr.o-4,(shot_stack[i].y+iii)*8-4,16,mine_ex_4,light_vscreen,dark_vscreen,SPRT_XOR);jet.score+=(BLOCK_VALUE*ptr.lvl);
						}
						shot_stack[i].type=EX_1;
					}
				}
			}
			if(shot_stack[i].type==UP_MISSILE&&jet.missile==1)m_Sprite8((shot_stack[i].x-ptr.x)*8+(shot_stack[i].xo-ptr.o),shot_stack[i].y*8+shot_stack[i].yo,8,missile_up,light_vscreen,dark_vscreen,SPRT_XOR);
			else if(shot_stack[i].type==UP_MISSILE&&jet.missile==2)m_Sprite8((shot_stack[i].x-ptr.x)*8+(shot_stack[i].xo-ptr.o),shot_stack[i].y*8+shot_stack[i].yo,8,super_missile_up,light_vscreen,dark_vscreen,SPRT_XOR);
			else if(shot_stack[i].type==DOWN_MISSILE&&jet.missile==1)m_Sprite8((shot_stack[i].x-ptr.x)*8+(shot_stack[i].xo-ptr.o),shot_stack[i].y*8+shot_stack[i].yo,8,missile_down,light_vscreen,dark_vscreen,SPRT_XOR);
			else if(shot_stack[i].type==DOWN_MISSILE&&jet.missile==2)m_Sprite8((shot_stack[i].x-ptr.x)*8+(shot_stack[i].xo-ptr.o),shot_stack[i].y*8+shot_stack[i].yo,8,super_missile_down,light_vscreen,dark_vscreen,SPRT_XOR);
		} if(shot_stack[i].type==CANNON){
			shot_stack[i].xo+=(CANNON_SPEED+speed);
			shot_stack[i]=(SHOT)testOffsetShot(shot_stack[i]);
			if(shot_stack[i].x*8+shot_stack[i].xo>(ptr.x+20)*8+ptr.o){shot_stack[i].type=0;continue;}
			for(ii=0;ii<=1;ii++){
				if(ii==1&&shot_stack[i].xo==0)break;
				for(iii=0;iii<=1;iii++){
					if(iii==1&&shot_stack[i].yo==0)break;
					if(map[shot_stack[i].y+iii][shot_stack[i].x+ii]>0){
						if(map[shot_stack[i].y+iii][shot_stack[i].x+ii]<4&&jet.cannon<4){
							map[shot_stack[i].y+iii][shot_stack[i].x+ii]-=jet.cannon;
							if(map[shot_stack[i].y+iii][shot_stack[i].x+ii]<=0){
								map[shot_stack[i].y+iii][shot_stack[i].x+ii]=makePickup(map[shot_stack[i].y+iii][shot_stack[i].x+ii]);
								m_Sprite16((shot_stack[i].x+ii-ptr.x)*8-ptr.o-4,(shot_stack[i].y+iii)*8-4,16,mine_ex_4,light_vscreen,dark_vscreen,SPRT_XOR);
								jet.score+=(BLOCK_VALUE*ptr.lvl);
							}
						}else if(jet.cannon==4&&map[shot_stack[i].y+iii][shot_stack[i].x+ii]<5){
							map[shot_stack[i].y+iii][shot_stack[i].x+ii]=makePickup(map[shot_stack[i].y+iii][shot_stack[i].x+ii]);
							m_Sprite16((shot_stack[i].x+ii-ptr.x)*8-ptr.o-4,(shot_stack[i].y+iii)*8-4,16,mine_ex_4,light_vscreen,dark_vscreen,SPRT_XOR);
							jet.score+=(BLOCK_VALUE*ptr.lvl);
						}
						else if(map[shot_stack[i].y+iii][shot_stack[i].x+ii]>=MINE_1&&map[shot_stack[i].y+iii][shot_stack[i].x+ii]<=MINE_18){
								map[shot_stack[i].y+iii][shot_stack[i].x+ii]=0;
								jet.score+=(BLOCK_VALUE*ptr.lvl);
								pushMine(shot_stack[i].x+ii,shot_stack[i].xo,shot_stack[i].y+iii,shot_stack[i].yo);
						}
						shot_stack[i].type=0;
					}
				}
			}
			if(shot_stack[i].type==CANNON)m_Sprite8((shot_stack[i].x-ptr.x)*8+(shot_stack[i].xo-ptr.o),shot_stack[i].y*8+shot_stack[i].yo,8,cannon,light_vscreen,dark_vscreen,SPRT_XOR);
		}
		if(shot_stack[i].type==EX_1){
			shot_stack[i].type=EX_2;
			if(shot_stack[i].x+shot_stack[i].xo<ptr.x-ptr.o){shot_stack[i].type=0;continue;}
			else m_Sprite8((shot_stack[i].x-ptr.x)*8+(shot_stack[i].xo-ptr.o),shot_stack[i].y*8+shot_stack[i].yo,8,ex_1,light_vscreen,dark_vscreen,SPRT_XOR);
			continue;
		}
		if(shot_stack[i].type==EX_2){
			shot_stack[i].type=0;
			if(shot_stack[i].x+shot_stack[i].xo<ptr.x-ptr.o){shot_stack[i].type=0;continue;}
			else m_Sprite16((shot_stack[i].x-ptr.x)*8+(shot_stack[i].xo-ptr.o)-4,shot_stack[i].y*8+shot_stack[i].yo-4,16,ex_2,light_vscreen,dark_vscreen,SPRT_XOR);
			continue;
		}
	}
}
void doMines(void){
	int i,ii,iii;
	for(i=0;i<=24;i++){
		if(ex_stack[i].type==EX_1){
			ex_stack[i].type=EX_2;
			m_Sprite8((ex_stack[i].x-ptr.x)*8+(ex_stack[i].xo-ptr.o)-4,ex_stack[i].y*8+ex_stack[i].yo-4,8,mine_ex_1,light_vscreen,dark_vscreen,SPRT_XOR);
			continue;
		}
		if(ex_stack[i].type==EX_2){
			ex_stack[i].type=EX_3;
			m_Sprite16((ex_stack[i].x-ptr.x)*8+(ex_stack[i].xo-ptr.o)-8,ex_stack[i].y*8+ex_stack[i].yo-8,16,mine_ex_2,light_vscreen,dark_vscreen,SPRT_XOR);
			continue;
		}
		if(ex_stack[i].type==EX_3){
			ex_stack[i].type=EX_4;
			m_Sprite16((ex_stack[i].x-ptr.x)*8+(ex_stack[i].xo-ptr.o)-8,ex_stack[i].y*8+ex_stack[i].yo-8,16,mine_ex_3,light_vscreen,dark_vscreen,SPRT_XOR);
			for(ii=-1;ii<=1;ii++){
				if(ex_stack[i].x+ii>LEVEL_LENGTH-1)continue;
				for(iii=-1;iii<=1;iii++){
					if(ex_stack[i].y+iii>10||ex_stack[i].y+iii<0)continue;
					if(map[ex_stack[i].y+iii][ex_stack[i].x+ii]>=MINE_1&&map[ex_stack[i].y+iii][ex_stack[i].x+ii]<=MINE_18)
						{pushMine(ex_stack[i].x+ii,ex_stack[i].xo,ex_stack[i].y+iii,ex_stack[i].yo);map[ex_stack[i].y+iii][ex_stack[i].x+ii]=0;jet.score+=(BLOCK_VALUE*ptr.lvl*2);}
					else if(map[ex_stack[i].y+iii][ex_stack[i].x+ii]>0){m_Sprite16((ex_stack[i].x+ii-ptr.x)*8-ptr.o-4,(ex_stack[i].y+iii)*8-4,16,mine_ex_4,light_vscreen,dark_vscreen,SPRT_XOR);map[ex_stack[i].y+iii][ex_stack[i].x+ii]=makePickup(map[ex_stack[i].y+iii][ex_stack[i].x+ii]);jet.score+=(BLOCK_VALUE*ptr.lvl*2);}
				}
			}
			continue;
		}
		if(ex_stack[i].type==EX_4){
			ex_stack[i].type=0;
			m_Sprite16((ex_stack[i].x-ptr.x)*8+(ex_stack[i].xo-ptr.o)-8,ex_stack[i].y*8+ex_stack[i].yo-8,16,mine_ex_4,light_vscreen,dark_vscreen,SPRT_XOR);
			continue;
		}
	}
}
void drawStats(void){
	unsigned int i;
	char str[10];
	GrayDrawStrExt2B(0,88,(char *)" SHIELD ",A_REVERSE,F_4x6,light_vscreen,dark_vscreen);
	for(i=0;i<jet.shield;i++)m_Sprite8(0+i*6,95,4,shield_icon,light_vscreen,dark_vscreen,SPRT_XOR);
	GrayDrawStrExt2B(35,88,(char *)" BOOST ",A_REVERSE,F_4x6,light_vscreen,dark_vscreen);
	for(i=0;i<25;i++){if(jet.boost>=(i+1)*4)FastDrawLine(dark_vscreen,35+i,95,35+i,98,A_NORMAL);else break;}
	GrayDrawStrExt2B(65,88,(char *)" CASH ",A_REVERSE,F_4x6,light_vscreen,dark_vscreen);
	sprintf(str,"$%lu",jet.cash);
	GrayDrawStrExt2B(65,94,(char *)str,A_NORMAL,F_4x6,light_vscreen,dark_vscreen);
	GrayDrawStrExt2B(98,88,(char *)" SCORE ",A_REVERSE,F_4x6,light_vscreen,dark_vscreen);
	sprintf(str,"%lu",jet.score);
	GrayDrawStrExt2B(98,94,(char *)str,A_NORMAL,F_4x6,light_vscreen,dark_vscreen);
	sprintf(str,"LV %d",ptr.lvl);
	GrayDrawStrExt2B(135,90,(char *)str,A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
	FastDrawLine(light_vscreen,0,99,159-ptr.x,99,A_NORMAL);
	FastDrawLine(dark_vscreen,0,99,159-ptr.x,99,A_NORMAL);
}
/*############### Miscellaneous Rotutines ##############*/
atexit_t exitStuff(void){
	delay(200);
	GraySpriteDeInit();
	SetIntVec(AUTO_INT_1,interrupt1);
	SetIntVec(AUTO_INT_5,interrupt5);
	FILE *f=NULL;
	if((f=fopen("b2save","rb"))==NULL){
		printf("file io error 3\npress enter to exit\n");
		while(!_keytest(RR_ENTER));
		return 0;
	}
	if(fread(&save,sizeof(SAVE),1,f)!=1){
		printf("file io error 4\npress enter to exit\n");
		fclose(f);
		while(!_keytest(RR_ENTER));
		return 0;
	}
	fclose(f);
	f=NULL;
	save.jet=(JET)jet;
	save.ptr=(PTR)ptr;
	save.speed=speed;
	if((f=fopen("b2save","wb"))==NULL){
		printf("file io error 8\npress enter to exit\n");
		while(!_keytest(RR_ENTER));
		return 0;
	}
	if(fwrite(&save,sizeof(SAVE),1,f)!=1){
		printf("file io error 5\npress enter to exit\n");
		fclose(f);
		while(!_keytest(RR_ENTER));
		return 0;
	}
	// append the file ID tag
	fputc(0,f);
	fputs("SAV",f);
	fputc(0,f);
	fputc(OTH_TAG,f);
	fclose(f);
	return 0;
}
void initialize(void){
	clrscr();
	FILE *f=NULL;
	atexit((atexit_t)exitStuff);
	randomize();
	interrupt1=GetIntVec(AUTO_INT_1);
	interrupt5=GetIntVec(AUTO_INT_5);
	SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
	SetIntVec(AUTO_INT_5,DUMMY_HANDLER);
	speed=NORMAL;
	if(!GraySpriteInit(3840)){
		printf("Not enough free memory\npress enter to exit\n");
		while(!_keytest(RR_ENTER));
		exit(0);
	}
	clearScreens();
	if((f=fopen("b2save","rb"))==NULL){
		if((f=fopen("b2save","wb"))==NULL){
			printf("file io error 1\npress enter to exit\n");
			while(!_keytest(RR_ENTER));
			exit(0);
		}
		save=(SAVE){(JET){JET_START_X,JET_START_Y,0,0,1,0,0,0,0,0,EX_1,0,0,2,2,4,6,0,0},(PTR){0,0,1},
							 {(HI_SCORE){(unsigned long)0, "          "},(HI_SCORE){(unsigned long)/*500000*/0, "Big Ben   "},(HI_SCORE){(unsigned long)250000, "Orage     "},
								(HI_SCORE){(unsigned long)200000, "JudeCN    "},(HI_SCORE){(unsigned long)100000, "Magus     "},(HI_SCORE){(unsigned long)50000, "Thunder   "}},NORMAL};
		if(fwrite(&save,sizeof(SAVE),1,f)!=1){
			printf("file io error 2\npress enter to exit\n");
			while(!_keytest(RR_ENTER));
			exit(0);
		}
		// append the file ID tag
		fputc(0,f);
		fputs("SAV",f);
		fputc(0,f);
		fputc(OTH_TAG,f);
		// close the file
		fclose(f);
	}
	else {
		if(fread(&save,sizeof(SAVE),1,f)!=1){
			printf("file io error 9\npress enter to exit\n");
			while(!_keytest(RR_ENTER));
			exit(0);
		}
		speed=save.speed;
		fclose(f);
	}

}
void saveGame(void){
	printf("saving...\n");
	FILE *f=NULL;
	save.jet=(JET)jet;
	save.ptr=(PTR)ptr;
	save.speed=speed;
	if((f=fopen("b2save","wb"))==NULL){
		printf("file io error 10\npress enter to exit\n");
		while(!_keytest(RR_ENTER));
		exit(0);
	}
	if(fwrite(&save,sizeof(SAVE),1,f)!=1){
		printf("file io error 11\npress enter to exit\n");
		while(!_keytest(RR_ENTER));
		exit(0);
	}
	// append the file ID tag
	fputc(0,f);
	fputs("SAV",f);
	fputc(0,f);
	fputc(OTH_TAG,f);
	fclose(f);
}
void loadGame(void){
	printf("loading...\n");
	FILE *f=NULL;
	if((f=fopen("b2save","rb"))==NULL){
		printf("file io error 12\npress enter to exit\n");
		while(!_keytest(RR_ENTER));
		exit(0);
	}
	if(fread(&save,sizeof(SAVE),1,f)!=1){
		printf("file io error 13\npress enter to exit");
		while(!_keytest(RR_ENTER));
		exit(0);
	}
	fclose(f);
	jet=(JET)save.jet;
	ptr=(PTR)save.ptr;
}
void resetGame(void){
	flushStacks();
	clearScreens();
	jet=(JET){JET_START_X,JET_START_Y,0,0,1,0,0,0,0,0,EX_1,0,0,2,2,4,6,0,0};
	ptr=(PTR){0,0,1};
	keys=(KEYS){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	randomizeMap();
}
void prepareGame(void){
	clearScreens();
	delay(200);
	short x=0,y=0,pos=CON,ex=EX_1;
	SHOT mine={random(LCD_WIDTH),random(LCD_HEIGHT),0,0,1};
	while(1){
		GrayDrawStrExt2B(32,0,(char *)"Play a Game?",A_REVERSE,F_8x10,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(26,25,(char *)"Continue",A_NORMAL,F_8x10,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(26,45,(char *)"New Game",A_NORMAL,F_8x10,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(26,65,(char *)"Back",A_NORMAL,F_8x10,light_vscreen,dark_vscreen);
		if(pos==CON)x=1,y=26;
		else if(pos==NEW)x=1,y=46;
		else if(pos==BACK)x=1,y=66;
		if(ex<=EX_2)m_Sprite8(x,y,8,reg_exhaust_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(ex<=EX_4)m_Sprite8(x,y,8,reg_exhaust_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(ex<=EX_6)m_Sprite8(x,y,8,reg_exhaust_3,light_vscreen,dark_vscreen,SPRT_XOR);
		if(++ex>EX_6)ex=EX_1;
		m_Sprite16(x+8,y,8,jet_reg,light_vscreen,dark_vscreen,SPRT_XOR);
		if(mine.type<=3)m_Sprite8(mine.x,mine.y,8,mine_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=6)m_Sprite8(mine.x,mine.y,8,mine_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=9)m_Sprite8(mine.x,mine.y,8,mine_3,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=12)m_Sprite8(mine.x,mine.y,8,mine_ex_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=15)m_Sprite16(mine.x-4,mine.y-4,16,mine_ex_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=18)m_Sprite16(mine.x-4,mine.y-4,16,mine_ex_3,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=21)m_Sprite16(mine.x-4,mine.y-4,16,mine_ex_4,light_vscreen,dark_vscreen,SPRT_XOR);
		if(++mine.type>21)mine=(SHOT){random(LCD_WIDTH),random(LCD_HEIGHT),0,0,1};
		FastCopyScreen(light_vscreen,GetPlane(0));
		FastCopyScreen(dark_vscreen,GetPlane(1));
		clearScreens();
		if(_keytest(RR_DOWN)){if(pos==CON)pos=NEW;else if(pos==NEW)pos=BACK;}
		if(_keytest(RR_UP)){if(pos==NEW)pos=CON;else if(pos==BACK)pos=NEW;}
		if(_keytest(RR_2ND)||_keytest(RR_ENTER)){
			if(pos==NEW)resetGame();
			else if(pos==CON){
				FILE *f=NULL;
				if((f=fopen("b2save","r+b"))==NULL){
					printf("file io error 6\npress enter to exit\n");
					while(!_keytest(RR_ENTER));
					exit(0);
				}
				if(fread(&save,sizeof(SAVE),1,f)!=1){
					printf("file io error 7\npress enter to exit\n");
					while(!_keytest(RR_ENTER));
					exit(0);
				}
				fclose(f);
				jet=(JET)save.jet;
				ptr=(PTR)save.ptr;
			}
			else if(pos==BACK)doMenu();
			break;
		}
		if(_keytest(RR_PLUS))OSContrastUp();
		if(_keytest(RR_MINUS))OSContrastDn();
		if(_keytest(RR_F5))exit(0);
		delay(40);
	}
	delay(200);
}
void flushStacks(void){
	//short i;
	//for(i=0;i<=24;i++){shot_stack[i]=(SHOT){0,0,0,0,0};ex_stack[i]=(SHOT){0,0,0,0,0};}
	memset(shot_stack,0x00,sizeof(SHOT)*25);
	memset(ex_stack,0x00,sizeof(SHOT)*25);
}
short makePickup(short type){
	int cash_100=0,cash_250=0,cash_500=0,cash_1000=0,shield=0,boost=0,r=0;
	if(random(3)==3)return 0;
	if(type<=3)shield=2,boost=5,cash_100=55,cash_250=80,cash_500=95,cash_1000=100;
	if(type==INDESTRUCTIBLE)shield=15,boost=30,cash_100=30,cash_250=40,cash_500=60,cash_1000=100;
	r=random(100);
	if(r<=shield&&jet.shield<jet.shield_max)return SHIELD;
	else if(r<=boost&&jet.boost<jet.boost_max&&!_keytest(RR_ALPHA))return BOOST;
	else if(r<=cash_100)return CASH_100;
	else if(r<=cash_250)return CASH_250;
	else if(r<=cash_500)return CASH_500;
	else if(r<=cash_1000)return CASH_1000;
	else return 0;
}
void doPause(void){
	SetPlane(1);
	DrawStr(0,0,"PAUSED, press F1",A_REVERSE);
	delay(100);
	while(!_keytest(RR_F1)&&!keys.f1){
		if(_keytest(RR_F5)||keys.f5)exit(0);
		if(_keytest(RR_PLUS)||keys.plus)OSContrastUp();
		if(_keytest(RR_MINUS)||keys.minus)OSContrastDn();
		if(_keytest(RR_CLEAR)||keys.clear){ptr.lvl++;gotoShop();}// this is a cheat
		delay(100);
	}
	delay(100);
}
void pushMine(short x,short xo,short y,short yo){
	int i;
	for(i=0;i<=24;i++){
		if(!ex_stack[i].type){
			ex_stack[i].type=EX_1;
			ex_stack[i].x=x;
			ex_stack[i].xo=xo;
			ex_stack[i].y=y;
			ex_stack[i].yo=yo;
			break;
		}
	}
}
void testOffsetJet(void){
	while(jet.xo>=8){jet.xo-=8;jet.x++;}
	while(jet.xo<0){jet.xo+=8;jet.x--;}
	while(jet.yo>=8){jet.yo-=8;jet.y++;}
	while(jet.yo<0){jet.yo+=8;jet.y--;}
}
SHOT testOffsetShot(SHOT shot){
	while(shot.xo>=8){shot.xo-=8;shot.x++;}
	while(shot.xo<0){shot.xo+=8;shot.x--;}
	while(shot.yo>=8){shot.yo-=8;shot.y++;}
	while(shot.yo<0){shot.yo+=8;shot.y--;}
	return shot;
}
void clearScreens(void){
	memset(light_vscreen,0x00,3840);
	memset(dark_vscreen,0x00,3840);
}
void doDeath(void){
	short i;
	death_marker=0;
	jet.score=(unsigned long)(jet.score-(jet.score/10));
	m_Sprite8((jet.x-ptr.x)*8+(jet.xo-ptr.o)+4,jet.y*8+jet.yo,8,mine_ex_1,GetPlane(0),GetPlane(1),SPRT_XOR);
	delay(500);
	m_Sprite16((jet.x-ptr.x)*8+(jet.xo-ptr.o),jet.y*8+jet.yo-4,16,mine_ex_2,GetPlane(0),GetPlane(1),SPRT_XOR);
	delay(500);
	m_Sprite16((jet.x-ptr.x)*8+(jet.xo-ptr.o),jet.y*8+jet.yo-4,16,mine_ex_3,GetPlane(0),GetPlane(1),SPRT_XOR);
	for(i=0;i<=5;i++){OSContrastDn();delay(100);}
	memset(GetPlane(0),0x00,3840);
	memset(GetPlane(1),0x00,3840);
	for(i=0;i<=5;i++){OSContrastUp();delay(100);}
	gotoShop();
	GrayDrawStrExt2B(0,50,(char *)"Back",A_NORMAL,F_8x10,GetPlane(0),GetPlane(1));
	delay(100);
}
void randomizeMap(void){
	short i,ii,iii;
	short b[8];
	short r=0,m=0;
	if(ptr.lvl==1||ptr.lvl==2)r=50,m=70;
	else if(ptr.lvl==3||ptr.lvl==4)r=55,m=75;
	else if(ptr.lvl==5)r=50,m=75;
	else if(ptr.lvl==6)r=50,m=80;
	else if(ptr.lvl==7)r=50,m=85;
	else if(ptr.lvl==8)r=40,m=85;
	else if(ptr.lvl==9)r=40,m=90;
	for(i=9;i<=LEVEL_LENGTH-1;i++){
		for(ii=0;ii<=7;ii++)b[ii]=random(11);
		if(ptr.lvl<=9)b[1]=b[0];
		if(ptr.lvl<=8)b[2]=b[0];
		if(ptr.lvl<=6)b[3]=b[0];
		if(ptr.lvl<=4)b[4]=b[0];
		if(ptr.lvl<=2)b[5]=b[0];
		if(ptr.lvl<=1)b[6]=b[0];
		for(ii=0;ii<=10;ii++){
			for(iii=0;iii<=7;iii++){
				if(ii!=b[iii])map[ii][i]=0;
				else {
					short bt=random(100);
					if(bt<=r)map[ii][i]=REGULAR;
					else if(bt<=m)map[ii][i]=INDESTRUCTIBLE;
					else {short mine=random(3);if(mine==1)map[ii][i]=MINE_1;else if(mine==2)map[ii][i]=MINE_4;else if(mine==3)map[ii][i]=MINE_7;}
					break;
				}
			}
		}
	}
}
void delay(short number) {
	short loop, loop2;
	for (loop=0;loop<number;loop++) {
		for (loop2=0;loop2<1000;loop2++);
	}
}
/*################ The Main Menu ##################*/
void doMenu(void){
	short x,y,pos=PLAY,ex=EX_1;
	SHOT mine={random(LCD_WIDTH),random(LCD_HEIGHT),0,0,1};
	delay(200);
	while(1){
		clearScreens();
		for(y=0;y<1;y++){for(x=0;x<5;x++){GraySprite32_XOR(x*32,y*58,58,&title[0][(y*5+x)*58],&title[1][(y*5+x)*58],light_vscreen,dark_vscreen);}}
		GrayDrawStrExt2B(26,68,(char *)"Play",A_NORMAL,F_8x10,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(26,88,(char *)"Options",A_NORMAL,F_8x10,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(110,68,(char *)"Help",A_NORMAL,F_8x10,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(110,88,(char *)"Exit",A_NORMAL,F_8x10,light_vscreen,dark_vscreen);
		if(_keytest(RR_DOWN)){if(pos==PLAY)pos=OPTIONS;else if(pos==HELP)pos=EXIT;}
		if(_keytest(RR_UP)){if(pos==OPTIONS)pos=PLAY;else if(pos==EXIT)pos=HELP;}
		if(_keytest(RR_RIGHT)){if(pos==PLAY)pos=HELP;else if(pos==OPTIONS)pos=EXIT;}
		if(_keytest(RR_LEFT)){if(pos==HELP)pos=PLAY;else if(pos==EXIT)pos=OPTIONS;}
		if(_keytest(RR_ESC))doQuit();
		if(_keytest(RR_PLUS))OSContrastUp();
		if(_keytest(RR_MINUS))OSContrastDn();
		if(_keytest(RR_2ND)||_keytest(RR_ENTER)){if(pos==PLAY){prepareGame();doGame();}else if(pos==OPTIONS)doOptions();else if(pos==HELP)doHelp();else if(pos==EXIT)doQuit();}
		if(_keytest(RR_F5))exit(0);
		if(pos==PLAY)x=1,y=69;
		else if(pos==OPTIONS)x=1,y=89;
		else if(pos==HELP)x=85,y=69;
		else if(pos==EXIT)x=85,y=89;
		if(ex<=EX_2)m_Sprite8(x,y,8,reg_exhaust_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(ex<=EX_4)m_Sprite8(x,y,8,reg_exhaust_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(ex<=EX_6)m_Sprite8(x,y,8,reg_exhaust_3,light_vscreen,dark_vscreen,SPRT_XOR);
		if(++ex>EX_6)ex=EX_1;
		m_Sprite16(x+8,y,8,jet_reg,light_vscreen,dark_vscreen,SPRT_XOR);
		if(mine.type<=3)m_Sprite8(mine.x,mine.y,8,mine_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=6)m_Sprite8(mine.x,mine.y,8,mine_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=9)m_Sprite8(mine.x,mine.y,8,mine_3,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=12)m_Sprite8(mine.x,mine.y,8,mine_ex_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=15)m_Sprite16(mine.x-4,mine.y-4,16,mine_ex_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=18)m_Sprite16(mine.x-4,mine.y-4,16,mine_ex_3,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=21)m_Sprite16(mine.x-4,mine.y-4,16,mine_ex_4,light_vscreen,dark_vscreen,SPRT_XOR);
		if(++mine.type>21)mine=(SHOT){random(LCD_WIDTH),random(LCD_HEIGHT),0,0,1};
		FastCopyScreen(light_vscreen,GetPlane(0));
		FastCopyScreen(dark_vscreen,GetPlane(1));
		delay(60);
	}
	delay(200);
}
void doOptions(void){
	short x=0,y=0,pos=SPEED,ex=EX_1;
	SHOT mine={random(LCD_WIDTH),random(LCD_HEIGHT),0,0,1};
	char spd_str[15];
	clearScreens();
	delay(100);
	while(1){
		if(speed==SLOW)sprintf(spd_str,"Speed - Slow");
		else if(speed==NORMAL)sprintf(spd_str,"Speed - Normal");
		else if(speed==FAST)sprintf(spd_str,"Speed - Fast");
		GrayDrawStrExt2B(52,0,(char *)"OPTIONS",A_REVERSE,F_8x10,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(26,20,(char *)spd_str,A_NORMAL,F_8x10,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(26,40,(char *)"Back",A_NORMAL,F_8x10,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(26,60,(char *)"+/- to adjust contrast",A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		if(_keytest(RR_2ND)||_keytest(RR_ENTER)){if(pos==SPEED){if(++speed>FAST)speed=SLOW;}else if(pos==BACK)break;}
		if(_keytest(RR_PLUS))OSContrastUp();
		if(_keytest(RR_MINUS))OSContrastDn();
		if(_keytest(RR_ESC))break;
		if(_keytest(RR_UP))if(pos==BACK)pos=SPEED;
		if(_keytest(RR_DOWN))if(pos==SPEED)pos=BACK;
		if(_keytest(RR_F5))exit(0);
		if(pos==SPEED)x=1,y=21;
		else if(pos==BACK)x=1,y=41;
		if(ex<=EX_2)m_Sprite8(x,y,8,reg_exhaust_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(ex<=EX_4)m_Sprite8(x,y,8,reg_exhaust_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(ex<=EX_6)m_Sprite8(x,y,8,reg_exhaust_3,light_vscreen,dark_vscreen,SPRT_XOR);
		if(++ex>EX_6)ex=EX_1;
		m_Sprite16(x+8,y,8,jet_reg,light_vscreen,dark_vscreen,SPRT_XOR);
		if(mine.type<=3)m_Sprite8(mine.x,mine.y,8,mine_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=6)m_Sprite8(mine.x,mine.y,8,mine_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=9)m_Sprite8(mine.x,mine.y,8,mine_3,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=12)m_Sprite8(mine.x,mine.y,8,mine_ex_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=15)m_Sprite16(mine.x-4,mine.y-4,16,mine_ex_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=18)m_Sprite16(mine.x-4,mine.y-4,16,mine_ex_3,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=21)m_Sprite16(mine.x-4,mine.y-4,16,mine_ex_4,light_vscreen,dark_vscreen,SPRT_XOR);
		if(++mine.type>21)mine=(SHOT){random(LCD_WIDTH),random(LCD_HEIGHT),0,0,1};
		FastCopyScreen(light_vscreen,GetPlane(0));
		FastCopyScreen(dark_vscreen,GetPlane(1));
		clearScreens();
		delay(60);
	}
	delay(200);
}
void doHelp(void){
	unsigned short help_buf_size=20000;
	free(light_vscreen);
	free(dark_vscreen);
	if((light_vscreen=malloc(help_buf_size))==NULL){
		printf("Not enough free memory\nto view help\npress enter to exit\n");
		exit(0);
	}
	if((dark_vscreen=malloc(help_buf_size))==NULL){
		printf("Not enough free memory\nto view help\npress enter to exit\n");
		exit(0);
	}
	char *help_text[] = {"              Jet Blaster Mach II Help"," ","2ND - fires the cannon",
												"Shift - fires missile up","Diamond - fires missile down","Alpha - hold to boost",
												"Arrows - move the jet","ESC - Quit to Shop","F1 - pause or save (shop)",
												"F4- Quick-Load","F5 - Save and Quit","+/- Adjusts contrast"," ","The Debris:",
												"You will navigate your way through 9","debris fields of increasing density.",
												" ","Regular blocks look like this:","They have a health of 3. If hit",
												"by cannons, they will lose","this health, and will change to     and","Missiles will always destroy these",
												"in one hit."," ","Sometimes you encounter these:","which are metal blocks and are",
												"hard to destroy.  They can only be","destroyed by collision, mines, super",
												"missiles, and level 4 cannons.  All","of these destroy them in one hit."," ",
												"Mines also float in the debris fields"," "," ",
												"They spin around.  You can use these","to your advantage!  Destroy them and",
												"they will destroy everything adjacent!"," ","The Pickups:","When blocks, not mines, are destroyed,",
												"they leave behind pickups."," ","    This is money"," ","    This restores boost"," ","    This restores shield",
												"Metal blocks generally drop better","pickups than regular ones."," ","Modifications:",
												"At the Jet Shop you can upgrade your","ship for money. There are three","categories:"," ","Weapons:",
												"You can upgrade your cannon's strength.","You can purchase unlimited supplies of",
												"missiles and super missiles.  You can","also increase the speed at which your","cannon repeats."," ",
												"Jet Mods:","The wing modification makes your jet","move up and down faster.  The Thruster",
												"makes you move left and right faster."," ","Special:","Each shield upgrade increases your",
												"shield capacity by one.  when the","shield is on, your jet looks like this:"," "," ",
												"Your current shield level is","located in the lower right.  Shields","absorb collisions and destroy the",
												"blocks.  When you have no shields, a","collision will result in death, and you",
												"will return to the shop.  The first","boost upgrade gives you a half tank of",
												"boost, and the second gives another","half.  When you have boost, hold down",
												"alpha to speed up, and you will destroy","blocks in your path without damaging","your shields."," ",
												"Jet Blaster Mach 2 also keeps track of","your progress through the file \"b2save\".",
												"Whenever you quit, your progress is","automatically stored here and will be",
												"loaded again if you choose \"Continue\"","on the Play menu.  Additionally, you can",
												"save while in the game.  F1 will save","your game in the shop, and F4 will load",
												"it.  This way you can save and buy","something, try it out, then load and buy",
												"something else.  Your games saved in","this manner will not be overwritten","until you save again or quit.",
												" ","That's it! Have Fun!","http://oragestudio.free.fr"};
	memset(light_vscreen,0x00,help_buf_size);
	memset(dark_vscreen,0x00,help_buf_size);
	ST_helpMsg("Generating...");
	FontSetSys(F_4x6);
	int y=0;
	sprt_bound.downright_y=102*6;
	PortSet(dark_vscreen,239,102*6-1);
	int i;
	for(i=0;i<=101;i++)DrawStr(8,i*6,help_text[i],A_NORMAL);
	memcpy(light_vscreen,dark_vscreen,help_buf_size);
	for(i=0;i<=101;i++){m_Sprite8(0,i*8,8,indestructible,light_vscreen,dark_vscreen,SPRT_XOR);
											m_Sprite8(152,i*8,8,indestructible,light_vscreen,dark_vscreen,SPRT_XOR);}
	for(i=95;i<=140;i+=15)m_Sprite8(i,10,8,cannon,light_vscreen,dark_vscreen,SPRT_XOR);
	unsigned char *sprite_list[]={missile_up,super_missile_up,super_missile_down,missile_down,regular,broken_1,broken_2,
																indestructible,mine_2,mine_1,mine_3,cash_pickup,boost_pickup,shield_pickup,reg_exhaust_2};
	unsigned short x_list[]={110,130,120,140,112,114,138,120,27,80,133,8,8,8,116};
	unsigned short y_list[]={17,17,21,21,100,118,118,142,194,194,194,250,262,274,430};
	for(i=0;i<=14;i++)m_Sprite8(x_list[i],y_list[i],8,sprite_list[i],light_vscreen,dark_vscreen,SPRT_XOR);
	m_Sprite16(8+116,71*6+4,8,jet_reg,light_vscreen,dark_vscreen,SPRT_XOR);
	m_Sprite16(8+120,71*6,16,shield_1,light_vscreen,dark_vscreen,SPRT_XOR);
	SetPlane(0);
	//draw the sprites
	while(!_keytest(RR_ESC)){
		FastCopyScreen(light_vscreen+30*y,GetPlane(0));
		FastCopyScreen(dark_vscreen+30*y,GetPlane(1));
		if(_keytest(RR_DOWN)&&y<102*6-LCD_HEIGHT)y+=2;
		if(_keytest(RR_UP)&&y)y-=2;
		if(_keytest(RR_PLUS))OSContrastUp();
		if(_keytest(RR_MINUS))OSContrastDn();
		if(_keytest(RR_F5))exit(0);
		delay(25);
	}
	memset(GetPlane(0),0x00,3840);
	memset(GetPlane(1),0x00,3840);
	realloc(light_vscreen,3840);
	realloc(dark_vscreen,3840);
	sprt_bound.downright_y=LCD_HEIGHT;
	delay(100);
}
void doQuit(void){
	clearScreens();
	delay(200);
	short x=0,y=0,pos=NO,ex=EX_1;
	SHOT mine={random(LCD_WIDTH),random(LCD_HEIGHT),0,0,1};
	while(1){
		GrayDrawStrExt2B(32,0,(char *)"Really Quit?",A_REVERSE,F_8x10,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(26,45,(char *)"I guess so...",A_NORMAL,F_8x10,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(26,25,(char *)"No, I must play!",A_NORMAL,F_8x10,light_vscreen,dark_vscreen);
		if(pos==YES)x=1,y=46;
		else if(pos==NO)x=1,y=26;
		if(ex<=EX_2)m_Sprite8(x,y,8,reg_exhaust_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(ex<=EX_4)m_Sprite8(x,y,8,reg_exhaust_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(ex<=EX_6)m_Sprite8(x,y,8,reg_exhaust_3,light_vscreen,dark_vscreen,SPRT_XOR);
		if(++ex>EX_6)ex=EX_1;
		m_Sprite16(x+8,y,8,jet_reg,light_vscreen,dark_vscreen,SPRT_XOR);
		if(mine.type<=3)m_Sprite8(mine.x,mine.y,8,mine_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=6)m_Sprite8(mine.x,mine.y,8,mine_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=9)m_Sprite8(mine.x,mine.y,8,mine_3,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=12)m_Sprite8(mine.x,mine.y,8,mine_ex_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=15)m_Sprite16(mine.x-4,mine.y-4,16,mine_ex_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=18)m_Sprite16(mine.x-4,mine.y-4,16,mine_ex_3,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(mine.type<=21)m_Sprite16(mine.x-4,mine.y-4,16,mine_ex_4,light_vscreen,dark_vscreen,SPRT_XOR);
		if(++mine.type>21)mine=(SHOT){random(LCD_WIDTH),random(LCD_HEIGHT),0,0,1};
		FastCopyScreen(light_vscreen,GetPlane(0));
		FastCopyScreen(dark_vscreen,GetPlane(1));
		clearScreens();
		if(_keytest(RR_DOWN)&&pos==NO)pos=YES;
		if(_keytest(RR_UP)&&pos==YES)pos=NO;
		if(_keytest(RR_2ND)||_keytest(RR_ENTER)){
			if(pos==YES)exit(0);
			else if(pos==NO)break;
		}
		if(_keytest(RR_PLUS))OSContrastUp();
		if(_keytest(RR_MINUS))OSContrastDn();
		if(_keytest(RR_F5))exit(0);
		delay(25);
	}
	delay(200);
}
/*#################### The Jet Shop ################*/
void gotoShop(void){
	delay(100);
	clearScreens();
	ptr.x=159;
	short x=0,y=0,pos=PLAY,ex=EX_1;
	while(1){
		clearScreens();
		GrayDrawStrExt2B(32,0,(char *)"The Jet Shop",A_REVERSE,F_8x10,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(26,20,(char *)"Play",A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(100,20,(char *)"Weapons",A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(26,40,(char *)"Special",A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(100,40,(char *)"Jet Mods",A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(64,60,(char *)"Quit",A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		jet.boost=jet.boost_max,jet.shield=jet.shield_max;
		drawStats();
		if(pos==PLAY)x=1,y=20;
		else if(pos==WEAPONS)x=75,y=20;
		else if(pos==SPECIAL)x=1,y=40;
		else if(pos==JET_MOD)x=75,y=40;
		else if(pos==QUIT)x=39,y=60;
		if(ex<=EX_2)m_Sprite8(x,y,8,reg_exhaust_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(ex<=EX_4)m_Sprite8(x,y,8,reg_exhaust_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(ex<=EX_6)m_Sprite8(x,y,8,reg_exhaust_3,light_vscreen,dark_vscreen,SPRT_XOR);
		if(++ex>EX_6)ex=EX_1;
		m_Sprite16(x+8,y,8,jet_reg,light_vscreen,dark_vscreen,SPRT_XOR);
		FastCopyScreen(light_vscreen,GetPlane(0));
		FastCopyScreen(dark_vscreen,GetPlane(1));
		clearScreens();
		if(_keytest(RR_UP)){if(pos==SPECIAL)pos=PLAY;else if(pos==JET_MOD)pos=WEAPONS;else if(pos==QUIT)pos=SPECIAL;}
		if(_keytest(RR_DOWN)){if(pos==PLAY)pos=SPECIAL;else if(pos==WEAPONS)pos=JET_MOD;else if(pos==SPECIAL||pos==JET_MOD)pos=QUIT;}
		if(_keytest(RR_RIGHT)){if(pos==PLAY)pos=WEAPONS;else if(pos==SPECIAL)pos=JET_MOD;}
		if(_keytest(RR_LEFT)){if(pos==WEAPONS)pos=PLAY;else if(pos==JET_MOD)pos=SPECIAL;}
		if(_keytest(RR_2ND)||_keytest(RR_ENTER)){
			if(pos==WEAPONS)doWeaponShop();
			else if(pos==SPECIAL)doSpecialShop();
			else if(pos==JET_MOD)doJetModShop();
			else if(pos==QUIT)doQuit();
			else if(pos==PLAY)doGame();
		}
		if(_keytest(RR_PLUS))OSContrastUp();
		if(_keytest(RR_MINUS))OSContrastDn();
		if(_keytest(RR_ESC))doGame();
		if(_keytest(RR_F5))exit(0);
		if(_keytest(RR_F2))jet.cash+=25000;
		if(_keytest(RR_F1))saveGame();
		if(_keytest(RR_F4))loadGame();
		if(_keytest(RR_CLEAR))ptr.lvl++;
		delay(0);
	}
}
void doWeaponShop(void){
	short x=0,y=0,pos=CANNON,ex=EX_1;
	char cost[8],name[20];
	delay(100);
	while(1){
		clearScreens();
		GrayDrawStrExt2B(52,0,(char *)"WEAPONS",A_REVERSE,F_8x10,light_vscreen,dark_vscreen);
		if(jet.cannon==1){sprintf(name,"Cannon Lvl 2 -");sprintf(cost,"$30,000");}
		else if(jet.cannon==2){sprintf(name,"Cannon Lvl 3 -");sprintf(cost,"$100,000");}
		else if(jet.cannon==3){sprintf(name,"Cannon Lvl 4 -");sprintf(cost,"$200,000");}
		else {sprintf(name,"-Cannon At Max-");sprintf(cost," ");}
		GrayDrawStrExt2B(26,20,(char *)name,A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(94,28,(char *)cost,A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		if(jet.missile==0){sprintf(name,"Missiles -");sprintf(cost,"$7,500");}
		else if(jet.missile==1){sprintf(name,"Super Missiles -");sprintf(cost,"$75,000");}
		else {sprintf(name,"-Missiles At Max-");sprintf(cost," ");}
		GrayDrawStrExt2B(26,36,(char *)name,A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(94,44,(char *)cost,A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		if(jet.repeat_rate_c==4){sprintf(name,"Rapid Fire Mod -");sprintf(cost,"$150,000");}
		else {sprintf(name,"-Rapid Fire At Max-");sprintf(cost," ");}
		GrayDrawStrExt2B(26,52,(char *)name,A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(94,60,(char *)cost,A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(26,68,(char *)"Back",A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		jet.boost=jet.boost_max,jet.shield=jet.shield_max;
		drawStats();
		if(pos==CANNON)x=1,y=20;
		else if(pos==MISSILE)x=1,y=36;
		else if(pos==RAPID)x=1,y=52;
		else if(pos==BACK)x=1,y=68;
		if(ex<=EX_2)m_Sprite8(x,y,8,reg_exhaust_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(ex<=EX_4)m_Sprite8(x,y,8,reg_exhaust_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(ex<=EX_6)m_Sprite8(x,y,8,reg_exhaust_3,light_vscreen,dark_vscreen,SPRT_XOR);
		if(++ex>EX_6)ex=EX_1;
		m_Sprite16(x+8,y,8,jet_reg,light_vscreen,dark_vscreen,SPRT_XOR);
		FastCopyScreen(light_vscreen,GetPlane(0));
		FastCopyScreen(dark_vscreen,GetPlane(1));
		clearScreens();
		if(_keytest(RR_UP)){if(pos==MISSILE)pos=CANNON;else if(pos==RAPID)pos=MISSILE;else if(pos==BACK)pos=RAPID;}
		if(_keytest(RR_DOWN)){if(pos==CANNON)pos=MISSILE;else if(pos==MISSILE)pos=RAPID;else if(pos==RAPID)pos=BACK;}
		if(_keytest(RR_2ND)||_keytest(RR_ENTER)){
			if(pos==CANNON){
				if(jet.cannon==1&&jet.cash>=30000)jet.cannon++,jet.cash-=30000;
				else if(jet.cannon==2&&jet.cash>=100000)jet.cannon++,jet.cash-=100000;
				else if(jet.cannon==3&&jet.cash>=200000)jet.cannon++,jet.cash-=200000;
			}else if(pos==MISSILE){
				if(jet.missile==0&&jet.cash>=7500)jet.missile++,jet.cash-=7500;
				else if(jet.missile==1&&jet.cash>=75000)jet.missile++,jet.cash-=75000;
			}else if(pos==RAPID){
				if(jet.repeat_rate_c==4&&jet.cash>=150000)jet.repeat_rate_c/=2,jet.cash-=150000;
			}else if(pos==BACK)break;
		}
		if(_keytest(RR_PLUS))OSContrastUp();
		if(_keytest(RR_MINUS))OSContrastDn();
		if(_keytest(RR_ESC))break;
		if(_keytest(RR_F5))exit(0);
		if(_keytest(RR_F2))jet.cash+=25000;
		if(_keytest(RR_F1))saveGame();
		if(_keytest(RR_F4))loadGame();
		if(_keytest(RR_CLEAR))ptr.lvl++;
		delay(0);
	}
	delay(100);
}
void doJetModShop(void){
	short x=0,y=0,pos=WINGS,ex=EX_1;
	char cost[8],name[20];
	delay(100);
	while(1){
		clearScreens();
		GrayDrawStrExt2B(48,0,(char *)"JET MODS",A_REVERSE,F_8x10,light_vscreen,dark_vscreen);
		if(jet.move_rate_v==2){sprintf(name,"Wing Mod -");sprintf(cost,"$50,000");}
		else{sprintf(name,"-Wings Maxed-");sprintf(cost," ");}
		GrayDrawStrExt2B(26,20,(char *)name,A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(94,28,(char *)cost,A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		if(jet.move_rate_h==2){sprintf(name,"Thruster Mod -");sprintf(cost,"$50,000");}
		else{sprintf(name,"-Thrusters Maxed-");sprintf(cost," ");}
		GrayDrawStrExt2B(26,36,(char *)name,A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(94,44,(char *)cost,A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(26,52,(char *)"Back",A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		jet.boost=jet.boost_max,jet.shield=jet.shield_max;
		drawStats();
		if(pos==WINGS)x=1,y=20;
		else if(pos==THRUST)x=1,y=36;
		else if(pos==BACK)x=1,y=52;
		if(ex<=EX_2)m_Sprite8(x,y,8,reg_exhaust_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(ex<=EX_4)m_Sprite8(x,y,8,reg_exhaust_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(ex<=EX_6)m_Sprite8(x,y,8,reg_exhaust_3,light_vscreen,dark_vscreen,SPRT_XOR);
		if(++ex>EX_6)ex=EX_1;
		m_Sprite16(x+8,y,8,jet_reg,light_vscreen,dark_vscreen,SPRT_XOR);
		FastCopyScreen(light_vscreen,GetPlane(0));
		FastCopyScreen(dark_vscreen,GetPlane(1));
		clearScreens();
		if(_keytest(RR_UP)){if(pos==THRUST)pos=WINGS;else if(pos==BACK)pos=WINGS;}
		if(_keytest(RR_DOWN)){if(pos==WINGS)pos=THRUST;else if(pos==THRUST)pos=BACK;}
		if(_keytest(RR_2ND)||_keytest(RR_ENTER)){
			if(pos==WINGS){
				if(jet.move_rate_v==2&&jet.cash>=50000)jet.move_rate_v*=2,jet.cash-=50000;
			}else if(pos==THRUST){
				if(jet.move_rate_h==2&&jet.cash>=50000)jet.move_rate_h*=2,jet.cash-=50000;
			}else if(pos==BACK)break;
		}
		if(_keytest(RR_PLUS))OSContrastUp();
		if(_keytest(RR_MINUS))OSContrastDn();
		if(_keytest(RR_ESC))break;
		if(_keytest(RR_F5))exit(0);
		if(_keytest(RR_F2))jet.cash+=25000;
		if(_keytest(RR_F1))saveGame();
		if(_keytest(RR_F4))loadGame();
		if(_keytest(RR_CLEAR))ptr.lvl++;
		delay(0);
	}
	delay(100);
}
void doSpecialShop(void){
	short x=0,y=0,pos=BOOST,ex=EX_1;
	char cost[8],name[20];
	delay(100);
	while(1){
		clearScreens();
		GrayDrawStrExt2B(52,0,(char *)"SPECIAL",A_REVERSE,F_8x10,light_vscreen,dark_vscreen);
		if(jet.boost_max==0){sprintf(name,"Boost Lvl 1 -");sprintf(cost,"$50,000");}
		else if(jet.boost_max==50){sprintf(name,"Boost Lvl 2 -");sprintf(cost,"$100,000");}
		else{sprintf(name,"-Booster Maxed-");sprintf(cost," ");}
		GrayDrawStrExt2B(26,20,(char *)name,A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(94,28,(char *)cost,A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		if(jet.shield_max==0){sprintf(name,"Shield Lvl 1 -");sprintf(cost,"$10,000");}
		else if(jet.shield_max==1){sprintf(name,"Shield Lvl 2 -");sprintf(cost,"$15,000");}
		else if(jet.shield_max==2){sprintf(name,"Shield Lvl 3 -");sprintf(cost,"$25,000");}
		else if(jet.shield_max==3){sprintf(name,"Shield Lvl 4 -");sprintf(cost,"$50,000");}
		else if(jet.shield_max==4){sprintf(name,"Shield Lvl 5 -");sprintf(cost,"$75,000");}
		else{sprintf(name,"-Shields Maxed-");sprintf(cost," ");}
		GrayDrawStrExt2B(26,36,(char *)name,A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(94,44,(char *)cost,A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		GrayDrawStrExt2B(26,52,(char *)"Back",A_NORMAL,F_6x8,light_vscreen,dark_vscreen);
		jet.boost=jet.boost_max,jet.shield=jet.shield_max;
		drawStats();
		if(pos==BOOST)x=1,y=20;
		else if(pos==SHIELD)x=1,y=36;
		else if(pos==BACK)x=1,y=52;
		if(ex<=EX_2)m_Sprite8(x,y,8,reg_exhaust_1,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(ex<=EX_4)m_Sprite8(x,y,8,reg_exhaust_2,light_vscreen,dark_vscreen,SPRT_XOR);
		else if(ex<=EX_6)m_Sprite8(x,y,8,reg_exhaust_3,light_vscreen,dark_vscreen,SPRT_XOR);
		if(++ex>EX_6)ex=EX_1;
		m_Sprite16(x+8,y,8,jet_reg,light_vscreen,dark_vscreen,SPRT_XOR);
		FastCopyScreen(light_vscreen,GetPlane(0));
		FastCopyScreen(dark_vscreen,GetPlane(1));
		clearScreens();
		if(_keytest(RR_UP)){if(pos==SHIELD)pos=BOOST;else if(pos==BACK)pos=SHIELD;}
		if(_keytest(RR_DOWN)){if(pos==BOOST)pos=SHIELD;else if(pos==SHIELD)pos=BACK;}
		if(_keytest(RR_2ND)||_keytest(RR_ENTER)){
			if(pos==BOOST){
				if(jet.boost_max==0&&jet.cash>=50000)jet.boost_max+=50,jet.cash-=50000;
				else if(jet.boost_max==50&&jet.cash>=100000)jet.boost_max+=50,jet.cash-=100000;
			}else if(pos==SHIELD){
				if(jet.shield_max==0&&jet.cash>=10000)jet.shield_max++,jet.cash-=10000;
				else if(jet.shield_max==1&&jet.cash>=15000)jet.shield_max++,jet.cash-=15000;
				else if(jet.shield_max==2&&jet.cash>=25000)jet.shield_max++,jet.cash-=25000;
				else if(jet.shield_max==3&&jet.cash>=50000)jet.shield_max++,jet.cash-=50000;
				else if(jet.shield_max==4&&jet.cash>=75000)jet.shield_max++,jet.cash-=75000;
			}else if(pos==BACK)break;
		}
		if(_keytest(RR_PLUS))OSContrastUp();
		if(_keytest(RR_MINUS))OSContrastDn();
		if(_keytest(RR_ESC))break;
		if(_keytest(RR_F5))exit(0);
		if(_keytest(RR_F2))jet.cash+=25000;
		if(_keytest(RR_F1))saveGame();
		if(_keytest(RR_F4))loadGame();
		if(_keytest(RR_CLEAR))ptr.lvl++;
		delay(0);
	}
	delay(100);
}
/*############## Winning ##################*/
void doWin(void){
	//okay, lets see what i need here
	// im going to need some form of ending to satisfy gamers, even if it is kind of lame
	// maybe also give them the cheats now that they have won the game once
	// then i will need to test for a new hiscore and then get name input if it is a new high score
	// then display the high score table
	clearScreens();
	memset(GetPlane(0),0x00,3840);
	memset(GetPlane(1),0x00,3840);
	FontSetSys(F_8x10);
	DrawGrayStr(0,0,"Congratulations!",A_NORMAL);
	DrawGrayStr(0,10,"You Win!",A_NORMAL);
	char str[17]="                 ";
	sprintf(str,"Score: %lu",jet.score);
	DrawGrayStr(0,20,str,A_NORMAL);
	DrawGrayStr(0,50,"Press Enter...",A_NORMAL);
	//while(!_keytest(RR_ENTER));
	FILE *f=NULL;
	if((f=fopen("b2save","rb"))==NULL){
		printf("file io error 14\npress esc to exit\n");
		while(!_keytest(RR_ESC));
		exit(0);
	}
	if(fread(&save,sizeof(SAVE),1,f)!=1){
		printf("file io error 15\npress esc to exit\n");
		while(!_keytest(RR_ESC));
		exit(0);
	}
	fclose(f);
	sprintf(str,"Hi Score: %lu",save.hiScores[1].score);
	DrawGrayStr(0,30,str,A_NORMAL);
	//if(jet.score>save.hiScores[5].score)newHiScore();
	if(jet.score>save.hiScores[1].score)DrawGrayStr(0,40,"New High Score!!!",A_NORMAL);
	else DrawGrayStr(0,40,"No New High Score",A_NORMAL);
	while(!_keytest(RR_ENTER));
	exit(0);
	//showHiScores();
}
/*void newHiScore(void){
	GrayOff();
	SetIntVec(AUTO_INT_1,interrupt1);
	SetIntVec(AUTO_INT_5,interrupt5);
	DrawStr(0,30,"New High Score!",A_NORMAL);
	ngetchx();
	clrscr();
	DrawStr(20,0,"Enter Your Name",A_REVERSE);
	FontSetSys(F_6x8);

	int i=0;
	/*int key=0;
	int x=0;
	int y=45;
	//int back=0;
	//int pulse=0;
	void *kq=kbd_queue();
	OSqclear(kq);
	while(1){
		if(OSdequeue(&key,kq)){
			if(key==KEY_BACKSPACE){
				save.hiScores[0].name[x]=' ';
				DrawGrayStr(49+6*x,y," ",A_REPLACE);
				if(--x<0)x=0;
			} else if(key==KEY_ENTER||key==KEY_ESC){
				break;
			} else {
				save.hiScores[0].name[x]=key;
				DrawGrayStr(49+6*x,y,(char)key,A_REPLACE);
				if(++x>9)x=9;
			}

		}
	}*/
	// ripped temporarily from ai by jason ho
	/*int key=0;
	int x=49;
	int y=45;
	int c;
	for(c=0;c<9;c++)save.hiScores[0].name[c]=' ';
	printf_xy(x,y,"_");
   for(;;){
   	key=ngetchx();
   	if(key==264) continue;
   	if(key==13) break;
   	if(key==257&&i>0){
   		i--;
   		save.hiScores[0].name[i]=' ';
   		printf_xy(x+i*6,y,"_ ");
   	}
   	if(key!=257&&i<9){
   		save.hiScores[0].name[i]=key;
   		printf_xy(x+i*6,y,"%c_ ",save.hiScores[0].name[i]);
   		i++;
   	}
  }
	FontSetSys(F_8x10);
	save.hiScores[0].score=jet.score;
	/*if(save.hiScores[0].score>save.hiScores[5].score){
		save.hiScores[5]=save.hiScores[0];
		if(save.hiScores[0].score>save.hiScores[4].score){
			save.hiScores[5]=save.hiScores[4];
			save.hiScores[4]=save.hiScores[0];
			if(save.hiScores[0].score>save.hiScores[3].score){
				save.hiScores[4]=save.hiScores[3];
				save.hiScores[3]=save.hiScores[0];
				if(save.hiScores[0].score>save.hiScores[2].score){
					save.hiScores[3].score=save.hiScores[2].score;
					save.hiScores[2].score=save.hiScores[0].score;
					if(save.hiScores[0].score>save.hiScores[1].score){
						save.hiScores[2].score=save.hiScores[1].score;
						save.hiScores[1].score=save.hiScores[0].score;
					}
				}
			}
		}
	}*/
	/*for(i=5;i>=1;i--){
		if(save.hiScores[0].score>save.hiScores[i].score){
			if(i!=5)save.hiScores[i+1]=save.hiScores[i];
			save.hiScores[i]=save.hiScores[0];
		}
	}
	save.jet=jet;
	save.ptr=ptr;
	save.speed=speed;
	FILE *f;
	if((f=fopen("b2save","wb"))==NULL){
		/*printf("file io error 16\npress esc to exit\n");
		while(!_keytest(RR_ESC));
		exit(0);*/
	/*}
	/*if(fwrite(&save,sizeof(SAVE),1,f)!=1){
		/*printf("file io error 17\npress esc to exit\n");
		while(!_keytest(RR_ESC));
		exit(0);*/
	/*}
	// append the file ID tag
	/*fputc(0,f);
	fputs("SAV",f);
	fputc(0,f);
	fputc(OTH_TAG,f);
	fclose(f);
	showHiScores();
}
void showHiScores(void){
	clrscr();
	int i;
	DrawStr(36,0,"High Scores",A_REVERSE);
	FontSetSys(F_6x8);
	for(i=1;i<=5;i++)printf_xy(10,i*9+26,"%d. %s - %lu",i,save.hiScores[i].name,save.hiScores[i].score);
	while (ngetchx()!=KEY_ENTER);
	SetIntVec(AUTO_INT_1,DUMMY_HANDLER);
	SetIntVec(AUTO_INT_5,DUMMY_HANDLER);
	GrayOn();
	exit(0);
}*/