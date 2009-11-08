// Header File
// Created 1/31/2005; 12:37:28 AM

#include <tigcclib.h>

#include "graphics.h"
#include "extgraph.h"
#include "tiles.h"
#include "Enemies.h"
#include "Maps.h"
#include "Sprites.h"
#include "Objects.h"
#include "Samus.h"
#include "textBox.h"
//#include "polysnd2.h"

#define SHOOT RR_2ND
#define JUMP RR_DIAMOND
#define SPRINT RR_ALPHA
#define AIM RR_SHIFT

#define USE_VTI
//#define DEBUG

struct game {
	int (*mode)(void);
	int frame;
	struct {
		char up;
		char down;
		char left;
		char right;
		char fire;
		char sprint;
		char jump;
		char aim;
		char f1, f2, f3, f4, f5;
	} keys;
};

void makeGame(void);
void delay(short);
int playingMode(void);
void pause(char);
int boundingBoxCollide(int,int,int,int,int,int,int,int,int);
void getKeys(void);
void setContrast(int);
void ContrastUp(int);
void ContrastDn(int);
int checkDoors(void);
int mapTransition(void);
void drawDoors(void);

void *v0, *v1;
struct enemyList* enemies;
struct objlist* objects;
struct samus* samus;
struct cam* cam;
struct game* game;
struct map* map;
struct graphics* gfx;
struct tile* tiles;
struct enemy* enemyKey;
struct sprites* sprites;
struct templates* templates;
