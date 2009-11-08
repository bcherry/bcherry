// C Source File
// Created 2/10/2005; 4:17:53 PM

#include <tigcclib.h>
#include "all.h"

void hurtEnemy(struct enemy* this, int ATK, int i) {
	if(i < this->immunity || this->immunity < 0)
		return;
	this->HP -= ATK;
	this->hurt = 4;
	if(this->HP <= 0) {
		this->dead = 1;
		if(this->onDeath!=NULL)
			this->onDeath(this);
	}
}

int getPickup(struct enemy* this) {
	int p = random(100);
	if(p < this->E)
		return 'e';
	else if(p+=this->E < this->M)
		return 'm';
	else if(p+=this->M < this->SM)
		return 's';
	else if(p+=this->SM < this->PB)
		return 'p';
	return 0;
}

void skreeDeath(struct enemy* this) {
	enemyKey[3].vel_y=-4;
	enemyKey[3].vel_x=-1;
	pushEnemy(this->x+2,this->y,3);
	enemyKey[3].vel_x=1;
	pushEnemy(this->x+2,this->y,3);
	enemyKey[3].vel_x=-2;
	enemyKey[3].vel_y=-2;
	pushEnemy(this->x+2,this->y,3);
	enemyKey[3].vel_x=2;
	pushEnemy(this->x+2,this->y,3);
	simpleDeath(this);
}

void simpleDeath(struct enemy* this) {
	if(this->dead < 0)
		return;
	pushObject(this->x + this->w/2 - 8, this->y - this->h/2 + 8, 0, 0, &templates->effects.simpleEnemyExplosion);
	objects->first->value->type = getPickup(this);
}

void zebboAI(struct enemy* this) {
	if(this->vel_x<0) {
		this->sprt = &sprites->enemies.zebboL;
	}
	if(this->extra1 > 0) {
		this->y--;
		this->extra1--;
	} else {
		this->x+=this->vel_x*2;
	}
}

void zebboSpawnAI(struct enemy* this) {
	map->array[1-map->array[((this->y-8)/8)*map->w]+((this->y-8)/8)*map->w+this->x/8] = 44;
	if(abs(samus->x-this->x) < 100 && this->y - samus->y < 30) {
		this->extra1++;
		if((this->extra1%=30) == 0) {
			if(samus->x < this->x)
				enemyKey[4].vel_x = -1;
			else
				enemyKey[4].vel_x = 1;
			enemyKey[4].extra1 = this->y-samus->y+8;
			pushEnemy(this->x+4,this->y,4);
		}
	}
}

void spacePirateLaserAI(struct enemy* this) {
	int i;
	for(i = 0; i < 3; i++) {
		this->x+=this->vel_x;
		if(hitWallX(this->x+8+8*this->vel_x,this->y-1,this->y-3,0,0))
			this->dead = -1;
	}
}

void spacePirateAI(struct enemy* this) {
	this->extra1--;
	if((this->y-enemyKey[7].h)-samus->y<-6 && this->y-(samus->y-samus->sprt->h)>6) {
		if(this->sprt==&sprites->enemies.SpacePirateShootRight) {
			if(!random(15)) {
				enemyKey[8].vel_x=1;
				pushEnemy(this->x+8,this->y-7-random(6),8);
			}
		} else if(this->sprt==&sprites->enemies.SpacePirateShootLeft) {
			if(!random(15)) {
				enemyKey[8].vel_x=-1;
				pushEnemy(this->x-8,this->y-7-random(6),8);
			}
		} else if(this->vel_x>0 && samus->x-this->x>0) {
			this->sprt=&sprites->enemies.SpacePirateShootRight;
			return;
		} else if(this->vel_x<0 && samus->x-this->x<0) {
			this->sprt = &sprites->enemies.SpacePirateShootLeft;
			return;
		}
	}
	if(this->extra1==0) {
		if(this->sprt==&sprites->enemies.SpacePirateStandRight) {
			this->sprt=&sprites->enemies.SpacePirateWalk1Left;
			this->vel_x=-1;
		} else {
			this->sprt=&sprites->enemies.SpacePirateWalk1Right;
			this->vel_x=1;
		}
	} else if(this->extra1<0&&this->sprt!=&sprites->enemies.SpacePirateShootRight&&this->sprt!=&sprites->enemies.SpacePirateShootLeft) {
		if(this->sprt==&sprites->enemies.SpacePirateStandRight)
			this->sprt=&sprites->enemies.SpacePirateWalk1Right;
		else if(this->sprt==&sprites->enemies.SpacePirateStandLeft)
			this->sprt=&sprites->enemies.SpacePirateWalk1Left;
		int i;
		for(i = 0; i < enemyKey[7].vel_x; i++) {
			if(hitWallX(this->x+8+(this->vel_x*4),this->y-7,this->y-enemyKey[7].h,0,0) ||
				!hitWallY(this->y+3,(this->x+8+(this->vel_x*4)),(this->x+8+(this->vel_x*4)),0,0)) {
				if(this->vel_x<0)
					this->sprt = &sprites->enemies.SpacePirateStandLeft;
				else
					this->sprt = &sprites->enemies.SpacePirateStandRight;
				this->extra1 = enemyKey[7].extra2;
				return;
			} else {
				this->x+=this->vel_x;
				if(tiles[map->array[1-map->array[((this->y-1)/8)*map->w]+((this->y-1)/8)*map->w+((this->x+8)/8)]].m!=NULL)
					this->y=tiles[map->array[1-map->array[((this->y-1)/8)*map->w]+((this->y-1)/8)*map->w+((this->x+8)/8)]].m[0+(this->x+8)%8] + ((this->y-1)/8)*8;
				else if(tiles[map->array[1-map->array[((this->y+7)/8)*map->w]+((this->y+7)/8)*map->w+((this->x+8)/8)]].m!=NULL) {
					this->y=tiles[map->array[1-map->array[((this->y+7)/8)*map->w]+((this->y+7)/8)*map->w+((this->x+8)/8)]].m[0+(this->x+8)%8] + ((this->y+7)/8)*8;
				}
			}
		}
	} else if(this->extra1==enemyKey[7].extra3) {
		if(this->sprt==&sprites->enemies.SpacePirateStandRight)
			this->sprt=&sprites->enemies.SpacePirateLookRight;
		else
			this->sprt=&sprites->enemies.SpacePirateLookLeft;
	}
}

