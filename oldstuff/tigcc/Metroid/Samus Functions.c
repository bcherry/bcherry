// C Source File
// Created 2/2/2005; 5:22:05 PM

#include "all.h"

void makeSamus(void) {
	if((samus = malloc(sizeof(struct samus))) == NULL)
		exit(0);
	struct samus s = {
		16, 88,													// X, Y
		0, 0,														// VX, VY
		0, 0,														// HURT, INVINC
		99,															// HEALTH
		1,															// FRAMES
		0,															// SPIDER WALL
		&sprites->samus.rightFall,			// SPRITE
		fallingRight,										// MODE
		{/***************WEAPONS*****************/
			1, 1,													// CHARGE
			0, 0,													// SPAZER
			0, 0,													// WAVE
			0, 0,													// PLASMA
			0, 0,													// ICE
			1,														// BOMB
			0, 0,													// MISSILE
			0, 0,												// SUPER MISSILE
			0, 0,													// POWERBOMB
			&templates->weapons.powerBeam,// GUN
			&templates->weapons.bomb,			// BOMB
			&templates->weapons.powerBeam,// BEAM
			0,														// CHARGE
			0															// WAIT
		},{/************ITEMS********************/
			8, 8,													// TANKS
			0, 0,													// SPRING BALL
			0, 0													// HIJUMP
		}
	};
	memcpy(samus, &s, sizeof(s));
}

void wallJumpRight(void) {
	if(game->keys.right == 2 && game->keys.jump == 1) {
		game->keys.jump++;
		samus->sprt = &sprites->samus.rightFlip1;
		samus->mode = flippingUpRight;
		samus->vel_x = WALL_JUMP_X_VEL;
		samus->vel_y = WALL_JUMP_Y_VEL;
		return;
	} else {
		samus->sprt = &sprites->samus.rightFall;
		samus->mode = fallingRight;
		return;
	}
}
void wallJumpLeft(void) {
	if(game->keys.left == 2 && game->keys.jump == 1) {
		game->keys.jump++;
		samus->sprt = &sprites->samus.leftFlip1;
		samus->mode = flippingUpLeft;
		samus->vel_x = WALL_JUMP_X_VEL;
		samus->vel_y = WALL_JUMP_Y_VEL;
		return;
	} else {
		samus->sprt = &sprites->samus.leftFall;
		samus->mode = fallingLeft;
		return;
	}
}

void hurtingLeft(void) {
	if(samus->beenHurt>0) {
		if(--samus->beenHurt==0) {
			samus->mode = fallingLeft;
			samus->sprt = &sprites->samus.leftFall;
			samus->vel_x=0;
			samus->vel_y=0;
			samus->invincible=8;
		} else {
			samus->sprt = &sprites->samus.leftHurt;
		}
		if(!hitWallX(SAMUS_RIGHT+2,SAMUS_FEET-2,SAMUS_HEAD-2, 1, 'r'))
			samus->x+=2;
		if(!hitWallY(samus->y-26,SAMUS_LEFT,SAMUS_RIGHT, 1, 'b'))
			samus->y-=2;
	}
}

void hurtingRight(void) {
	if(samus->beenHurt>0) {
		if(--samus->beenHurt==0) {
			samus->mode = fallingRight;
			samus->sprt = &sprites->samus.rightFall;
			samus->vel_x=0;
			samus->vel_y=0;
			samus->invincible=8;
		} else {
			samus->sprt = &sprites->samus.rightHurt;
		}
		if(!hitWallX(SAMUS_LEFT-2,SAMUS_FEET-2,SAMUS_HEAD-2, 1, 'l'))
			samus->x-=2;
		if(!hitWallY(samus->y-26,SAMUS_LEFT,SAMUS_RIGHT, 1, 'b'))
			samus->y-=2;
	}
}

int hitWallX(int x, int y1, int y2, int collide, char dir) {
	int y, y_store = -1, hit = 0;
	for(y = y1; y >= y2; y--) {
		if(tiles[map->array[1-map->array[(y/8)*map->w]+(y/8)*map->w+x/8]].m != NULL) {
			if(x%8 >= tiles[map->array[1-map->array[(y/8)*map->w]+(y/8)*map->w+x/8]].m[24+y%8] &&
				 x%8 <= tiles[map->array[1-map->array[(y/8)*map->w]+(y/8)*map->w+x/8]].m[16+y%8] &&
				 y%8 <= tiles[map->array[1-map->array[(y/8)*map->w]+(y/8)*map->w+x/8]].m[8+x%8] &&
				 y%8 >= tiles[map->array[1-map->array[(y/8)*map->w]+(y/8)*map->w+x/8]].m[0+x%8])
				hit = 1;
				if(!collide)
					return 1;
				if(y/8 != y_store) {
					y_store = y/8;
					int s = onCollide(dir, (x/8)*8, (y/8)*8);
					if(collide == 3)
						hit = s;
				}

		}
	}
	return hit;
}
int hitWallY(int y, int x1, int x2, int collide, char dir) {
	int x, x_store = -1, hit = 0;
	for(x = x1; x <= x2; x++) {
		if(tiles[map->array[1-map->array[(y/8)*map->w]+(y/8)*map->w+x/8]].m != NULL) {
			if(x%8 >= tiles[map->array[1-map->array[(y/8)*map->w]+(y/8)*map->w+x/8]].m[24+y%8] &&
			   x%8 <= tiles[map->array[1-map->array[(y/8)*map->w]+(y/8)*map->w+x/8]].m[16+y%8] &&
			   y%8 <= tiles[map->array[1-map->array[(y/8)*map->w]+(y/8)*map->w+x/8]].m[8+x%8] &&
			   y%8 >= tiles[map->array[1-map->array[(y/8)*map->w]+(y/8)*map->w+x/8]].m[0+x%8]) {
				hit = 1;
				if(!collide)
					return 1;
				if(x/8 != x_store) {
					x_store = x/8;
					onCollide(dir, (x/8)*8, (y/8)*8);
				}
			}
		}
	}
	return hit;
}


void standingLeft(void) {
	samus->vel_x=samus->vel_y=0;
	if(samus->beenHurt>0) {
		samus->sprt = &sprites->samus.leftHurt;
		samus->mode = hurtingLeft;
		return;
	}
	if(!hitWallY(samus->y,SAMUS_LEFT,SAMUS_RIGHT, 1, 't')) {
		samus->mode = fallingLeft;
		samus->sprt = &sprites->samus.leftFall;
		samus->vel_y = 0;
		samus->vel_x = 0;
		return;
	}
	if(game->keys.left == 1) {
		game->keys.left++;
		if(!hitWallX(SAMUS_LEFT,SAMUS_FEET,SAMUS_HEAD, 1, 'l')) {
			samus->mode = runningLeft;
			samus->sprt = &sprites->samus.leftRun1;
			samus->vel_x = RUN_INIT;
			return;
		}
	} else if(game->keys.right) {
		samus->mode = standingRight;
		samus->sprt = &sprites->samus.rightStand;
		return;
	} else if(game->keys.down == 1) {
		game->keys.down++;
		samus->mode = kneelingLeft;
		samus->sprt = &sprites->samus.leftKneel;
		return;
	} else if(game->keys.jump == 1) {
		game->keys.jump++;
		if(!hitWallY(SAMUS_HEAD-1,SAMUS_LEFT,SAMUS_RIGHT, 1, 'b')) {
			samus->mode = jumpingLeft;
			samus->sprt = &sprites->samus.leftJump1;
			samus->vel_x = 0;
			samus->vel_y = JUMP_INIT_Y;
		}
	}
	if(game->keys.fire || samus->weapons.charge) {
		shoot(SAMUS_LEFT, samus->y - 10, -1);
		if(game->keys.fire)
			game->keys.fire = 2;
	}
}
void standingRight(void) {
	samus->vel_x=samus->vel_y=0;
	if(samus->beenHurt>0) {
		samus->sprt = &sprites->samus.rightHurt;
		samus->mode = hurtingRight;
		return;
	}
	if(!hitWallY(samus->y,SAMUS_LEFT,SAMUS_RIGHT, 1, 't')) {
		samus->mode = fallingRight;
		samus->sprt = &sprites->samus.rightFall;
		samus->vel_y = 0;
		samus->vel_x = 0;
		return;
	}
	if(game->keys.right == 1) {
		game->keys.right++;
		if(!hitWallX(SAMUS_RIGHT,SAMUS_FEET,SAMUS_HEAD, 1, 'r')) {
			samus->mode = runningRight;
			samus->sprt = &sprites->samus.rightRun1;
			samus->vel_x = RUN_INIT;
			return;
		}
	} else if(game->keys.left) {
		samus->mode = standingLeft;
		samus->sprt = &sprites->samus.leftStand;
		return;
	} else if(game->keys.down == 1) {
		game->keys.down++;
		samus->mode = kneelingRight;
		samus->sprt = &sprites->samus.rightKneel;
		return;
	} else if(game->keys.jump == 1) {
		game->keys.jump++;
		if(!hitWallY(SAMUS_HEAD-1,SAMUS_LEFT,SAMUS_RIGHT, 1, 'b')) {
			samus->mode = jumpingRight;
			samus->sprt = &sprites->samus.rightJump1;
			samus->vel_x = 0;
			samus->vel_y = JUMP_INIT_Y;
		}
	}
	if(game->keys.fire || samus->weapons.charge) {
		shoot(SAMUS_LEFT + 10, samus->y - 10, 1);
		if(game->keys.fire)
			game->keys.fire = 2;
	}
}

