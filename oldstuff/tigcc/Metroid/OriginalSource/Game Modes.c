// C Source File
// Created 2/2/2005; 6:50:27 PM

#include "all.h"

int enemies_c = 0;
int shots_c = 0;

int playingMode(void) {
	if(_keytest(RR_ESC))
		return 0;

	if(checkDoors())
		return 1;

	if(_keytest(RR_F2)) {
		samus->invincible=-1;
	}
	if(_keytest(RR_F4)) {
		printf_xy(0, 0, "%d %d", enemies_c, shots_c);
		while(!_keytest(RR_ENTER));
		clrscr();
	}
	if(_keytest(RR_CLEAR)) {
		if(cam->hud == 0)
			cam->hud = 11;
		else if(cam->hud == 1)
			cam->hud = -10;
	}
	getKeys();


	if(_keytest(RR_PLUS))OSContrastUp();
	if(_keytest(RR_MINUS))OSContrastDn();

	drawScreen();

	if(samus->invincible>0)
		samus->invincible--;
	if(++game->frame>100)
		game->frame=0;
	if((game->frame%samus->frames)==0)
		samus->sprt = samus->sprt->link;
	weaponsSetup();
	samus->mode();
	adjustCamera();
	doEnemies();
	doObjects();

	//delay(0);

	return 1;
}

int mapTransition(void) {
	int x = 0, y = 0, d = game->frame;
	switch(map->doors[d].dir) {
		case 'l':
			x = map->w*8 - LCD_WIDTH;
			y = map->doors[d].y*8 + 16 - LCD_HEIGHT/2;
			break;
		case 'r':
			x = 0;
			y = map->doors[d].y*8 + 16 - LCD_HEIGHT/2;
			break;
		case 'u':
			x = map->doors[d].x*8 + 16 - LCD_WIDTH/2;
			y = map->h*8 - LCD_HEIGHT;
			break;
		case 'd':
			x = map->doors[d].x*8 + 16 - LCD_WIDTH/2;
			y = 0;
			break;
	}
	if(y < 0)
		y = 0;
	else if((int)y >= (int)(map->h*8 - LCD_HEIGHT))
		y = map->h*8 - LCD_HEIGHT;
	if(x < 0)
		x = 0;
	else if((int)x >= (int)(map->w*8 - LCD_WIDTH))
		x = map->w*8 - LCD_WIDTH;
	if(cam->x == x && cam->y == y) {
		game->mode = playingMode;
		game->frame = 0;
		return 1;
	} else {
		GrayClearScreen2B(v0,v1);
		//GrayFillScreen2B_R(v0,v1,3,3);
		memset(v0,0xFF,3840);
		//memset(v1,0xFF,3840);
		drawDoors();
		FastCopyScreen(v0, GetPlane(0));
		FastCopyScreen(v1, GetPlane(1));
		delay(15);
		//drawScreen();
		if(cam->x < x) {
			cam->x+=2;
			if(cam->x >x)
				cam->x = x;
		} else if(cam->x > x) {
			cam->x-=2;
			if(cam->x < x)
				cam->x = x;
		}
		if(cam->y < y) {
			cam->y+=2;
			if(cam->y >y)
				cam->y = y;
		}	else if(cam->y > y) {
			cam->y-=2;
			if(cam->y < y)
				cam->y = y;
		}
	}
	return 1;
}

void drawDoors(void) {
	int x = 0, y = 0, d = game->frame, n = 0;
	switch(map->doors[d].dir) {
		case 'l':
			x = map->doors[d].x;
			y = map->doors[d].y;
			n = 11;
			break;
		case 'r':
			x = map->doors[d].x - 2;
			y = map->doors[d].y;
			n = 11;
			break;
		case 'u':
			x = map->doors[d].x;
			y = map->doors[d].y;
			n = 32;
			break;
		case 'd':
			x = map->doors[d].x;
			y = map->doors[d].y - 2;
			n = 32;
			break;
	}
	int i, j;
	for(i = 0; i < 3; i++) {
		for(j = 0; j < 4; j++) {
			int x2, y2;
			if(n == 11) {
				x2 = x + i;
				y2 = y + j;
			} else {
				x2 = x + j;
				y2 = y + i;
			}
			GrayClipSprite8_SMASK_R(x2*8-cam->x, y2*8-cam->y, 8, tiles[n+i*4+j].s, tiles[n+i*4+j].s+8, tiles[n+i*4+j].s+16, v0, v1);
		}
	}
}