void ripperAI(struct enemy* this) {
	this->x+=this->vel_x;
	if(this->vel_x<0) {
		if(hitWallX(this->x,this->y-4,this->y-4, 0, 'r')) {
			int y = this->y - 4;
			int x = this->x;
			this->x=tiles[map->array[1-map->array[((y)/8)*map->w]+(y/8)*map->w+x/8]].m[16+y%8] + (x/8)*8;
			this->vel_x*=-1;
			this->sprt = &sprites->enemies.ripper;
		}
	} else if(this->vel_x>0) {
		if(hitWallX(this->x+16,this->y-4,this->y-4, 0, 'l')) {
			int y = this->y - 4;
			int x = this->x + 16;
			this->x=tiles[map->array[1-map->array[((y)/8)*map->w]+(y/8)*map->w+x/8]].m[24+y%8] + (x/8)*8 - 16;
			this->vel_x*=-1;
			this->sprt = &sprites->enemies.ripper2;
		}
	}
}

void waverAI(struct enemy* this) {
	this->x+=this->vel_x;
	this->vel_y+=this->extra1;
	if(this->vel_y > 9 || this->vel_y < -9 || hitWallY(this->y,this->x,this->x+16,0,'t') || hitWallY(this->y-8,this->x,this->x+16,0,'b')) {
		this->extra1*=-1;
	}
	this->y+=this->extra1;
	int x = this->x;
	if(this->vel_x > 0)
		x+=16;
	if(hitWallX(x,this->y,this->y-8,0,'r'))
		this->vel_x*=-1;
}

void sideHopperAI(struct enemy* this) {
	if(this->extra1 == 0) {
		if(random(3) == 2) {
			this->extra1=1;
			this->vel_y=-5;
			this->vel_x=3;
			if(random(2)==1)
				this->vel_x=-3;
		}
	} else {
		int backupX = this->x;
		int backupY = this->y;
		this->x+=this->vel_x;
		this->y+=this->vel_y;
		if(++this->vel_y > 5)
			this->vel_y = 5;
		if(this->vel_x) {
			int x = this->x;
			if(this->vel_x>0)
				x=this->x+16;
			if(hitWallX(x, this->y, this->y-12, 0, 'l')) {
				this->x = backupX;
				this->vel_x = 0;
			}
		}
		if(hitWallY(this->y-12,this->x,this->x+16,0,'b')) {
			this->y=backupY;
			this->vel_y = 1;
		} else if(hitWallY(this->y, this->x, this->x+16, 0, 't')) {
			int x = this->x + 8;
			if(tiles[map->array[1-map->array[((this->y)/8)*map->w]+(this->y/8)*map->w+x/8]].m==NULL)
				x=this->x+1;
			if(tiles[map->array[1-map->array[((this->y)/8)*map->w]+(this->y/8)*map->w+x/8]].m==NULL)
				x=this->x+15;
			this->y=tiles[map->array[1-map->array[((this->y)/8)*map->w]+(this->y/8)*map->w+x/8]].m[0+x%8] + ((this->y)/8)*8;
			this->extra1=0;
		}
	}
}