int runningCollision(int dir) {
	int i;
	for(i = 0; i < samus->vel_x; i++) {
		int backupX = samus->x;
		int backupY = samus->y;
		samus->x+=dir;
		groundHug(SAMUS_CRIT, samus->y - 1, &samus->y, 0);
		if(hitWallX(SAMUS_LEFT, SAMUS_FEET, SAMUS_HEAD, 1, 'l') || hitWallX(SAMUS_RIGHT, SAMUS_FEET, SAMUS_HEAD, 1, 'r')) {
			samus->x = backupX;
			samus->y = backupY;
			if(dir<0) {
				samus->mode = standingLeft;
				samus->sprt = &sprites->samus.leftStand;
			} else {
				samus->mode = standingRight;
				samus->sprt = &sprites->samus.rightStand;
			}
			return 0;
		}
		if(!hitWallY(samus->y,SAMUS_LEFT,SAMUS_RIGHT, 1, 't')) {
			if(dir<0) {
				samus->mode = fallingLeft;
				samus->sprt = &sprites->samus.leftFall;
			} else {
				samus->mode = fallingRight;
				samus->sprt = &sprites->samus.rightFall;
			}
			samus->vel_y = 0;
			samus->vel_x/=FALLING_X_VEL_DIVISOR;
			return 0;
		}
	}

	return 1;
}

void runningLeft(void) {
	samus->frames=RUN_FRAMES;
	if(samus->beenHurt>0) {
		samus->sprt = &sprites->samus.leftHurt;
		samus->mode = hurtingLeft;
		return;
	}
	if(game->keys.right == 1) {
		samus->mode = standingRight;
		samus->sprt = &sprites->samus.rightStand;
		return;
	} else if(game->keys.down == 1) {
		game->keys.down++;
		samus->mode = kneelingLeft;
		samus->sprt = &sprites->samus.leftKneel;
		return;
	} else if(!game->keys.left) {
		if((samus->vel_x-=4) <= 0) {
			samus->mode = standingLeft;
			samus->sprt = &sprites->samus.leftStand;
			return;
		}
	} else if(game->keys.jump == 1) {
		game->keys.jump++;
		if(samus->vel_x > FLIP_CUTOFF) {
			samus->mode = flippingUpLeft;
			samus->sprt = &sprites->samus.leftFlip1;
		} else {
			samus->mode = jumpingLeft;
			samus->sprt = &sprites->samus.leftJump1;
		}
		samus->vel_y = JUMP_INIT_Y;
		return;
	}

	if(runningCollision(-1) == 0)
		return;

	if(game->keys.fire || samus->weapons.charge) {
		shoot(SAMUS_LEFT, samus->y - 10, -1);
		if(game->keys.fire)
			game->keys.fire = 2;
	}
	if(game->keys.left) {
		game->keys.left = 2;
		samus->vel_x++;
		int max = RUN_MAX;
		if(_keytest(SPRINT))
			max = SPRINT_MAX;
		if(++samus->vel_x > max)
			samus->vel_x = max;
	}
}
void runningRight(void) {
	samus->frames=RUN_FRAMES;
	if(samus->beenHurt>0) {
		samus->sprt = &sprites->samus.rightHurt;
		samus->mode = hurtingRight;
		return;
	}
	if(game->keys.left == 1) {
		samus->mode = standingLeft;
		samus->sprt = &sprites->samus.leftStand;
		return;
	} else if(game->keys.down == 1) {
		game->keys.down++;
		samus->mode = kneelingRight;
		samus->sprt = &sprites->samus.rightKneel;
		return;
	} else if(!game->keys.right) {
		if((samus->vel_x-=4) <= 0) {
			samus->mode = standingRight;
			samus->sprt = &sprites->samus.rightStand;
			return;
		}
	} else if(game->keys.jump == 1) {
		game->keys.jump++;
		if(samus->vel_x > FLIP_CUTOFF) {
			samus->mode = flippingUpRight;
			samus->sprt = &sprites->samus.rightFlip1;
		} else {
			samus->mode = jumpingRight;
			samus->sprt = &sprites->samus.rightJump1;
		}
		samus->vel_y = JUMP_INIT_Y;
		return;
	}
	if(runningCollision(1) == 0)
		return;

	if(game->keys.fire || samus->weapons.charge) {
		shoot(SAMUS_RIGHT, samus->y - 10, 1);
		if(game->keys.fire)
			game->keys.fire = 2;
	}
	if(game->keys.right) {
		game->keys.right = 2;
		samus->vel_x++;
		int max = RUN_MAX;
		if(_keytest(SPRINT))
			max = SPRINT_MAX;
		if(++samus->vel_x > max)
			samus->vel_x = max;
	}
}
void kneelingLeft(void) {
	samus->vel_x=samus->vel_y=0;
	if(samus->beenHurt>0) {
		samus->sprt = &sprites->samus.leftHurt;
		samus->mode = hurtingLeft;
		return;
	}
	if(!hitWallY(samus->y,SAMUS_LEFT,SAMUS_RIGHT, 1, 't')) {
		samus->mode = fallingLeft;
		samus->sprt = &sprites->samus.leftFall;
		samus->vel_y = 0;
		samus->vel_x = 0;
		return;
	}
	if(game->keys.left == 1) {
		game->keys.left = 2;
		if(!hitWallY(SAMUS_HEAD,SAMUS_LEFT,SAMUS_RIGHT, 1, 'b') && !hitWallX(SAMUS_LEFT,SAMUS_FEET,SAMUS_HEAD, 1, 'l')) {
			samus->mode = runningLeft;
			samus->sprt = &sprites->samus.leftRun1;
			samus->vel_x = RUN_INIT;
			return;
		}
	} else if(game->keys.right) {
		samus->mode = kneelingRight;
		samus->sprt = &sprites->samus.rightKneel;
		return;
	} else if(game->keys.down == 1) {
		game->keys.down = 2;
		samus->mode = morphedLeft;
		samus->sprt = &sprites->samus.leftMorph;
		return;
	} else if(game->keys.up == 1) {
		game->keys.up = 2;
		if(!hitWallY(SAMUS_HEAD,SAMUS_LEFT,SAMUS_RIGHT, 1, 'b')) {
			samus->mode = standingLeft;
			samus->sprt = &sprites->samus.leftStand;
			return;
		}
	}
	if(game->keys.fire || samus->weapons.charge) {
		shoot(SAMUS_LEFT, samus->y - 5, -1);
		if(game->keys.fire)
			game->keys.fire = 2;
	}
}
void kneelingRight(void) {
	samus->vel_x=samus->vel_y=0;
	if(samus->beenHurt>0) {
		samus->sprt = &sprites->samus.rightHurt;
		samus->mode = hurtingRight;
		return;
	}
	if(!hitWallY(samus->y,SAMUS_LEFT,SAMUS_RIGHT, 1, 't')) {
		samus->mode = fallingRight;
		samus->sprt = &sprites->samus.rightFall;
		samus->vel_y = 0;
		samus->vel_x = 0;
		return;
	}
	if(game->keys.right == 1) {
		game->keys.right = 2;
		if(!hitWallY(SAMUS_HEAD,SAMUS_LEFT,SAMUS_RIGHT, 1, 'b') && !hitWallX(SAMUS_RIGHT,SAMUS_FEET,SAMUS_HEAD, 1, 'r')) {
			samus->mode = runningRight;
			samus->sprt = &sprites->samus.rightRun1;
			samus->vel_x = RUN_INIT;
			return;
		}
	} else if(game->keys.left) {
		samus->mode = kneelingLeft;
		samus->sprt = &sprites->samus.leftKneel;
		return;
	} else if(game->keys.down == 1) {
		game->keys.down = 2;
		samus->mode = morphedRight;
		samus->sprt = &sprites->samus.rightMorph;
		return;
	} else if(game->keys.up == 1) {
		game->keys.up = 2;
		if(!hitWallY(SAMUS_HEAD,SAMUS_LEFT,SAMUS_RIGHT, 1, 'b')) {
			samus->mode = standingRight;
			samus->sprt = &sprites->samus.rightStand;
			return;
		}
	}
	if(game->keys.fire || samus->weapons.charge) {
		shoot(SAMUS_RIGHT, samus->y - 5, 1);
		if(game->keys.fire)
			game->keys.fire = 2;
	}
}