void adjustCamera(void) {
	if(samus->x < (cam->x + CAMERA_MOVE)) {
		cam->x = samus->x - CAMERA_MOVE;
		if(cam->x < map->l)
			cam->x = map->l;
	} else if((int)(samus->x) > (int)(cam->x + LCD_WIDTH - CAMERA_MOVE)) {
		cam->x = samus->x + CAMERA_MOVE - LCD_WIDTH;
		if((int)(cam->x + LCD_WIDTH) >= (int)(map->r*8))
			cam->x = map->r*8 - LCD_WIDTH;
	}
	if((int)samus->y > (int)(cam->y + LCD_HEIGHT - CAMERA_MOVE)) {
		cam->y = samus->y + CAMERA_MOVE - LCD_HEIGHT;
		if((int)cam->y > (int)(map->b*8 - LCD_HEIGHT))
			cam->y = map->b*8 - LCD_HEIGHT;
	} else if((int)(samus->y ) < (int)(cam->y + CAMERA_MOVE)) {
		cam->y = samus->y  - CAMERA_MOVE;
		if(cam->y < map->t)
			cam->y = map->t;
	}
}
void drawScreen(void) {
	GrayClearScreen2B(v0,v1);
	//GrayFillScreen2B_R(v0,v1,3,3);
	//memset(v0,0xFF,3840);
	//memset(v1,0xFF,3840);
	drawMap();
	drawSamus();
	drawEnemies();
	drawObjects();
	drawHud();
	#ifdef DEBUG
	clrscr();
	#endif
	FastCopyScreen(v0, GetPlane(0));
	FastCopyScreen(v1, GetPlane(1));
	//pause('g');
}
void doObjects(void) {
	struct objnode* curr = objects->first;
	struct objnode* prev = NULL;
	shots_c = 0;
	while(curr!=NULL) {
		if(game->frame%curr->value->frames==0)
			curr->value->sprt=curr->value->sprt->link;
		if(curr->value->x < map->l*8-16 || curr->value->x > map->r*8 + 16 || curr->value->y < map->t*8-16 || curr->value->y > map->b*8 + 16)
			curr->value->gone = 1;
		if(curr->value->gone) {
			if(curr == objects->first) {
				objects->first = curr->link;
				free(curr->value);
				free(curr);
				curr = objects->first;
			} else {
				prev->link = curr->link;
				free(curr->value);
				free(curr);
				curr = prev->link;
			}
		} else {
			shots_c++;
			curr->value->behavior(curr->value);
			prev = curr;
			curr = curr->link;
		}
	}
}

int checkDoors(void) {
	if(SAMUS_LEFT < 3 || SAMUS_RIGHT > (map->w)*8 - 3 || samus->y - samus->sprt->h < 3 || samus->y > (map->h)*8 - 3) {
		int i;
		for(i = 0; i < map->d; i++) {
			if(map->doors[i].state == 0)
				continue;
			else if(((map->doors[i].dir == 'l' || map->doors[i].dir == 'r') && (samus->y - samus->sprt->h >= map->doors[i].y*8 && samus->y <= (map->doors[i].y+4)*8)) ||
							((map->doors[i].dir == 'u' || map->doors[i].dir == 'd') && (SAMUS_LEFT >= map->doors[i].x*8 && SAMUS_RIGHT <= (map->doors[i].x+4)*8))) {
				int m = map->doors[i].map_link;
				int d = map->doors[i].link_i;
				cam->y = map->doors[i].y*8 - cam->y;
				cam->x = map->doors[i].x*8 - cam->x;
				loadMap(map->doors[i].map_link, map->doors[i].link_i, map->doors[i].x*8, map->doors[i].y*8);
				switch(map->doors[d].dir) {
					case 'l':
						samus->x=map->doors[d].x*8-16;
						samus->y=map->doors[d].y*8+32;
						cam->x = map->w*8;
						cam->y = map->doors[d].y*8 - cam->y;
						break;
					case 'r':
						samus->x=map->doors[d].x*8+8;
						samus->y=map->doors[d].y*8+32;
						cam->x = -LCD_WIDTH;
						cam->y = map->doors[d].y*8 - cam->y;
						break;
					case 'u':
						samus->x=map->doors[d].x*8+8;
						samus->y=map->doors[d].y*8;
						cam->x = map->doors[d].x*8 - cam->x;
						cam->y = map->h*8;
						break;
					case 'd':
						samus->x=map->doors[d].x*8+8;
						samus->y=map->doors[d].y*8+32;
						cam->x = map->doors[d].x*8 - cam->x;
						cam->y = -LCD_HEIGHT;
						break;
				}
				game->mode=mapTransition;
				game->frame = d;
				return 1;
			}
		}
	}
	return 0;
}

