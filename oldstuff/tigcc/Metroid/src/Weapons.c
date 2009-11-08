// C Source File
// Created 2/5/2005; 5:24:18 PM

#include <tigcclib.h>
#include "all.h"

void basicShot(struct object* this) {
	int moved_x = 0, moved_y = 0;
	while((moved_x < abs(this->vel_x) || moved_y < abs(this->vel_y)) && !this->gone) {
		//pause('a');
		if(++moved_x < abs(this->vel_x)) {
			if(this->vel_x<0)
				this->x--;
			else
				this->x++;
		}
		if(++moved_y < abs(this->vel_y)){
			if(this->vel_y < 0)
				this->y--;
			else
				this->y++;
		}
		if(tiles[map->array[1-map->array[((this->y-2)/8)*map->w]+((this->y-2)/8)*map->w+((this->x+2)/8)]].m!=NULL) {
			onShot(((this->x+2)/8)*8,((this->y-2)/8)*8,this);
			this->gone = 1;
		}
	//	pause('c');
		checkEnemies(this);
	//	pause('b');
	}
}
void missileShot(struct object* this) {
	if(this->vel_x < 0 && this->vel_y < 0)
		this->sprt = &sprites->weapons.missile1;
	else if(this->vel_x > 0 && this->vel_y < 0)
		this->sprt = &sprites->weapons.missile2;
	else if(this->vel_x > 0 && this->vel_y > 0)
		this->sprt = &sprites->weapons.missile3;
	else if(this->vel_x < 0 && this->vel_y > 0)
		this->sprt = &sprites->weapons.missile4;
	else if(this->vel_x < 0 && this->vel_y == 0)
		this->sprt = &sprites->weapons.missile5;
	else if(this->vel_x > 0 && this->vel_y == 0)
		this->sprt = &sprites->weapons.missile6;
	else if(this->vel_x == 0 && this->vel_y < 0)
		this->sprt = &sprites->weapons.missile7;
	else if(this->vel_x == 0 && this->vel_y > 0)
		this->sprt = &sprites->weapons.missile8;
	if(this->vel_x < 0)
		this->vel_x--;
	else if(this->vel_x > 0)
		this->vel_x++;
	if(this->vel_y < 0)
		this->vel_y--;
	else if(this->vel_y > 0)
		this->vel_y++;
	basicShot(this);
}
void bombShot(struct object* this) {
	if(--this->timer < 0) {
		if(boundingBoxCollide(this->x,this->y,4,4,samus->x,samus->y,8,8,1)) {
			if(samus->mode == morphedLeft || samus->mode == fallingBallLeft || samus->mode == rollingLeft || samus->mode == bouncingLeft || samus->mode == spiderLeft || samus->mode == spiderUp) {
				samus->mode = bouncingLeft;
				samus->vel_y = BOMB_BOUNCE;
				samus->vel_x=0;
			} else if(samus->mode == morphedRight || samus->mode == fallingBallRight || samus->mode == rollingRight || samus->mode == bouncingLeft || samus->mode == spiderRight || samus->mode == spiderDown) {
				samus->mode = bouncingRight;
				samus->vel_y = BOMB_BOUNCE;
				samus->vel_x=0;
			}
		}
		int x, y;
		for(x = this->x - 3; x <= this->x + 6; x+=3) {
			for(y = this->y - 7; y <= this->y + 2; y+=3) {
				if((x == this->x - 3 && y == this->y - 7) || (x == this->x - 3 && y == this->y + 2) ||
					 (x == this->x + 6 && y == this->y - 7) || (x == this->x + 6 && y == this->y + 2))
					 continue;
				onShot((x/8)*8,(y/8)*8,this);
			}
		}
		pushObject(this->x - 2, this->y + 2, 0, 0, &templates->effects.bombExplosion);
		checkEnemies(this);
		this->gone = 1;
	}
}