int jumpingCollision(int dir) {
	int moved_y = 0, moved_x = 0;
	while(moved_y < samus->vel_y/3 || moved_x < samus->vel_x) {
		int backupX = samus->x;
		int backupY = samus->y;
		if(++moved_y <= samus->vel_y/3)
			samus->y--;
		if(++moved_x <= samus->vel_x)
			samus->x+=dir;
		if(hitWallX(SAMUS_LEFT, SAMUS_FEET, SAMUS_HEAD, 1, 'l') || hitWallX(SAMUS_RIGHT, SAMUS_FEET, SAMUS_HEAD, 1, 'r')) {
			samus->x = backupX;
			samus->vel_x = 0;
		}
		if(hitWallY(SAMUS_HEAD,SAMUS_LEFT,SAMUS_RIGHT, 1, 'b')) {
			samus->y = backupY;
			samus->vel_x/=FALLING_X_VEL_DIVISOR;;
			samus->vel_y = 5;
			if(dir<0) {
				samus->mode = fallingLeft;
				samus->sprt = &sprites->samus.leftFall;
			} else {
				samus->mode = fallingRight;
				samus->sprt = &sprites->samus.rightFall;
			}
			return 0;
		}
	}
	return 1;
}

void jumpingLeft(void) {
	samus->frames=2;
	if(samus->beenHurt>0) {
		samus->sprt = &sprites->samus.leftHurt;
		samus->mode = hurtingLeft;
		return;
	}
	if(game->keys.left) {
		game->keys.left = 2;
		samus->vel_x = IN_AIR_X_VEL;
	} else
		samus->vel_x = 0;

	if(game->keys.fire || samus->weapons.charge) {
		shoot(SAMUS_LEFT, samus->y - 10, -1);
		if(game->keys.fire)
			game->keys.fire = 2;
	}

	if(jumpingCollision(-1) == 0)
		return;

	samus->vel_y-=JUMPING_GRAVITY;

	if(game->keys.down == 1) {
		game->keys.down = 2;
		samus->mode=bouncingLeft;
		samus->sprt = &sprites->samus.ball1;
		samus->vel_y/=3;
		return;
	}
	if(game->keys.right) {
		game->keys.right = 2;
		samus->vel_x = 0;
		samus->mode = jumpingRight;
		samus->sprt = &sprites->samus.rightJump2;
		return;
	}
	if(!(game->keys.jump == 2) || samus->vel_y <= 5) {
		samus->vel_y=5;
		samus->mode=fallingLeft;
		samus->sprt = &sprites->samus.leftFall;
		return;
	}
}
void jumpingRight(void) {
	samus->frames=2;
	if(samus->beenHurt>0) {
		samus->sprt = &sprites->samus.rightHurt;
		samus->mode = hurtingRight;
		return;
	}
	if(game->keys.right) {
		game->keys.right = 2;
		samus->vel_x = IN_AIR_X_VEL;
	} else
		samus->vel_x = 0;

	if(game->keys.fire || samus->weapons.charge) {
		shoot(SAMUS_RIGHT, samus->y - 10, 1);
		if(game->keys.fire)
			game->keys.fire = 2;
	}

	if(jumpingCollision(1) == 0)
		return;

	samus->vel_y-=JUMPING_GRAVITY;
	if(game->keys.down == 1) {
		game->keys.down = 2;
		samus->mode=bouncingRight;
		samus->sprt = &sprites->samus.ball1;
		samus->vel_y/=3;
		return;
	}
	if(game->keys.left) {
		game->keys.left = 2;
		samus->vel_x = 0;
		samus->mode = jumpingLeft;
		samus->sprt = &sprites->samus.leftJump2;
		return;
	}
	if(!(game->keys.jump == 2) || samus->vel_y <= 5) {
		samus->vel_y = 5;
		samus->mode = fallingRight;
		samus->sprt = &sprites->samus.rightFall;
		return;
  }
}

int fallingCollision(int dir) {
	int moved_y = 0, moved_x = 0;
	while(moved_y < samus->vel_y/3 || moved_x < samus->vel_x) {
		int backupX = samus->x;
		int backupY = samus->y;
		if(++moved_y <= samus->vel_y/3)
			samus->y++;
		if(++moved_x <= samus->vel_x)
			samus->x+=dir;
		int bottom = SAMUS_FEET;
		if(!hitWallY(samus->y-1, SAMUS_CRIT, SAMUS_CRIT, 1, 't'))
			bottom = samus->y-1;
		if(hitWallX(SAMUS_LEFT, bottom, SAMUS_HEAD, 1, 'l') || hitWallX(SAMUS_RIGHT, bottom, SAMUS_HEAD, 1, 'r')) {
			samus->x = backupX;
			samus->vel_x = 0;
		}
		if(hitWallY(samus->y-1,SAMUS_LEFT,SAMUS_RIGHT, 1, 't')) {
			samus->y = backupY;
			samus->vel_x = 0;
			samus->vel_y = 0;
			if(dir<0) {
				if(game->keys.left) {
					game->keys.left = 2;
					samus->mode = runningLeft;
					samus->sprt = &sprites->samus.leftRun1;
				} else {
					samus->mode = standingLeft;
					samus->sprt = &sprites->samus.leftLand;
				}
			} else {
				if(game->keys.right) {
					game->keys.right = 2;
					samus->mode = runningRight;
					samus->sprt = &sprites->samus.rightRun1;
				} else {
					samus->mode = standingRight;
					samus->sprt = &sprites->samus.rightLand;
				}
			}
			return 0;
		}
	}
	return 1;
}

