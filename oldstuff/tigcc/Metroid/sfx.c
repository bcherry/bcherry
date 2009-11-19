// C Source File
// Created 2/10/2005; 1:28:19 PM

#include <tigcclib.h>
#include "all.h"
/*
int bombBreakEffect(struct object* this) {
	if(--this->time < 0) {
		map->array[(this->y/8-1)*map->w+(this->x/8)] = 9;
		return 0;
	}
	return 1;
}*/

void bombExplodeFX(struct object* this) {
	this->timer--;
	if(this->timer < 0)
		this->gone = 1;
}
/*
int breakAwayEffect(Effect this) {
	if(--this->time == breakAway->time-2) {
		map->array[((this->y-8)/8)*map->w+(this->x/8)] = 0;
		this->sprt = &sprites.sfx.breakAway;
	} else if(this->time < 0) {
		map->array[((this->y-8)/8)*map->w+(this->x/8)] = 7;
		return 0;
	}
	//pause('a');
	return 1;
}*/

void crumblingFX(struct object* this) {
	if(--this->timer < 0 && !hitSamus(this->x +4, this->y, 8, 8)) {
		this->gone = 1;
		map->array[1-map->array[((this->y-8)/8)*map->w]+(this->y-8)/8*map->w+this->x/8] = this->e1;
	}
}

void spawnPickup(struct object* this) {
	void* t = NULL;
	switch(this->type) {
		case 0:
			return;
		case 'e':
			t = &templates->pickups.energy;
			break;
		case 'm':
			t = &templates->pickups.missile;
			break;
		case 's':
			t = &templates->pickups.superMissile;
			break;
		case 'p':
			t = &templates->pickups.powerBomb;
			break;
	}
	pushObject(this->x + this->w/2 - 4, this->y - this->h/2 + 4, 0, 0, t);
}

void simpleEnemyExplosionFX(struct object* this) {
	if(--this->timer < 0) {
		spawnPickup(this);
		this->gone = 1;
	}
}