void skreeAI(struct enemy* this) {
	if(this->vel_y == 0 && this->vel_x == 0) {
		if(samus->y > this->y && samus->y - this->y < 64) {
			if(this->x - samus->x < 20  && samus->x - this->x < 4) {
				this->sprt = &sprites->enemies.skree2;
				this->vel_y = 8;
				this->vel_x = 2;
				if(samus->x < this->x)
					this->vel_x=-2;
			}
		}
	} else {
		this->y+=this->vel_y;
		this->x+=this->vel_x;
		if(hitWallY(this->y,this->x+2,this->x+6, 0, 't')) {
			this->dead = -1;
			skreeDeath(this);
		}
	}
}

void projectileAI(struct enemy* this) {
	this->x+=this->vel_x;
	this->y+=this->vel_y;
	if(++this->vel_y > 8)
		this->vel_y=8;
}

void drawEnemy(struct enemy* this) {
	void *l = v0;
	if(this->hurt) {
		this->hurt--;
		l = v1;
	}
	if(this->w > 8) {
		GrayClipSprite16_OR_R(this->x-cam->x,this->y-cam->y-this->sprt->h,this->sprt->h,this->sprt->s1,this->sprt->s2,l,v1);
	} else {
		GrayClipSprite8_OR_R(this->x-cam->x,this->y-cam->y-this->sprt->h,this->sprt->h,this->sprt->s1,this->sprt->s2,l,v1);
	}
}


void makeEnemies() {
	struct enemy e[] = {
		{///*** Waver 200 ***/
			0, 0,											// X, Y
			8, 16,										// H, W
			4, 0,											// VX, VY
			1, 0, 0,									// Extras
			30, 10, 48, 24, 24, 3,		// HP, ATK, E, M, SM, PB
			0, 0,											// Dead
			1, 												// Frames
			&sprites->enemies.waver,	// Sprite
			0,												// Immunity Level
			waverAI,									// Behavior - AI
			simpleDeath								// Death Function
		},{/*** Side Hopper 201 ***/
			0, 0,
			12, 16,
			0, 0,
			0, 0, 0,
			320, 80, 16, 22, 22, 2,
			0, 0,
			1,
			&sprites->enemies.sideHopper,
			0,
			sideHopperAI,
			simpleDeath
		},{/*** Skree 202 ***/
			0, 0,
			11, 8,
			0, 0,
			0, 0, 0,
			15, 10, 9, 33, 2, 2,
			0, 0,
			1,
			&sprites->enemies.skree,
			0,
			skreeAI,
			skreeDeath
		},{/*** Skree Projectile 203 ***/
			0, 0,
			4, 4,
			-3, -5,
			0, 0, 0,
			-1, 10, 9, 33, 2, 2,
			0, 0,
			1,
			&sprites->enemies.skreeProjectile,
			-1,
			projectileAI,
			NULL
		},{/*** Zebbo 204 ***/
			0, 0,
			7, 8,
			1, -1,
			16, 0, 0,
			30, 20, 55, 4, 29, 2,
			0, 0,
			1,
			&sprites->enemies.zebboR,
			0,
			zebboAI,
			simpleDeath
		},{/*** Zebbo Spawn 205 ***/
			0, 0,
			8, 8,
			0, 0,
			0, 0, 0,
			-1, 0, 0, 0, 0, 0,
			0, 0,
			1,
			&sprites->enemies.zebboSpawn,
			-1,
			zebboSpawnAI,
			NULL
		},{/*** Ripper 206 ***/
			0, 0,
			8, 16,
			1, 0,
			0, 0, 0,
			200, 5, 39, 31, 2, 2,
			0, 0,
			1,
			&sprites->enemies.ripper,
			2,
			ripperAI,
			simpleDeath
		},{/*** Space Pirate 207 ***/
			0, 0,
			19, 16,
			2, 0,
			1, 12, 9,
			20, 15, 67, 31, 2, 0,
			0, 0,
			6,
			&sprites->enemies.SpacePirateStandRight,
			2,
			spacePirateAI,
			simpleDeath
		},{/*** Space Pirate Laser 208 ***/
			0, 0,
			3, 16,
			3, 0,
			0, 0, 0,
			-1, 20, 0, 0, 0, 0,
			0, 0,
			1,
			&sprites->enemies.SpacePirateLaser,
			-1,
			spacePirateLaserAI,
			NULL
		}
	};
	if((enemyKey = malloc(sizeof(e))) == NULL)
		exit(0);
	memcpy(enemyKey, e, sizeof(e));
	if((enemies = malloc(sizeof(struct enemyList))) == NULL)
		exit(0);
	enemies->first = NULL;
}
void pushEnemy(int x, int y, int n) {
	struct enemy* this = malloc(sizeof(struct enemy));
	memcpy(this, enemyKey + n, sizeof(struct enemy));
	this->x = x;
	this->y = y;
	struct enemyNode* node = malloc(sizeof(struct enemyNode));
	node->value = this;
	node->next = enemies->first;
	enemies->first = node;
}