void fallingLeft(void) {
	samus->frames=2;
	if(samus->beenHurt>0) {
		samus->sprt = &sprites->samus.leftHurt;
		samus->mode = hurtingLeft;
		return;
	}
	if(game->keys.jump)
		game->keys.jump = 2;
	if(game->keys.left) {
		game->keys.left = 2;
		samus->vel_x=IN_AIR_X_VEL;
	} else
		samus->vel_x = 0;

	if(game->keys.fire || samus->weapons.charge) {
		shoot(SAMUS_LEFT, samus->y - 7, -1);
		if(game->keys.fire)
			game->keys.fire = 2;
	}

	if(fallingCollision(-1) == 0)
		return;

	samus->vel_y+=FALLING_GRAVITY;
	if(samus->vel_y > TERMINAL_VELOCITY)
		samus->vel_y = TERMINAL_VELOCITY;

	if(game->keys.down == 1) {
		game->keys.down = 2;
		samus->mode=fallingBallLeft;
		samus->sprt = &sprites->samus.ball1;
		samus->vel_y/=3;
		return;
	}
	if(game->keys.right) {
		game->keys.right = 2;
		samus->vel_x = 0;
		samus->mode = fallingRight;
		samus->sprt = &sprites->samus.rightFall;
		return;
	}
}
void fallingRight(void) {
	samus->frames=2;
	if(samus->beenHurt>0) {
		samus->sprt = &sprites->samus.rightHurt;
		samus->mode = hurtingRight;
		return;
	}
	if(game->keys.jump)
		game->keys.jump = 2;
	if(game->keys.right) {
		game->keys.right = 2;
		samus->vel_x=IN_AIR_X_VEL;
	} else
		samus->vel_x = 0;

	if(game->keys.fire || samus->weapons.charge) {
		shoot(SAMUS_RIGHT, samus->y - 7, 1);
		if(game->keys.fire)
			game->keys.fire = 2;
	}

	if(fallingCollision(1) == 0)
		return;

	samus->vel_y+=FALLING_GRAVITY;
	if(samus->vel_y > TERMINAL_VELOCITY)
		samus->vel_y = TERMINAL_VELOCITY;

	if(game->keys.down == 1) {
		game->keys.down = 2;
		samus->mode=fallingBallRight;
		samus->sprt = &sprites->samus.ball1;
		samus->vel_y/=3;
		return;
	}
	if(game->keys.left) {
		game->keys.left = 2;
		samus->vel_x = 0;
		samus->mode = fallingLeft;
		samus->sprt = &sprites->samus.leftFall;
		return;
	}
}

int flippingUpCollision(int dir) {
	int moved_y = 0, moved_x = 0, r = 1;
	while(moved_y < samus->vel_y/3 || moved_x < samus->vel_x) {
		int backupX = samus->x;
		int backupY = samus->y;
		if(++moved_y <= samus->vel_y/3)
			samus->y--;
		if(++moved_x <= samus->vel_x)
			samus->x+=dir;
		if(hitWallY(samus->y-14,SAMUS_LEFT,SAMUS_RIGHT, 1, 'b')) {
			samus->y = backupY;
			samus->vel_y = 5;
			if(dir>0)
				samus->mode = flippingDownRight;
			else
				samus->mode = flippingDownLeft;
			r = 0;
		}
		if(hitWallX(SAMUS_LEFT,SAMUS_FEET,FLIP_TOP, 1, 'l') || hitWallX(SAMUS_RIGHT, SAMUS_FEET, FLIP_TOP, 1, 'r')) {
			samus->x = backupX;
			samus->vel_x = 0;
			if((dir<0 && _keytest(RR_RIGHT)) || (dir>0 &&  _keytest(RR_LEFT)))
				r = 2;
		}
	}
	return r;
}
void flippingUpLeft(void) {
	samus->frames=FLIP_FRAMES;
	if(samus->beenHurt>0) {
		samus->sprt = &sprites->samus.leftHurt;
		samus->mode = hurtingLeft;
		return;
	}

	if(game->keys.fire || samus->weapons.charge) {
		if(shoot(SAMUS_LEFT, samus->y - 10, -1) == 1) {
			samus->vel_x = 0;
			samus->vel_y = 5;
			samus->mode = fallingLeft;
			samus->sprt = &sprites->samus.leftFall;
		}
		if(game->keys.fire)
			game->keys.fire = 2;
	}

	int collide = flippingUpCollision(-1);
	if(collide == 0)
		return;
	else if(collide == 2) {
		samus->mode = wallJumpRight;
		samus->sprt = &sprites->samus.rightWallJump;
		return;
	}
	samus->vel_y-=JUMPING_GRAVITY;
	if(!(game->keys.jump == 2))
		samus->vel_y-=JUMPING_GRAVITY;
	if(samus->vel_y <= 5) {
		samus->vel_y = 5;
		samus->mode = flippingDownLeft;
		return;
	}
	if(game->keys.right) {
		game->keys.right = 2;
		samus->mode = flippingUpRight;
		samus->sprt = &sprites->samus.rightFlip1;
		return;
	}
	if(game->keys.up) {
		game->keys.up = 2;
		samus->vel_x = 0;
		samus->mode = jumpingLeft;
		samus->sprt = &sprites->samus.leftJump2;
		return;
	}
	if(game->keys.down) {
		game->keys.down = 2;
		samus->vel_x = 0;
		samus->vel_y = 5;
		samus->mode = fallingLeft;
		samus->sprt = &sprites->samus.leftFall;
	}
}
void flippingUpRight(void) {
	samus->frames=FLIP_FRAMES;
	if(samus->beenHurt>0) {
		samus->sprt = &sprites->samus.rightHurt;
		samus->mode = hurtingRight;
		return;
	}

	if(game->keys.fire || samus->weapons.charge) {
		if(shoot(SAMUS_RIGHT, samus->y - 7, 1) == 1) {
			samus->vel_x = 0;
			samus->vel_y = 5;
			samus->mode = fallingRight;
			samus->sprt = &sprites->samus.rightFall;
		}
		if(game->keys.fire)
			game->keys.fire = 2;
	}

	int collide = flippingUpCollision(1);
	if(collide == 0)
		return;
	else if(collide == 2) {
		samus->mode = wallJumpLeft;
		samus->sprt = &sprites->samus.leftWallJump;
		return;
	}
	samus->vel_y-=JUMPING_GRAVITY;
	if(!(game->keys.jump == 2))
		samus->vel_y-=JUMPING_GRAVITY;
	if(samus->vel_y <= 0) {
		samus->vel_y = 0;
		samus->mode = flippingDownRight;
		return;
	}
	if(game->keys.left) {
		game->keys.left = 2;
		samus->mode = flippingUpLeft;
		samus->sprt = &sprites->samus.leftFlip1;
		return;
	}
	if(game->keys.up) {
		game->keys.up = 2;
		samus->vel_x = 0;
		samus->mode = jumpingRight;
		samus->sprt = &sprites->samus.rightJump2;
		return;
	}
	if(game->keys.down == 1) {
		game->keys.down = 2;
		samus->vel_x = 0;
		samus->vel_y = 0;
		samus->mode = fallingRight;
		samus->sprt = &sprites->samus.rightFall;
	}
}

