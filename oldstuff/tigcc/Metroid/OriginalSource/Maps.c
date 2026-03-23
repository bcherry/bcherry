// C Source File
// Created 1/27/2005; 8:31:32 PM

#include "all.h"


void loadMap(int m, int d, int x, int y) {

	freeObjects();
	freeEnemies();
	FILE* f = fopen("metmaps","rb");
	if(f==NULL) {
		pause('d');
		printf("metmaps.MET: Failed to open file\n");
		while(!_keytest(RR_ENTER));
		exit(0);
	}
	int i = 0;
	for(i = 0; i <= m; i++) {
		if(fread(map, sizeof(struct map), 1, f)<1) {
			printf("metmaps.MET: Failed to read file\n");
			fclose(f);
			while(!_keytest(RR_ENTER));
			exit(0);
		}
	}
	fclose(f);
	setContrast(map->contrast);
}

void clearDoor(int x, int y, char dir) {
	switch(dir) {
		case 'r':
		case 'l':
			map->array[1+y*map->w+x] = 0;
			map->array[1+(y+1)*map->w+x] = 0;
			map->array[1+(y+2)*map->w+x] = 0;
			map->array[1+(y+3)*map->w+x] = 0;
			break;
		case 'u':
		case 'd':
			map->array[1+y*map->w+x]=0;
			map->array[1+y*map->w+x+1]=0;
			map->array[1+y*map->w+x+2]=0;
			map->array[1+y*map->w+x+3]=0;
			break;
	}
}

void fillDoor(int x, int y, char dir) {
	switch(dir) {
		case 'r':
			map->array[1+y*map->w+x] = 19;
			map->array[1+(y+1)*map->w+x] = 20;
			map->array[1+(y+2)*map->w+x] = 21;
			map->array[1+(y+3)*map->w+x] = 22;
			break;
		case 'l':
			map->array[1+y*map->w+x] = 11;
			map->array[1+(y+1)*map->w+x] = 12;
			map->array[1+(y+2)*map->w+x] = 13;
			map->array[1+(y+3)*map->w+x] = 14;
			break;
		case 'u':
			map->array[1+y*map->w+x] = 32;
			map->array[1+y*map->w+x+1] = 33;
			map->array[1+y*map->w+x+2] = 34;
			map->array[1+y*map->w+x+3] = 35;
			break;
		case 'd':
			map->array[1+y*map->w+x] = 40;
			map->array[1+y*map->w+x+1] = 41;
			map->array[1+y*map->w+x+2] = 42;
			map->array[1+y*map->w+x+3] = 43;
			break;
	}
}

void drawMap(void) {
	int i;
	for(i = 0; i < map->d; i++) {
		if(map->doors[i].state == 0) {
			fillDoor(map->doors[i].x,map->doors[i].y,map->doors[i].dir);
		} else if(map->doors[i].state == 1){
			clearDoor(map->doors[i].x,map->doors[i].y,map->doors[i].dir);
		}
	}
	int r, c;
	for(r = cam->y/8 - BUFFER; r < (int)(cam->y/8 + LCD_HEIGHT/8 + BUFFER); r++) {
		for(c = cam->x/8 - BUFFER; c < (int)(cam->x/8 + LCD_WIDTH/8 + BUFFER); c++) {
			if((r < 0 || r > map->h) ||
				 (c < map->array[r*map->w]) ||
				 (c >= map->array[r*map->w] + (map->w-1))) {
				ClipSprite8_OR_R(c*8-cam->x,r*8-cam->y,8, gfx->tiles.black, v0);
				ClipSprite8_OR_R(c*8-cam->x,r*8-cam->y,8, gfx->tiles.black, v1);
			} else {
				if(map->array[1+r*map->w+c-map->array[r*map->w]] >= 200) {
					pushEnemy(c*8,(r+1)*8, map->array[1+r*map->w+c-map->array[r*map->w]] - 200);
					map->array[1+r*map->w+c] = 0;
					continue;
				}
				GrayClipSprite8_SMASK_R(c*8-cam->x, r*8-cam->y, 8, tiles[map->array[1+r*map->w+c-map->array[r*map->w]]].s, tiles[map->array[1+r*map->w+c-map->array[r*map->w]]].s+8, tiles[1+map->array[r*map->w+c-map->array[r*map->w]]].s+16, v0, v1);
				onDraw(c,r);
			}
		}
	}

	/*for(c = cam->x/8 - BUFFER; c < (int)(cam->x/8 + LCD_WIDTH/8 + BUFFER); c++) {
		if(c < 0 || c >= map->w) continue;
		for(r = cam->y/8 - BUFFER; r < (int)(cam->y/8 + LCD_HEIGHT/8 + BUFFER); r++) {
			if(r < 0 || r >= map->h) continue;
			if(map->array[r*map->w+c] >= 200) {
				pushEnemy(c*8,(r+1)*8, map->array[r*map->w+c] - 200);
				map->array[r*map->w+c] = 0;
				continue;
			}
			GrayClipSprite8_SMASK_R(c*8-cam->x, r*8-cam->y, 8, tiles[map->array[r*map->w+c]].s, tiles[map->array[r*map->w+c]].s+8, tiles[map->array[r*map->w+c]].s+16, v0, v1);
			if(tiles[map->array[r*map->w+c]].onDraw!=NULL)
				tiles[map->array[r*map->w+c]].onDraw(c,r);
		}
	}*/
}

void makeMaps(void) {
	if((map = malloc(sizeof(struct map))) == NULL)
		exit(0);
	if((cam = malloc(sizeof(struct cam))) == NULL)
		exit(0);
	struct cam c = {0,0,1};
	memcpy(cam,&c,sizeof(c));
}
