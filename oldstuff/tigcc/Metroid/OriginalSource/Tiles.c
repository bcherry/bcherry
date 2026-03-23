// C Source File
// Created 3/2/2005; 11:08:38 PM

#include <tigcclib.h>
#include "all.h"

void makeTiles(void) {
	/**********
	TILE KEY
 **********/
	struct tile t[] = {{
		/*Tile 0 - Empty*/
		gfx->tiles.empty8x8,/*Sprite*/
	  NULL
		},{
		/*Tile 1 - Solid*/
		gfx->tiles.darkRocksSolid,
	  gfx->tiles.masks.solid
		},{
		/*Tile 2 - Slope Up*/
		gfx->tiles.darkRocksUp,
	  gfx->tiles.masks.slopeUp
		},{
		/*Tile 3 - Slope Down*/
		gfx->tiles.darkRocksDown,
		gfx->tiles.masks.slopeDown
		},{
		/*Tile 4 - Roof Up*/
		gfx->tiles.darkRocksRUp,
		gfx->tiles.masks.roofUp
		},{
		/*Tile 5 - Roof Down*/
		gfx->tiles.darkRocksRDown,
		gfx->tiles.masks.roofDown
		},{
		/*Tile 6 - Solid Pass Thru*/
		gfx->tiles.darkRocksSolid,
	  NULL
		},{
		/*Tile 7 - Breakaway*/
		gfx->tiles.breakAway,
	  gfx->tiles.masks.solid
		},{
		/*Tile 8 - OPEN##################################################################################*/
		gfx->tiles.empty8x8,
	  gfx->tiles.masks.solid
		},{/*##########################################################################################*/
		/*Tile 9 - Bomb Broken*/
		gfx->tiles.bombBreak,
	  gfx->tiles.masks.solid
		},{
		/*Tile 10 - Bomb break looks like Tile 1*/
		gfx->tiles.darkRocksSolid,
	  gfx->tiles.masks.solid
		},{
		/*Tile 11 - Top of Left Facing Door*/
		gfx->tiles.DoorOpenL1,
		gfx->tiles.masks.slopeUp
		},{
		/*Tile 12 - Top Middle of Left Facing Door*/
		gfx->tiles.DoorOpenL2,
	  gfx->tiles.masks.solid
		},{
		/*Tile 13 - Bottom Middle of Left Facing Door*/
		gfx->tiles.DoorOpenL3,
	  gfx->tiles.masks.solid
		},{
		/*Tile 14 - Bottom of Left Facing Door*/
		gfx->tiles.DoorOpenL4,
		gfx->tiles.masks.roofDown
		},{
		/*Tile 15 - Door Base Horizontal Piece 1*/
		gfx->tiles.DoorBaseH1,
	  NULL
		},{
		/*Tile 16 - Door Base Horizontal Piece 2*/
		gfx->tiles.DoorBaseH2,
	  NULL
		},{
		/*Tile 17 - Door Base Horizontal Piece 3*/
		gfx->tiles.DoorBaseH3,
	  NULL
		},{
		/*Tile 18 - Door Base Horizontal Piece 4*/
		gfx->tiles.DoorBaseH4,
	  NULL
		},{
		/*Tile 19 - Top of Right Facing Door*/
		gfx->tiles.DoorOpenR1,
		gfx->tiles.masks.slopeDown
		},{
		/*Tile 20 -  Top Middle of Right Facing Door*/
		gfx->tiles.DoorOpenR2,
	  gfx->tiles.masks.solid
		},{
		/*Tile 21 -  Bottom Middle of Right Facing Door*/
		gfx->tiles.DoorOpenR3,
	  gfx->tiles.masks.solid
		},{
		/*Tile 22 - Bottom of Right Facing Door*/
		gfx->tiles.DoorOpenR4,
		gfx->tiles.masks.roofUp
		},{
		/*Tile 23 - Grassy Bottom*/
		gfx->tiles.grassytop,
	  gfx->tiles.masks.solid
		},{
		/*Tile 24 - Rocky Bottom*/
		gfx->tiles.rockybottom,
	  gfx->tiles.masks.solid
		},{
		/*Tile 25 - Single Floor*/
		gfx->tiles.SingleFloor,
	  gfx->tiles.masks.solid
		},{
		/*Tile 26 - Metal Corner*/
		gfx->tiles.MetalCorner,
	  gfx->tiles.masks.solid
		},{
		/*Tile 27 - Metal Bar H*/
		gfx->tiles.MetalBarH,
	  gfx->tiles.masks.solid
		},{
		/*Tile 28 - Metal Bar V*/
		gfx->tiles.MetalBarV,
	  gfx->tiles.masks.solid
		},{
		/*Tile 29 - Metal Bar H pass-thru*/
		gfx->tiles.MetalBarH,
	  NULL
		},{
		/*Tile 30 - Block Left*/
		gfx->tiles.BlockLeft,
	  gfx->tiles.masks.solid
		},{
		/*Tile 31 - BlockRight*/
		gfx->tiles.BlockRight,
	  gfx->tiles.masks.solid
		},{
		/*Tile 32 - Tile 1 of Up Facing Door*/
		gfx->tiles.DoorOpenU1,
	  gfx->tiles.masks.slopeUp
		},{
		/*Tile 33 - Tile 2 of Up Facing Door*/
		gfx->tiles.DoorOpenU2,
	  gfx->tiles.masks.solid
		},{
		/*Tile 34 - Tile 3 of Up Facing Door*/
		gfx->tiles.DoorOpenU3,
	  gfx->tiles.masks.solid
		},{
		/*Tile 35 - Tile 4 of Up Facing Door*/
		gfx->tiles.DoorOpenU4,
	  gfx->tiles.masks.slopeDown
		},{
		/*Tile 36 - Door Base Vertical Piece 1*/
		gfx->tiles.DoorBaseV1,
	  NULL
		},{
		/*Tile 37 - Door Base Vertical Piece 2*/
		gfx->tiles.DoorBaseV2,
	  NULL
		},{
		/*Tile 38 - Door Base Vertical Piece 3*/
		gfx->tiles.DoorBaseV3,
	  NULL
		},{
		/*Tile 39 - Door Base Vertical Piece 4*/
		gfx->tiles.DoorBaseV4,
	  NULL
		},{
		/*Tile 40 - Tile 1 of Down Facing Door*/
		gfx->tiles.DoorOpenD1,
	  gfx->tiles.masks.roofDown
		},{
		/*Tile 41 - Tile 2 of Down Facing Door*/
		gfx->tiles.DoorOpenD2,
	  gfx->tiles.masks.solid
		},{
		/*Tile 42 - Tile 3 of Down Facing Door*/
		gfx->tiles.DoorOpenD3,
	  gfx->tiles.masks.solid
		},{
		/*Tile 43 - Tile 4 of Down Facing Door*/
		gfx->tiles.DoorOpenD4,
	  gfx->tiles.masks.roofUp
		},{
		/*Tile 44 - Pipe Piece 1*/
		gfx->tiles.pipe1,
	  gfx->tiles.masks.solid
		},{
		/*Tile 45 - Pipe Piece 2*/
		gfx->tiles.pipe2,
	  gfx->tiles.masks.solid
		},{
		/*Tile 46 - Spike 1*/
		gfx->tiles.Spikes1,
	  gfx->tiles.masks.solid
		},{
		/*Tile 47 - Spike 2*/
		gfx->tiles.Spikes2,
	  gfx->tiles.masks.solid
		},{
		/*Tile 48 - Corrugated Pipe H*/
		gfx->tiles.corrugatedPipeH,
	  gfx->tiles.masks.solid
		},{
		/*Tile 49 - Corrugated Pipe H Breakaway*/
		gfx->tiles.corrugatedPipeH,
	  gfx->tiles.masks.solid
		},{
		/*Tile 50 - Spike 1  Break Away*/
		gfx->tiles.Spikes1,
	  gfx->tiles.masks.solid
		},{
		/*Tile 51 - Spike 2 Break Away*/
		gfx->tiles.Spikes2,
	  gfx->tiles.masks.solid
		},{
		/*Tile 52 - Solid Break Away*/
		gfx->tiles.darkRocksSolid,
	  gfx->tiles.masks.solid
		},{
		/* Tile 53 - Rock Top Soil */
		gfx->tiles.darkRocksTop,
		NULL
		},{
		/* Tile 54 - Rock Bottom Soil */
		gfx->tiles.darkRocksBot,
		NULL
		},{
		/* Tile 55 - Rock Left Soil */
		gfx->tiles.darkRocksLeft,
		NULL
		},{
		/* Tile 56 - Rock Right Soil */
		gfx->tiles.darkRocksRight,
		NULL
		},{
		/* Tile 57 - Pickup Stand */
		gfx->tiles.stand,
		gfx->tiles.masks.solid
		},{
		/* Tile 58 - Missile Item Feeder */
		gfx->tiles.empty8x8,
		NULL
		},{
		/* Tile 59 - Super Missile Item Feeder */
		gfx->tiles.empty8x8,
		NULL
		},{
		/* Tile 60 - PowerBomb Item Feeder */
		gfx->tiles.empty8x8,
		NULL
		},{
		/* Tile 61 - Energy Tank Feeder */
		gfx->tiles.empty8x8,
		NULL
		},{
		/* Tile 62 - Invisble Solid for Three Frames */
		gfx->tiles.empty8x8,
		gfx->tiles.masks.solid
		},{
		/* Tile 63 - Invisble Solid for Two Frames */
		gfx->tiles.empty8x8,
		gfx->tiles.masks.solid
		},{
		/* Tile 64 - Invisble Solid for One Frames */
		gfx->tiles.empty8x8,
		gfx->tiles.masks.solid
		}
	};
	if((tiles = malloc(sizeof(t))) == NULL)
		exit(0);
	memcpy(tiles, t, sizeof(t));
}