int flippingDownCollision(int dir) {
	int moved_y = 0, moved_x = 0, r = 1;
	while(moved_y < samus->vel_y/3 || moved_x < samus->vel_x) {
		int backupX = samus->x;
		int backupY = samus->y;
		if(++moved_y <= samus->vel_y/3)
			samus->y++;
		if(++moved_x <= samus->vel_x)
			samus->x+=dir;
		int bottom = SAMUS_FEET;
		if(!hitWallY(samus->y-1, SAMUS_CRIT, SAMUS_CRIT, 1, 't'))
			bottom = samus->y-1;
		if(hitWallX(SAMUS_LEFT, bottom, FLIP_TOP, 1, 'l') || hitWallX(SAMUS_RIGHT, bottom, FLIP_TOP, 1, 'r')) {
			samus->x = backupX;
			samus->vel_x = 0;
			if((dir<0 && _keytest(RR_RIGHT)) || (dir>0 &&  _keytest(RR_LEFT)))
				r = 2;
		}
		if(hitWallY(samus->y-1, SAMUS_LEFT, SAMUS_RIGHT, 1, 't')) {
			samus->y = backupY;
			samus->vel_x = 0;
			samus->vel_y = 0;
			if(hitWallY(SAMUS_HEAD, SAMUS_LEFT, SAMUS_RIGHT, 1, 'b')) {
				if(dir<0) {
					samus->mode = kneelingLeft;
					samus->sprt = &sprites->samus.leftKneel;
				} else {
					samus->mode = kneelingRight;
					samus->sprt = &sprites->samus.rightKneel;
				}
			} else {
				if(dir<0) {
					if(game->keys.left) {
						game->keys.left = 2;
						samus->mode = runningLeft;
						samus->sprt = &sprites->samus.leftRun1;
					} else {
						samus->mode = standingLeft;
						samus->sprt = &sprites->samus.leftLand;
					}
				} else {
					if(game->keys.right) {
						game->keys.right = 2;
						samus->mode = runningRight;
						samus->sprt = &sprites->samus.rightRun1;
					} else {
						samus->mode = standingRight;
						samus->sprt = &sprites->samus.rightLand;
					}
				}
			}
			r = 0;
		}
	}
	return r;
}

void flippingDownLeft(void) {
	samus->frames=FLIP_FRAMES;
	if(samus->beenHurt>0) {
		samus->sprt = &sprites->samus.leftHurt;
		samus->mode = hurtingLeft;
		return;
	}

	if(game->keys.fire || samus->weapons.charge) {
		if(shoot(SAMUS_LEFT, samus->y - 10, -1) == 1) {
			samus->vel_x = 0;
			samus->vel_y = 5;
			samus->mode = fallingLeft;
			samus->sprt = &sprites->samus.leftFall;
		}
		if(game->keys.fire)
			game->keys.fire = 2;
	}

	int collide = flippingDownCollision(-1);
	if(collide == 0)
		return;
	else if(collide == 2) {
		samus->mode = wallJumpRight;
		samus->sprt = &sprites->samus.rightWallJump;
		return;
	}
	samus->vel_y+=FALLING_GRAVITY;
	if(samus->vel_y > TERMINAL_VELOCITY)
		samus->vel_y = TERMINAL_VELOCITY;

	if(game->keys.right) {
		game->keys.right = 2;
		samus->mode = flippingDownRight;
		samus->sprt = &sprites->samus.rightFlip1;
		return;
	}
	if(game->keys.up) {
		game->keys.up = 2;
		samus->vel_x = 0;
		samus->mode = fallingLeft;
		samus->sprt = &sprites->samus.leftFall;
		return;
	}
	if(game->keys.down == 1) {
		game->keys.down = 2;
		samus->vel_x = 0;
		samus->mode = fallingLeft;
		samus->sprt = &sprites->samus.leftFall;
	}
}
void flippingDownRight(void) {
	samus->frames=FLIP_FRAMES;
	if(samus->beenHurt>0) {
		samus->sprt = &sprites->samus.rightHurt;
		samus->mode = hurtingRight;
		return;
	}

	if(game->keys.fire || samus->weapons.charge) {
		if(shoot(SAMUS_RIGHT, samus->y - 7, 1) == 1) {
			samus->vel_x = 0;
			samus->vel_y = 5;
			samus->mode = fallingRight;
			samus->sprt = &sprites->samus.rightFall;
		}
		if(game->keys.fire)
			if(game->keys.fire)
			game->keys.fire = 2;
	}

	int collide = flippingDownCollision(1);
	if(collide == 0)
		return;
	else if(collide == 2) {
		samus->mode = wallJumpLeft;
		samus->sprt = &sprites->samus.leftWallJump;
		return;
	}

	samus->vel_y+=FALLING_GRAVITY;
	if(samus->vel_y > TERMINAL_VELOCITY)
		samus->vel_y = TERMINAL_VELOCITY;


	if(game->keys.left) {
		game->keys.left = 2;
		samus->mode = flippingDownLeft;
		samus->sprt = &sprites->samus.leftFlip1;
		return;
	}
	if(game->keys.up) {
		game->keys.up = 2;
		samus->vel_x = 0;
		samus->mode = fallingRight;
		samus->sprt = &sprites->samus.rightFall;
		return;
	}
	if(game->keys.down == 1) {
		game->keys.down = 2;
		samus->vel_x = 0;
		samus->mode = fallingRight;
		samus->sprt = &sprites->samus.rightFall;
	}
}
void morphedLeft(void) {
	if(_keytest(RR_SHIFT)) {
		samus->mode = spiderLeft;
		samus->spiderWall = 0;
		spiderLeft();
		return;
	}
	if(game->keys.up == 1) {
		game->keys.up = 2;
		if(!hitWallY(KNEEL_HEAD, SAMUS_LEFT,SAMUS_RIGHT, 1, 'b')) {
			samus->mode = kneelingLeft;
			samus->sprt = &sprites->samus.leftKneel;
			return;
		}
	}
	if(!hitWallY(samus->y,SAMUS_LEFT,SAMUS_RIGHT, 1, 't')) {
		samus->mode = fallingBallLeft;
		samus->vel_y = 0;
		samus->vel_x = 0;
		return;
	}
	morphed();
}
void morphedRight(void) {
	if(_keytest(RR_SHIFT)) {
		samus->mode = spiderRight;
		samus->spiderWall = 0;
		spiderRight();
		return;
	}
	if(game->keys.up == 1) {
		game->keys.up = 2;
		if(!hitWallY(KNEEL_HEAD, SAMUS_LEFT,SAMUS_RIGHT, 1, 'b')) {
			samus->mode = kneelingRight;
			samus->sprt = &sprites->samus.rightKneel;
			return;
		}
	}
	if(!hitWallY(samus->y,SAMUS_LEFT,SAMUS_RIGHT, 1, 't')) {
		samus->mode = fallingBallLeft;
		samus->vel_y = 0;
		samus->vel_x = 0;
		return;
	}
	morphed();
}
void morphed(void) {
	samus->frames=BALL_FRAMES;
	if(samus->beenHurt>0) {
		if(--samus->beenHurt==0) {
			;
		} else {
			;
		}
	}
	if(game->keys.left == 1) {
		game->keys.left = 2;
		samus->mode = rollingLeft;
		samus->vel_x = ROLL_INIT;
		return;
	} else if(game->keys.right == 1) {
		game->keys.right = 2;
		samus->mode = rollingRight;
		samus->vel_x = ROLL_INIT;
		return;
	}
	layBomb();
}

int rollingCollision(int dir) {
	int i;
	for(i = 0; i < samus->vel_x; i++) {
		int backupX = samus->x;
		int backupY = samus->y;
		samus->x+=dir;
		groundHug(SAMUS_CRIT, samus->y - 1, &samus->y,0);
		if(hitWallX(SAMUS_LEFT, SAMUS_FEET, MORPH_TOP, 1, 'l') || hitWallX(SAMUS_RIGHT,SAMUS_FEET,MORPH_TOP, 1, 'r')) {
			samus->x = backupX;
			samus->y = backupY;
			if(dir<0) {
				samus->mode = morphedLeft;
			} else {
				samus->mode = morphedRight;
			}
			return 0;
		}
		if(!hitWallY(samus->y,SAMUS_LEFT,SAMUS_RIGHT, 1, 't')) {
			if(dir<0) {
				samus->mode = fallingBallLeft;
			} else {
				samus->mode = fallingBallRight;
			}
			samus->vel_y = 0;
			samus->vel_x/=FALLING_X_VEL_DIVISOR;
			return 0;
		}
	}
	return 1;
}

