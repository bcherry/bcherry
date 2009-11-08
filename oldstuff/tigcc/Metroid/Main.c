// C Source File
// Created 1/27/2005; 8:16:31 PM

#include "all.h"

INT_HANDLER int_1 = DUMMY_HANDLER;
INT_HANDLER int_5 = DUMMY_HANDLER;

atexit_t exitStuff(void) {
	GrayClearScreen();
	GrayOff();
	freeObjects();
	freeEnemies();
	free(v0);
	free(v1);
	free(samus);
	free(game);
	free(cam);
	free(enemies);
	free(map);
	free(gfx);
	free(objects);
	free(sprites);
	free(templates);
	free(tiles);
	free(enemyKey);
	SetIntVec(AUTO_INT_1, int_1);
	SetIntVec(AUTO_INT_5, int_5);
	setContrast(18);
	return 0;
}

void initialize(void) {
	ST_helpMsg("loading...");
	atexit((atexit_t)exitStuff);
	makeGfx();
	makeSprites();
	makeTiles();
	makeObjects();
	makeEnemies();
	makeSamus();
	makeMaps();
	makeGame();
	int_1 = GetIntVec(AUTO_INT_1);
	int_5 = GetIntVec(AUTO_INT_5);
	SetIntVec(AUTO_INT_1, DUMMY_HANDLER);
	SetIntVec(AUTO_INT_5, DUMMY_HANDLER);
	GrayOn();
	GrayClearScreen2B(v0,v1);
	randomize();
	loadMap(5, 0, 0, 0);
	//rightFlip = rightFlip1;
	//leftFlip = leftFlip1;
}

void _main(void)
{
	initialize();
	while(game->mode());
}

void setContrast(int n) {
	#ifndef USE_VTI
	ContrastDn(40);
	ContrastUp(n);
	#endif
}

void ContrastDn(int n) {
	#ifndef USE_VTI
	while(n--)
		OSContrastDn();
	#endif
}

void ContrastUp(int n) {
	#ifndef USE_VTI
	while(n--)
		OSContrastUp();
	#endif
}



void delay(short number) {
	short loop, loop2;
	for (loop=0;loop<number;loop++) {
		for (loop2=0;loop2<1000;loop2++);
	}
}

void pause(char c) {
	#ifdef DEBUG
	printf("%c", c);
	while(!_keytest(RR_ENTER));
	delay(100);
	#endif
}

void freeObjects(void) {
	struct objnode* curr = objects->first;
	while(curr != NULL) {
		struct objnode* next = curr->link;
		free(curr->value);
		free(curr);
		curr = next;
	}
	objects->first=NULL;
}
void freeEnemies(void) {
	struct enemyNode* curr = enemies->first;
	while(curr != NULL) {
		struct enemyNode* next = curr->next;
		free(curr->value);
		free(curr);
		curr = next;
	}
	enemies->first = NULL;
}