void onShot(int x, int y, struct object* shot) {
	int i;
	switch(map->array[1-map->array[(y/8)*map->w]+(y/8)*map->w+x/8]) {
		case 9:
		case 10:// BOMB BLOCK
			if(shot->behavior == bombShot) {
				pushObject(x,y+8,0,0,&templates->effects.crumble);
				objects->first->value->timer = 80;
				objects->first->value->e1 = 9;
				map->array[1-map->array[(y/8)*map->w]+(y/8)*map->w+x/8] = 0;
			}/* else {
				map->array[1-map->array[(y/8)*map->w]+(y/8)*map->w+x/8] = 9;
			}*/
			break;
		case 11:
		case 12:
		case 13:
		case 14:
		case 19:
		case 20:
		case 21:
		case 22:
		case 32:
		case 33:
		case 34:
		case 35:
		case 40:
		case 41:
		case 42:
		case 43:// DOORS
			for(i = 0; i < map->d; i++) {
				if(map->doors[i].state > 0)
					continue;
				switch(map->doors[i].dir) {
					case 'l':
					case 'r':
						//pause('a');
						if(x/8 - map->array[(y/8)*map->w] == map->doors[i].x && y/8 >= map->doors[i].y && y/8 <= map->doors[i].y + 3)
							map->doors[i].state = 1;
						break;
					case 'u':
					case 'd':
						if(y/8 == map->doors[i].y && x/8 - map->array[(y/8)*map->w] >= map->doors[i].x && x/8 - map->array[(y/8)*map->w] <= map->doors[i].x + 3)
							map->doors[i].state = 1;
						break;
				}
				if(map->doors[i].state > 0)
					break;
			}
			break;
	}
}