void rollingLeft(void) {
	samus->frames=BALL_FRAMES;
	if(samus->beenHurt>0) {
		if(--samus->beenHurt==0) {
			samus->mode=morphedLeft;
		} else {
			samus->vel_x=0;
		}
	}
	if(game->keys.right == 1) {
		game->keys.right = 2;
		samus->mode = rollingRight;
		samus->vel_x = ROLL_INIT;
		return;
	} else if(game->keys.up == 1) {
		game->keys.up = 2;
		if(!hitWallY(KNEEL_HEAD, SAMUS_LEFT,SAMUS_RIGHT, 1, 'b')) {
			samus->mode = kneelingLeft;
			samus->sprt = &sprites->samus.leftKneel;
			return;
		}
	} else if(!(game->keys.left)) {
		if((samus->vel_x-=2) <= 0) {
			samus->mode = morphedLeft;
			return;
		}
	}

	if(rollingCollision(-1) == 0)
		return;

	layBomb();
	if(game->keys.left) {
		game->keys.left = 2;
		if(++samus->vel_x > ROLL_MAX)
			samus->vel_x = ROLL_MAX;
	}
}
void rollingRight(void) {
	samus->frames=BALL_FRAMES;
	if(samus->beenHurt>0) {
		if(--samus->beenHurt==0) {
			samus->mode=morphedRight;
		} else {
			samus->vel_x=0;
		}
	}
	if(game->keys.left == 1) {
		game->keys.left = 2;
		samus->mode = rollingLeft;
		samus->vel_x = ROLL_INIT;
		return;
	} else if(game->keys.up == 1) {
		game->keys.up = 2;
		if(!hitWallY(KNEEL_HEAD, SAMUS_LEFT,SAMUS_RIGHT, 1, 'b')) {
			samus->mode = kneelingRight;
			samus->sprt = &sprites->samus.rightKneel;
			return;
		}
	} else if(!(game->keys.right)) {
		if((samus->vel_x-=2) <= 0) {
			samus->mode = morphedRight;
			return;
		}
	}

	if(rollingCollision(1) == 0)
		return;

	layBomb();
	if(game->keys.right) {
		game->keys.right = 2;
		if(++samus->vel_x > ROLL_MAX)
			samus->vel_x = ROLL_MAX;
	}
}

int fallingBallCollision(int dir) {
	int moved_y = 0, moved_x = 0;
	while(moved_y < samus->vel_y/3 || moved_x < samus->vel_x) {
		int backupX = samus->x;
		int backupY = samus->y;
		if(++moved_y <= samus->vel_y/3)
			samus->y++;
		if(++moved_x <= samus->vel_x)
			samus->x+=dir;
		int bottom = SAMUS_FEET;
		if(!hitWallY(samus->y-1, SAMUS_CRIT, SAMUS_CRIT, 1, 't'))
			bottom = samus->y-1;
		if(hitWallX(SAMUS_LEFT,bottom,MORPH_TOP, 1, 'l') || hitWallX(SAMUS_RIGHT,bottom,MORPH_TOP, 1, 'r')) {
			samus->x = backupX;
			samus->vel_x = 0;
		}
		if(hitWallY(samus->y-1,SAMUS_LEFT,SAMUS_RIGHT, 1, 't')) {
			samus->y = backupY;
			samus->vel_x /= LANDING_X_VEL_DIVISOR;
			samus->vel_y = 0;
			if(dir<0)
				samus->mode = rollingLeft;
			else
				samus->mode = rollingRight;
			return 0;
		}
	}
	return 1;
}

void fallingBallLeft(void) {
	samus->frames=BALL_FRAMES;
	if(samus->beenHurt>0) {
		if(--samus->beenHurt==0) {
			;
		} else {
			samus->vel_x=0;
		}
	}
	if(game->keys.left) {
		game->keys.left = 2;
		samus->vel_x = IN_AIR_X_VEL;
	} else
		samus->vel_x = 0;
	layBomb();
	if(fallingBallCollision(-1) == 0)
		return;
	samus->vel_y+=FALLING_GRAVITY;
	if(samus->vel_y > TERMINAL_VELOCITY)
		samus->vel_y = TERMINAL_VELOCITY;
	if(game->keys.up == 1) {
		game->keys.up = 2;
		if(!hitWallY(KNEEL_HEAD, SAMUS_LEFT,SAMUS_RIGHT, 1, 'b')) {
			samus->mode=fallingLeft;
			samus->sprt = &sprites->samus.leftFall;
			samus->vel_y/=3;
			return;
		}
	}
	if(game->keys.right == 1) {
		game->keys.right = 2;
		samus->vel_x = 0;
		samus->mode = fallingBallRight;
		return;
	}
}
void fallingBallRight(void) {
	samus->frames=BALL_FRAMES;
	if(samus->beenHurt>0) {
		if(--samus->beenHurt==0) {
			;
		} else {
			samus->vel_x=0;
		}
	}
	if(game->keys.right) {
		game->keys.right = 2;
		samus->vel_x = IN_AIR_X_VEL;
	} else
		samus->vel_x = 0;
  layBomb();
	if(fallingBallCollision(1) == 0)
		return;
	samus->vel_y+=FALLING_GRAVITY;
	if(samus->vel_y > TERMINAL_VELOCITY)
		samus->vel_y = TERMINAL_VELOCITY;
	if(game->keys.up == 1) {
		game->keys.up = 2;
		if(!hitWallY(KNEEL_HEAD, SAMUS_LEFT,SAMUS_RIGHT, 1, 'b')) {
			samus->mode=fallingRight;
			samus->sprt = &sprites->samus.rightFall;
			samus->vel_y/=3;
			return;
		}
	}
	if(game->keys.left == 1) {
		game->keys.left = 2;
		samus->vel_x = 0;
		samus->mode = fallingBallLeft;
		return;
	}
}

int bouncingCollision(int dir) {
	int moved_y = 0, moved_x = 0;
	while(moved_y < samus->vel_y/3 || moved_x < samus->vel_x) {
		int backupX = samus->x;
		int backupY = samus->y;
		if(++moved_y <= samus->vel_y/3)
			samus->y--;
		if(++moved_x <= samus->vel_x)
			samus->x+=dir;
		if(hitWallX(SAMUS_LEFT,SAMUS_FEET,MORPH_TOP, 1, 'l') || hitWallX(SAMUS_RIGHT,SAMUS_FEET,MORPH_TOP, 1, 'r')) {
			samus->x = backupX;
			samus->vel_x = 0;
		}
		if(hitWallY(MORPH_TOP,SAMUS_LEFT,SAMUS_RIGHT, 1, 'b')) {
			samus->y = backupY;
			samus->vel_x/=FALLING_X_VEL_DIVISOR;;
			samus->vel_y = 0;
			if(dir<0)
				samus->mode = fallingBallLeft;
			else
				samus->mode = fallingBallRight;
			return 0;
		}
	}
	return 1;
}

void bouncingLeft(void) {
	samus->frames=BALL_FRAMES;
	if(samus->beenHurt>0) {
		if(--samus->beenHurt==0) {
			samus->mode=fallingBallLeft;
		} else {
			samus->vel_x=0;
		}
	}
	if(game->keys.left) {
		game->keys.left = 2;
		samus->vel_x = IN_AIR_X_VEL;
	} else
		samus->vel_x = 0;
	layBomb();
	if(!bouncingCollision(-1))
		return;
	samus->vel_y-=JUMPING_GRAVITY;
	if(game->keys.up == 1) {
		game->keys.up = 2;
		if(!hitWallY(KNEEL_HEAD, SAMUS_LEFT,SAMUS_RIGHT, 1, 'b')) {
			samus->mode=jumpingLeft;
			samus->sprt = &sprites->samus.leftJump1;
			samus->vel_y/=3;
			return;
		}
	}
	if(game->keys.right == 1) {
		game->keys.right = 2;
		samus->vel_x = 0;
		samus->mode = bouncingRight;
		return;
	}
	if(samus->vel_y <= 0) {
		samus->vel_y = 0;
		samus->mode = fallingBallLeft;
		return;
	}
}
void bouncingRight(void) {
	samus->frames=BALL_FRAMES;
	if(samus->beenHurt>0) {
		if(--samus->beenHurt==0) {
			samus->mode=fallingBallRight;
		} else {
			samus->vel_x=0;
		}
	}
	if(game->keys.right) {
		game->keys.right = 2;
		samus->vel_x = IN_AIR_X_VEL;
	} else
		samus->vel_x = 0;
	layBomb();
	if(!bouncingCollision(1))
		return;
	samus->vel_y-=JUMPING_GRAVITY;
	if(game->keys.up == 1) {
		game->keys.up = 2;
		if(!hitWallY(KNEEL_HEAD, SAMUS_LEFT,SAMUS_RIGHT, 1, 'b')) {
			samus->mode=jumpingRight;
			samus->sprt = &sprites->samus.rightJump1;
			samus->vel_y/=3;
			return;
		}
	}
	if(game->keys.left == 1) {
		game->keys.left = 2;
		samus->vel_x = 0;
		samus->mode = bouncingLeft;
		return;
	}
	if(samus->vel_y <= 0) {
		samus->vel_y = 0;
		samus->mode = fallingBallRight;
		return;
	}
}