int shoot(int x, int y, int dir) {
	if(samus->weapons.wait)
		return 1;
	if(game->keys.fire && samus->weapons.charge_equipped && samus->weapons.gun != &templates->weapons.superMissile && samus->weapons.gun != &templates->weapons.missile) {
		if(++samus->weapons.charge>CHARGE_MAX)
			samus->weapons.charge = CHARGE_MAX;
		return 0;
	} else {
		int dx = dir;
		int dy = 0;
		if(game->keys.aim) {
			if(game->keys.down)
				dy=1;
			if(game->keys.up)
				dy=-1;
			if((game->keys.up || game->keys.down) && ((dir<0 && !game->keys.left) || (dir>0 && !game->keys.right)))
				dx=0;
		} else {
			if(game->keys.up) {
				dy=-1;
				if((dir<0 && !game->keys.left) || (dir>0 && !game->keys.right))
					dx=0;
			}
			if(game->keys.down) {
				dy=1;
				if((dir<0 && !game->keys.left) || (dir>0 && !game->keys.right))
					dx=0;
			}
		}
		int vel_x, vel_y;
		vel_x = dy==0?(samus->weapons.gun->vel_x+samus->vel_x)*dx:(0.7*samus->weapons.gun->vel_x+samus->vel_x)*dx;
		vel_y = dx==0?(samus->weapons.gun->vel_x+samus->vel_y)*dy:(0.7*samus->weapons.gun->vel_x+samus->vel_y)*dy;
		//pause('a');
		pushObject(x, y, vel_x, vel_y, samus->weapons.gun);
		//pause('b');
		if(samus->weapons.gun == &templates->weapons.missile)
			samus->weapons.missile_ammo--;
		else if(samus->weapons.gun == &templates->weapons.superMissile)
			samus->weapons.superMissile_ammo--;
		//pause('c');
		objects->first->value->ATK+=samus->weapons.charge;
		//pause('d');
		if(samus->weapons.charge == CHARGE_MAX)
			objects->first->value->type++;
		//pause('e');
		samus->weapons.charge = 0;
		samus->weapons.wait = 10;
		//pause('f');
	}
	return 1;
}
void layBomb(void) {
	if(samus->weapons.wait)
		return;
	if(game->keys.fire == 1) {
		game->keys.fire = 2;
		pushObject(samus->x + 6, samus->y - 2, 0, 0, samus->weapons.bomb);
		if(samus->weapons.bomb == &templates->weapons.powerBomb)
			samus->weapons.powerBomb_ammo--;
		samus->weapons.wait=10;
	}

}

void weaponsSetup(void) {
	if(game->keys.f5 == 1) {
		if(samus->weapons.bomb == &templates->weapons.powerBomb)
			samus->weapons.bomb =  &templates->weapons.bomb;
		else if(samus->weapons.gun == samus->weapons.beam) {
			samus->weapons.gun = &templates->weapons.missile;
			if(samus->weapons.missile_ammo == 0) {
				samus->weapons.gun = &templates->weapons.superMissile;
				if(samus->weapons.superMissile_ammo == 0) {
					samus->weapons.gun = samus->weapons.beam;
					samus->weapons.bomb = &templates->weapons.powerBomb;
				}
			}
		} else if(samus->weapons.gun == &templates->weapons.missile) {
			samus->weapons.gun =  &templates->weapons.superMissile;
			if(samus->weapons.superMissile_ammo == 0) {
				samus->weapons.gun = samus->weapons.beam;
				samus->weapons.bomb = &templates->weapons.powerBomb;
			}
		} else if(samus->weapons.gun ==  &templates->weapons.superMissile) {
			samus->weapons.gun = samus->weapons.beam;
			samus->weapons.bomb =  &templates->weapons.powerBomb;
		}
		game->keys.f5 = 2;
	}
	if(samus->weapons.bomb ==  &templates->weapons.powerBomb && samus->weapons.powerBomb_ammo <= 0)
		samus->weapons.bomb =  &templates->weapons.bomb;
	else if(samus->weapons.gun ==  &templates->weapons.missile && samus->weapons.missile_ammo <= 0)
		samus->weapons.gun = samus->weapons.beam;
	else if(samus->weapons.gun ==  &templates->weapons.superMissile && samus->weapons.superMissile_ammo <= 0)
		samus->weapons.gun = samus->weapons.beam;
	if(--samus->weapons.wait<0)
		samus->weapons.wait=0;
}


void checkEnemies(struct object* this) {
	struct enemyNode* curr = enemies->first;
	struct enemyNode* prev = NULL;
	while(curr!=NULL) {
		if(!(curr->value->HP < 0)) {
			if(boundingBoxCollide(this->x, this->y, this->w, this->h, curr->value->x, curr->value->y, curr->value->w, curr->value->h,1)) {
				hurtEnemy(curr->value, this->ATK, this->type);
				this->gone = 1;
			}
		}
		prev = curr;
		curr = curr->next;
	}
}