void onDraw(int c, int r) {
	switch(map->array[1-map->array[r*map->w]+r*map->w+c]) {
		case 64:// MAKE ZERO
			map->array[1-map->array[(r)*map->w]+r*map->w+c] = 0;
			break;

		case 62:
		case 63:// INCREMENT
			map->array[1-map->array[(r)*map->w]+r*map->w+c]++;
			break;

		case 46:
		case 47:
		case 50:
		case 51:// SPIKE ANIMATION
			if(game->frame%5 == 0) {
				switch(map->array[1-map->array[(r)*map->w]+r*map->w+c]) {
					case 46:
					case 50:
						//pause('a');
						map->array[1-map->array[(r)*map->w]+r*map->w+c]++;
						break;
					case 51:
					case 47:
						map->array[1-map->array[(r)*map->w]+r*map->w+c]--;
						break;
				}
			}
			break;

		case 58:// MISSILE TANK
			map->array[1-map->array[(r)*map->w]+r*map->w+c] = 0;
			pushObject(c*8,(r+1)*8,0,0,&templates->items.missile);
			break;
		case 59:// SUPER MISSILE TANK
			map->array[1-map->array[(r)*map->w]+r*map->w+c] = 0;
			pushObject(c*8,(r+1)*8,0,0,&templates->items.superMissile);
			break;
		case 60:// POWERBOMB TANK
			map->array[1-map->array[(r)*map->w]+r*map->w+c] = 0;
			pushObject(c*8,(r+1)*8,0,0,&templates->items.powerBomb);
			break;
		case 61:// ENERGY TANK
			map->array[1-map->array[(r)*map->w]+r*map->w+c] = 0;
			pushObject(c*8,(r+1)*8,0,0,&templates->items.energy);
			break;
	}
}

int onCollide(char dir, int x, int y) {
	switch(map->array[1-map->array[(y/8)*map->w]+(y/8)*map->w+x/8]) {
		case 7:
		case 49:
		case 50:
		case 51:
		case 52://	BREAKAWAY
			if(dir == 't') {
				pushObject(x,y+8,0,0,&templates->effects.crumble);
				objects->first->value->timer = 17;
				objects->first->value->e1 = 7;
				map->array[1-map->array[(y/8)*map->w]+(y/8)*map->w+x/8] = 63;
			}
			break;
		case 46:
		case 47://  SPIKES
			if(dir == 't') {
				hurtSamus(30);
			}
			break;
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:// SPIDER BALL BLOCKS
			return 1;
			break;
	}
	return 0;
}