void spiderLeft(void) {
	//pause('a');
	if(_keytest(RR_LEFT) || ((samus->vel_y == -1 && _keytest(RR_UP)) || (samus->vel_y == 1 && _keytest(RR_DOWN)))) {
		//pause('a');
		samus->vel_x++;
		if(samus->vel_x > SPIDER_MAX)
			samus->vel_x = SPIDER_MAX;
	}	else {
		//pause('a');
		//samus->vel_y = 0;
		samus->vel_x = 0;
		if(_keytest(RR_RIGHT)) {
			samus->vel_y*=-1;
			samus->mode = spiderRight;
			spiderRight();
			return;
		}
	}
	layBomb();
	//printf("%d", samus->vel_x);
	//pause(' ');
	if(samus->vel_x)
		spider(-1);
	if(!_keytest(RR_SHIFT))
		samus->mode = rollingLeft;
}
void spiderRight(void) {
	if(_keytest(RR_RIGHT) || ((samus->vel_y == -1 && _keytest(RR_UP)) || (samus->vel_y == 1 && _keytest(RR_DOWN)))) {
		samus->vel_x++;
		if(samus->vel_x > SPIDER_MAX)
			samus->vel_x = SPIDER_MAX;
	}	else {
		//samus->vel_y = 0;
		samus->vel_x = 0;
		if(_keytest(RR_LEFT)) {
			samus->vel_y*=-1;
			samus->mode = spiderLeft;
			spiderLeft();
			return;
		}
	}
	layBomb();


	if(samus->vel_x)
		spider(1);
	if(!_keytest(RR_SHIFT))
		samus->mode = rollingRight;
}
void spiderUp(void) {
	if(_keytest(RR_UP) || ((samus->vel_x == 1 && _keytest(RR_RIGHT)) || (samus->vel_x == -1 && _keytest(RR_LEFT)))) {
		samus->vel_y++;
		if(samus->vel_y > SPIDER_MAX)
			samus->vel_y = SPIDER_MAX;
	}	else {
		//samus->vel_x = 0;
		samus->vel_y = 0;
		if(_keytest(RR_DOWN)) {
			samus->vel_x*=-1;
			samus->mode = spiderDown;
			spiderDown();
			return;
		}
	}
	layBomb();

	if(samus->vel_y)
		spider(-1);
	if(!_keytest(RR_SHIFT)) {
		if(samus->spiderWall == 2)
			samus->mode = morphedLeft;
		else
			samus->mode = morphedRight;
	}
}
void spiderDown(void) {
	if(_keytest(RR_DOWN) || ((samus->vel_x == 1 && _keytest(RR_RIGHT)) || (samus->vel_x == -1 && _keytest(RR_LEFT)))) {
		samus->vel_y++;
		if(samus->vel_y > SPIDER_MAX)
			samus->vel_y = SPIDER_MAX;
	}	else {
		samus->vel_x = 0;
		samus->vel_y = 0;
		if(_keytest(RR_UP)) {
			samus->mode = spiderUp;
			spiderUp();
			return;
		}
	}
	layBomb();


	if(samus->vel_y)
		spider(1);
	if(!_keytest(RR_SHIFT)) {
		if(samus->spiderWall == 2)
			samus->mode = morphedLeft;
		else
			samus->mode = morphedRight;
	}
}

void spider(char dir) {
	int *x = &samus->x, *y = &samus->y, *vel = &samus->vel_x, *v = &samus->y;
	switch(samus->spiderWall) {
		case 2:
		case 3:
			x = &samus->y;
			y = &samus->x;
			vel = &samus->vel_y;
			v = &samus->x;
			break;
	}
	int i;
	for(i = 0; i < *vel; i++) {
		int backupX = *x;
		int backupY = *y;
		int backupV = *v;
		(*x)+=dir;
		int cx, cy;
		cx = samus->x+8;
		cy = samus->y-1;
		printf_xy(0, 0, "%d", samus->spiderWall);
		switch(samus->spiderWall) {
			case 1:
				cy = samus->y - 8;
				break;
			case 2:
				cy = samus->y - 5;
				cx = samus->x + 4;
				break;
			case 3:
				cy = samus->y - 5;
				cx = samus->x + 11;
				break;
		}
		if(!groundHug(cx,cy,v,samus->spiderWall)) {
			pause('a');
			switch(samus->spiderWall) {
				case 0:
					samus->mode = spiderDown;
					samus->vel_y = samus->vel_x;
					//samus->y++;
					if(dir<0) {
						pause('1');
						samus->spiderWall = 3;
						samus->y = cy + 5;
						samus->x = cx - 11;
					} else {
						pause('2');
						samus->spiderWall = 2;
						samus->y = cy + 5;
						samus->x = cx - 4;
					}
					break;
				case 1:
					samus->mode = spiderUp;
					samus->vel_y = samus->vel_x;
					//samus->y--;
					if(dir<0) {
						pause('3');
						samus->spiderWall = 3;
						samus->y = cy + 5;
						samus->x = cx - 11;
					} else {
						pause('4');
						samus->spiderWall = 2;
						samus->y = cy + 5;
						samus->x = cx - 4;
					}
					break;
				case 2:
					samus->mode = spiderLeft;
					samus->vel_x = samus->vel_y;
					//samus->x--;
					if(dir<0) {
						pause('5');
						samus->spiderWall = 0;
						samus->x = cx - 8;
						samus->y = cy + 1;
					} else {
						pause('6');
						samus->spiderWall = 1;
						samus->x = cx - 8;
						samus->y = cy + 8;
					}
					break;
				case 3:
					samus->mode = spiderRight;
					samus->vel_x = samus->vel_y;
					//samus->x++;
					if(dir<0) {
						pause('7');
						samus->spiderWall = 0;
						samus->x = cx - 8;
						samus->y = cy + 1;
					} else {
						pause('8');
						samus->spiderWall = 1;
						samus->x = cx - 8;
						samus->y = cy + 8;
					}
					break;
			}
			*vel = dir;
			return;
		}
		char wall = 0;
		switch(samus->spiderWall) {
			case 0:
				if((dir < 0 && hitWallX(SAMUS_LEFT, SAMUS_FEET, MORPH_TOP, 1, 'l')) || (dir > 0 && hitWallX(SAMUS_RIGHT,SAMUS_FEET,MORPH_TOP, 1, 'r'))) {
					pause('b');
					samus->mode = spiderUp;
					samus->vel_y = samus->vel_x;
					//samus->y--;
					if(dir<0) {
						pause('1');
						samus->spiderWall = 2;
					} else {
						pause('2');
						samus->spiderWall = 3;
					}
					wall = 1;
				}
				break;
			case 1:
				if((dir < 0 && hitWallX(SAMUS_LEFT, SAMUS_FEET + 4, MORPH_TOP + 4, 1, 'l')) || (dir > 0 && hitWallX(SAMUS_RIGHT,SAMUS_FEET+4,MORPH_TOP+4, 1, 'r'))) {
					pause('b');
					samus->mode = spiderDown;
					samus->vel_y = samus->vel_x;
					//samus->y++;
					if(dir<0) {
						pause('3');
						samus->spiderWall = 2;
					} else {
						pause('4');
						samus->spiderWall = 3;
					}
					wall = 1;
				}
				break;
			case 2:
				if((dir < 0 && hitWallY(MORPH_TOP, SAMUS_LEFT+4, SAMUS_RIGHT, 1, 'b')) || (dir > 0 && hitWallY(SAMUS_FEET+4, SAMUS_LEFT+4, SAMUS_RIGHT, 1, 't'))) {
					pause('b');
					samus->mode = spiderRight;
					samus->vel_x = samus->vel_y;
					//samus->x++;
					if(dir<0) {
						pause('5');
						samus->spiderWall = 1;
					} else {
						pause('6');
						samus->spiderWall = 0;
					}
					wall = 1;
				}
				break;
			case 3:
				if((dir < 0 && hitWallY(MORPH_TOP, SAMUS_LEFT, SAMUS_RIGHT-4, 1, 'b')) || (dir > 0 && hitWallY(SAMUS_FEET+4, SAMUS_LEFT, SAMUS_RIGHT-4, 1, 't'))) {
					pause('b');
					samus->mode = spiderLeft;
					samus->vel_x = samus->vel_y;
					//samus->x--;
					if(dir<0) {
						pause('7');
						samus->spiderWall = 1;
					} else {
						pause('8');
						samus->spiderWall = 0;
					}
					wall = 1;
				}
				break;
		}
		if(wall) {
			pause('c');
			*x = backupX;
			//*y = backupY;
			*vel = dir;
			//pause('b');
			return;
		}
	}
	pause('d');
}