void drawObjects(void) {
	struct objnode* curr;
	for(curr = objects->first; curr!=NULL; curr=curr->link)
		drawObject(curr->value);
}
void drawEnemies(void) {
	struct enemyNode* curr;
	for(curr = enemies->first; curr != NULL; curr = curr->next)
		drawEnemy(curr->value);
}
void doEnemies(void) {
	enemies_c = 0;
	struct enemyNode* curr = enemies->first;
	struct enemyNode* prev = NULL;
	while(curr!=NULL) {
		if(curr->value->dead != 0) {
			if(curr == enemies->first) {
				enemies->first = curr->next;
				free(curr->value);
				free(curr);
				curr = enemies->first;
			} else {
				prev->next = curr->next;
				free(curr->value);
				free(curr);
				curr = prev->next;
			}
		} else {
			enemies_c++;
			if(game->frame%curr->value->frames==0)
				curr->value->sprt=curr->value->sprt->link;
			curr->value->behavior(curr->value);
			if(curr->value->ATK > 0) {
				if(hitSamus(curr->value->x, curr->value->y, curr->value->w, curr->value->h)) {
					hurtEnemy(curr->value, 15, 0);
					hurtSamus(curr->value->ATK);
				}
			}
			if(curr->value->x < map->l*8-16 || curr->value->x > map->r*8 + 16 || curr->value->y < map->t*8-16 || curr->value->y > map->b*8 + 16)
				curr->value->dead = -1;
			prev = curr;
			curr = curr->next;
		}
	}
}
void getKeys(void) {
	if(!_keytest(RR_UP))
		game->keys.up = 0;
	else if(_keytest(RR_UP) && game->keys.up == 0)
		game->keys.up = 1;
	if(!_keytest(RR_DOWN))
		game->keys.down = 0;
	else if(_keytest(RR_DOWN) && game->keys.down == 0)
		game->keys.down = 1;
	if(!_keytest(RR_LEFT))
		game->keys.left = 0;
	else if(_keytest(RR_LEFT) && game->keys.left == 0)
		game->keys.left = 1;
	if(!_keytest(RR_RIGHT))
		game->keys.right = 0;
	else if(_keytest(RR_RIGHT) && game->keys.right == 0)
		game->keys.right = 1;
	if(!_keytest(SHOOT))
		game->keys.fire = 0;
	else if(_keytest(SHOOT) && game->keys.fire == 0)
		game->keys.fire = 1;
	if(!_keytest(JUMP))
		game->keys.jump = 0;
	else if(_keytest(JUMP) && game->keys.jump == 0)
		game->keys.jump = 1;
	if(!_keytest(AIM))
		game->keys.aim = 0;
	else if(_keytest(AIM) && game->keys.aim == 0)
		game->keys.aim = 1;
	if(!_keytest(SPRINT))
		game->keys.sprint = 0;
	else if(_keytest(SPRINT) && game->keys.sprint == 0)
		game->keys.sprint = 1;
	if(!_keytest(RR_F1))
		game->keys.f1 = 0;
	else if(_keytest(RR_F1) && game->keys.f1 == 0)
		game->keys.f1 = 1;
	if(!_keytest(RR_F2))
		game->keys.f2 = 0;
	else if(_keytest(RR_F2) && game->keys.f2 == 0)
		game->keys.f2 = 1;
	if(!_keytest(RR_F3))
		game->keys.f3 = 0;
	else if(_keytest(RR_F3) && game->keys.f3 == 0)
		game->keys.f3 = 1;
	if(!_keytest(RR_F4))
		game->keys.f4 = 0;
	else if(_keytest(RR_F4) && game->keys.f4 == 0)
		game->keys.f4 = 1;
	if(!_keytest(RR_F5))
		game->keys.f5 = 0;
	else if(_keytest(RR_F5) && game->keys.f5 == 0)
		game->keys.f5 = 1;

	if(game->keys.aim) {
		if(game->keys.up)
			game->keys.up = 2;
		if(game->keys.down)
			game->keys.down = 2;
		if(game->keys.left)
			game->keys.left = 2;
		if(game->keys.right)
			game->keys.right = 2;
	}
}

void makeGame(void) {
	if((game = malloc(sizeof(struct game))) == NULL)
		exit(0);
	struct game g = {
		playingMode,	// MODE
		0,						// FRAME
		{/********KEYS*******/
			0,					// UP
			0,					// DOWN
			0,					// LEFT
			0,					// RIGHT
			0,					// FIRE
			0,					// SPRINT
			0,					// JUMP
			0,					// AIM
			0,0,0,0,0		// F 1-5
		}
	};
	memcpy(game, &g, sizeof(g));
}