// C Source File
// Created 3/27/2005; 6:29:52 PM

#include "all.h"

void missileItem(struct object* this) {
	if(hitSamus(this->x, this->y, this->w, this->h)) {
		textBox("MISSILES");
		while(!_keytest(RR_ENTER));
		this->gone = 1;
		samus->weapons.missile_ammo+=5;
		samus->weapons.missile_max+=5;
	}
}
void superMissileItem(struct object* this) {
	if(hitSamus(this->x, this->y, this->w, this->h)) {
		textBox("SUPER MISSILES");
		while(!_keytest(RR_ENTER));
		this->gone = 1;
		samus->weapons.superMissile_ammo+=5;
		samus->weapons.superMissile_max+=5;
	}
}
void powerBombItem(struct object* this) {
	if(hitSamus(this->x, this->y, this->w, this->h)) {
		textBox("POWERBOMBS");
		while(!_keytest(RR_ENTER));
		this->gone = 1;
		samus->weapons.powerBomb_ammo+=5;
		samus->weapons.powerBomb_max+=5;
	}
}
void energyTankItem(struct object* this) {
	if(hitSamus(this->x, this->y, this->w, this->h)) {
		textBox("ENERGY TANK");
		while(!_keytest(RR_ENTER));
		this->gone = 1;
		samus->items.tanks_owned++;
		samus->items.tanks_full = samus->items.tanks_owned;
		samus->HP = 99;
	}
}