void drawSamus(void) {
	void* v2 = v0;
	if(samus->beenHurt > 0 || samus->invincible > 0)
		v2 = v1;
	GrayClipSprite16_OR_R(samus->x-cam->x,samus->y-cam->y-samus->sprt->h,samus->sprt->h,samus->sprt->s1,samus->sprt->s2,v2,v1);
}

int groundHug(int cx, int cy, int *v, int wall) {
	char dir = wall==0?'t':wall==1?'b':wall==2?'l':'r';
	switch(wall) {
		case 0:
			if(tiles[map->array[1-map->array[(cy/8)*map->w]+(cy/8)*map->w+(cx/8)]].m != NULL) {
				*v = tiles[map->array[1-map->array[(cy/8)*map->w]+(cy/8)*map->w+(cx/8)]].m[0+cx%8] + (cy/8)*8 + (*v - cy - 1);
				//onCollide((cx/8)*8,(cy/8)*8, dir);
			} else if(tiles[map->array[1-map->array[((cy+8)/8)*map->w]+((cy+8)/8)*map->w+(cx/8)]].m != NULL) {
				*v = tiles[map->array[1-map->array[((cy+8)/8)*map->w]+((cy+8)/8)*map->w+(cx/8)]].m[0+cx%8] + ((cy+8)/8)*8 + (*v - cy - 1);
				//onCollide((cx/8)*8,((cy+8)/8)*8, dir);
			} else
				return 0;
			break;
		case 1:
			if(tiles[map->array[1-map->array[(cy/8)*map->w]+(cy/8)*map->w+(cx/8)]].m != NULL) {
				*v = tiles[map->array[1-map->array[(cy/8)*map->w]+(cy/8)*map->w+(cx/8)]].m[8+cx%8] + (cy/8)*8 + (*v - cy + 1);
				//onCollide((cx/8)*8,(cy/8)*8, dir);
			} else if(tiles[map->array[1-map->array[((cy-8)/8)*map->w]+((cy-8)/8)*map->w+(cx/8)]].m != NULL) {
				*v = tiles[map->array[1-map->array[((cy-8)/8)*map->w]+((cy-8)/8)*map->w+(cx/8)]].m[8+cx%8] + ((cy-8)/8)*8 + (*v - cy + 1);
				//onCollide((cx/8)*8,((cy+8)/8)*8, dir);
			} else
				return 0;
			break;
		case 2:
			if(tiles[map->array[1-map->array[(cy/8)*map->w]+(cy/8)*map->w+(cx/8)]].m != NULL) {
				*v = tiles[map->array[1-map->array[(cy/8)*map->w]+(cy/8)*map->w+(cx/8)]].m[16+cy%8] + (cx/8)*8 + (*v - cx + 1);
				//onCollide((cx/8)*8,(cy/8)*8, dir);
			} else if(tiles[map->array[1-map->array[((cy)/8)*map->w]+((cy)/8)*map->w+((cx-8)/8)]].m != NULL) {
				*v = tiles[map->array[1-map->array[((cy)/8)*map->w]+((cy)/8)*map->w+((cx-8)/8)]].m[16+cy%8] + ((cx-8)/8)*8 + (*v - cx + 1);
				//onCollide((cx/8)*8,((cy+8)/8)*8, dir);
			} else
				return 0;
			break;
		case 3:
			if(tiles[map->array[1-map->array[(cy/8)*map->w]+(cy/8)*map->w+(cx/8)]].m != NULL) {
				*v = tiles[map->array[1-map->array[(cy/8)*map->w]+(cy/8)*map->w+(cx/8)]].m[24+cy%8] + (cx/8)*8 + (*v - cx - 1);
				//onCollide((cx/8)*8,(cy/8)*8, dir);
			} else if(tiles[map->array[1-map->array[((cy)/8)*map->w]+((cy)/8)*map->w+((cx+8)/8)]].m != NULL) {
				*v = tiles[map->array[1-map->array[((cy)/8)*map->w]+((cy)/8)*map->w+((cx+8)/8)]].m[24+cy%8] + ((cx+8)/8)*8 + (*v - cx - 1);
				//onCollide((cx/8)*8,((cy+8)/8)*8, dir);
			} else
				return 0;
			break;
	}
	return 1;
}

int hitSamus(int x, int y, int w, int h) {
	return
		boundingBoxCollide(x,y,w,h,samus->x,samus->y,samus->sprt->w,samus->sprt->h,1);
}

int boundingBoxCollide(int x, int y, int w, int h, int x2, int y2, int w2, int h2, int t) {
	int r =
		(x<x2&&x+w>x2&&y>y2&&y-h<y2) ||
		(x<x2+w2&&x+w>x2+w2&&y>y2&&y-h<y2) ||
		(x<x2&&x+w>x2&&y>y2-h2&&y-h<y2-h2) ||
		(x<x2+w2&&x+w>x2+w2&&y>y2-h2&&y-h<y2-h2) ||
		(x<x2+w2/2&&x+w>x2+w2/2&&y>y2&&y-h<y2) ||
		(x<x2+w2/2&&x+w>x2+w2/2&&y>y2-h2&&y-h<y2-h2) ||
		(x<x2&&x+w>x2&&y>y2-h2/3&&y-h<y2-h2/3) ||
		(x<x2+w2&&x+w>x2+w2&&y>y2-h2/3&&y-h<y2-h2/3) ||
		(x<x2&&x+w>x2&&y>y2-2*h2/3&&y-h<y2-2*h2/3) ||
		(x<x2+w2&&x+w>x2+w2&&y>y2-2*h2/3&&y-h<y2-2*h2/3);

	return t>1?r:(r?r:boundingBoxCollide(x2,y2,w2,h2,x,y,w,h,2));

}

void hurtSamus(int ATK) {
	if(samus->beenHurt>0 || samus->invincible!=0)
		return;
	samus->beenHurt=HURT_LENGTH;
	samus->HP-=ATK;
	while(samus->HP < 0) {
		if(samus->items.tanks_full > 0) {
			samus->items.tanks_full--;
			samus->HP+=100;
		}
		else {
			while(!_keytest(RR_ENTER));
			exit(0);
		}
	}
}