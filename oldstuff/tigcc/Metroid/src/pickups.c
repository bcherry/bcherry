// C Source File
// Created 3/23/2005; 11:37:13 PM

#include "all.h"


int pickup(struct object* this) {
	if(this->timer-- < 0) {
		this->gone = 1;
		return 0;
	} else {
		if(hitSamus(this->x, this->y, this->w, this->h)) {
			this->gone = 1;
			return 1;
		}
	}
	return 0;
}

void energyPickup(struct object* this) {
	if(pickup(this)) {
		samus->HP+=(random(20) + 10);
		while(samus->HP > 99 && samus->items.tanks_full < samus->items.tanks_owned) {
			samus->HP-=100;
			samus->items.tanks_full++;
		}
		if(samus->HP > 99)
			samus->HP = 99;
	}
}

void missilePickup(struct object* this) {
	if(pickup(this)) {
		samus->weapons.missile_ammo+=this->ATK;
		if(samus->weapons.missile_ammo > samus->weapons.missile_max)
			samus->weapons.missile_ammo = samus->weapons.missile_max;
	}
}

void superMissilePickup(struct object* this) {
	if(pickup(this)) {
		samus->weapons.superMissile_ammo+=this->ATK;
		if(samus->weapons.superMissile_ammo > samus->weapons.superMissile_max)
			samus->weapons.superMissile_ammo = samus->weapons.superMissile_max;
	}
}

void powerBombPickup(struct object* this) {
	if(pickup(this)) {
		samus->weapons.powerBomb_ammo+=this->ATK;
		if(samus->weapons.powerBomb_ammo > samus->weapons.powerBomb_max)
			samus->weapons.powerBomb_ammo = samus->weapons.powerBomb_max;
	}
}