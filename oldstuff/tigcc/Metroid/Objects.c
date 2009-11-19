// C Source File
// Created 3/21/2005; 10:58:28 PM

#include "all.h"

void makeObjects() {
	struct templates t = {
		{/*############ WEAPONS ##############*/
			{/************POWERBEAM****************/
				0, 0,													// X, Y
				4, 4,													// H, W
				5, 0,													// VX, VY
				10, 0,												// ATK, TIM
				0, 0, 0,											// E 1-3
				1,														// TYPE
				1, 0,													// FRAMES, GONE
				&sprites->weapons.powerBeam,	// SPRITE
				basicShot											// BEHAVIOR
			},{/**********MISSILE*******************/
				0, 0,
				8, 8,
				2, 0,
				100, 0,
				0, 0, 0,
				2,
				1, 0,
				&sprites->weapons.missile1,
				missileShot
			},{/************SUPER MISSILE************/
				0, 0,
				8, 8,
				2, 0,
				500, 0,
				0, 0, 0,
				3,
				1, 0,
				&sprites->weapons.missile1,
				missileShot
			},{/*************BOMB********************/
				0, 0,
				4, 4,
				0, 0,
				50, 20,
				0, 0, 0,
				2,
				1, 0,
				&sprites->weapons.powerBeam,
				bombShot
			},{/***********POWERBOMB****************/
				0, 0,
				4, 4,
				0, 0,
				200, 20,
				0, 0, 0,
				4,
				1, 0,
				&sprites->weapons.powerBeam,
				bombShot
			}
		},{/*############ EFFECTS #############*/
			{/************BOMB XPLOSION***************/
				0, 0,
				0, 0,
				0, 0,
				0, 3,
				0, 0, 0,
				0,
				2, 0,
				&sprites->sfx.bombExplode,
				bombExplodeFX
			},{/***********CRUMBLING*****************/
				0, 0,
				8, 8,
				0, 0,
				0, 15,
				0, 0, 0,
				0,
				2, 0,
				&sprites->sfx.breakAway,
				crumblingFX
			},{/************SIMPLE DEATH*************/
				0, 0,
				16, 16,
				0, 0,
				0, 8,
				0, 0, 0,
				0,
				3, 0,
				&sprites->sfx.simpleEnemyExplosion,
				simpleEnemyExplosionFX
			}
		},{/*#############PICKUPS##############*/
			{/***********ENERGY*******************/
				0, 0,
				8, 8,
				0, 0,
				25, 150,
				0, 0, 0,
				0,
				3, 0,
				&sprites->pickups.energy,
				energyPickup
			},{/***********MISSILE***************/
				0, 0,
				8, 4,
				0, 0,
				2, 150,
				0, 0, 0,
				0,
				3, 0,
				&sprites->pickups.missile,
				missilePickup
			},{/*************SUPER MISSILE**********/
				0, 0,
				8, 4,
				0, 0,
				2, 150,
				0, 0, 0,
				0,
				3, 0,
				&sprites->pickups.superMissile,
				superMissilePickup
			},{/*************POWERBOMB***************/
				0, 0,
				8, 4,
				0, 0,
				2, 150,
				0, 0, 0,
				0,
				3, 0,
				&sprites->pickups.powerBomb,
				powerBombPickup
			}
		},{/*############ ITEMS ###############*/
			{/*********MISSILE************/
				0, 0,
				8, 8,
				0, 0,
				5, 0,
				0, 0, 0,
				0,
				1, 0,
				&sprites->items.missile,
				missileItem
			},{/*******SUPER MISSILE*********/
				0, 0,
				8, 8,
				0, 0,
				5, 0,
				0, 0, 0,
				0,
				1, 0,
				&sprites->items.superMissile,
				superMissileItem
			},{/********POWERBOMB**********/
				0, 0,
				8, 8,
				0, 0,
				5, 0,
				0, 0, 0,
				0,
				1, 0,
				&sprites->items.powerBomb,
				powerBombItem
			},{/********ENERGY TANK*********/
				0, 0,
				8, 8,
				0, 0,
				100, 0,
				0, 0, 0,
				0,
				1, 0,
				&sprites->items.energy,
				energyTankItem
			}
		}
	};
	if((templates = malloc(sizeof(t))) == NULL)
		exit(0);
	memcpy(templates, &t, sizeof(t));
	if((objects = malloc(sizeof(struct objlist))) == NULL)
		exit(0);
	objects->first = NULL;
}

void drawObject(struct object* this) {
	if(this->sprt->w > 8) {
		if(this->sprt->m == NULL)
			GrayClipSprite16_OR_R(this->x-cam->x,this->y-cam->y-this->sprt->h,this->sprt->h,this->sprt->s1,this->sprt->s2,v0,v1);
		else
			GrayClipSprite16_SMASK_R(this->x-cam->x,this->y-cam->y-this->sprt->h,this->sprt->h,this->sprt->s1,this->sprt->s2,this->sprt->m,v0,v1);
	} else {
		if(this->sprt->m == NULL)
			GrayClipSprite8_OR_R(this->x-cam->x,this->y-cam->y-this->sprt->h,this->sprt->h,this->sprt->s1,this->sprt->s2,v0,v1);
		else
			GrayClipSprite8_SMASK_R(this->x-cam->x, this->y-cam->y-this->sprt->h,this->sprt->h,this->sprt->s1,this->sprt->s2,this->sprt->m,v0,v1);
	}
}

void pushObject(int x, int y, int vel_x, int vel_y, struct object* template) {
	struct object* this = malloc(sizeof(struct object));
	memcpy(this, template, sizeof(struct object));
	this->x = x;
	this->y = y;
	this->vel_x = vel_x;
	this->vel_y = vel_y;
	struct objnode* node = malloc(sizeof(struct objnode));
	node->value = this;
	node->link = objects->first;
	objects->first = node;
}
