/*
 * tiles.c - Tile initialization and behavior for Metroid TI-89 SDL2 port
 * Ported from original TIGCC Tiles.c
 *
 * All 65 tile definitions and all behavior switch statements
 * (onShot, onDraw, onCollide) have been ported from the original.
 */
#include <stdlib.h>
#include <string.h>

#include "tiles.h"
#include "gfx_data.h"
#include "map_data.h"

/* These are declared in game.c */
extern struct graphics* gfx;
extern struct tile* tiles;
extern struct map* map;

/* Forward declarations for game struct and object functions */
struct game;
extern struct game* game;

struct templates;
extern struct templates* templates;

struct objlist;
extern struct objlist* objects;

struct samus;
extern struct samus* samus;

/* Access game->frame - we include the struct definition here to avoid circular deps */
struct game {
    int (*mode)(void);
    int frame;
    struct {
        char up, down, left, right;
        char fire, sprint, jump, aim;
        char f1, f2, f3, f4, f5;
    } keys;
};

/* External functions from other modules (to be implemented) */
extern void pushObject(int, int, int, int, struct object*);
extern void hurtSamus(int);

/* Object struct needed for onShot parameter and behavior comparison */
struct object {
    int x, y;
    int h, w;
    int vel_x, vel_y;
    int ATK, timer;
    int e1, e2, e3;
    int type;
    int frames, gone;
    struct sprite* sprt;
    void (*behavior)(struct object*);
};

struct objnode {
    struct object* value;
    struct objnode* link;
};

struct objlist {
    struct objnode* first;
};

/* Object templates struct - needed to reference templates->effects.crumble etc. */
struct templates {
    struct {
        struct object powerBeam, missile, superMissile, bomb, powerBomb;
    } weapons;
    struct {
        struct object bombExplosion;
        struct object crumble;
        struct object simpleEnemyExplosion;
    } effects;
    struct {
        struct object energy;
        struct object missile;
        struct object superMissile;
        struct object powerBomb;
    } pickups;
    struct {
        struct object missile;
        struct object superMissile;
        struct object powerBomb;
        struct object energy;
    } items;
};

/* bombShot behavior function - declared extern, defined in objects module */
extern void bombShot(struct object*);

void makeTiles(void) {
    struct tile t[] = {
        /* Tile 0 - Empty */
        {gfx_data.tiles.empty8x8, NULL},
        /* Tile 1 - Solid */
        {gfx_data.tiles.darkRocksSolid, gfx_data.tiles.masks.solid},
        /* Tile 2 - Slope Up */
        {gfx_data.tiles.darkRocksUp, gfx_data.tiles.masks.slopeUp},
        /* Tile 3 - Slope Down */
        {gfx_data.tiles.darkRocksDown, gfx_data.tiles.masks.slopeDown},
        /* Tile 4 - Roof Up */
        {gfx_data.tiles.darkRocksRUp, gfx_data.tiles.masks.roofUp},
        /* Tile 5 - Roof Down */
        {gfx_data.tiles.darkRocksRDown, gfx_data.tiles.masks.roofDown},
        /* Tile 6 - Solid Pass Thru */
        {gfx_data.tiles.darkRocksSolid, NULL},
        /* Tile 7 - Breakaway */
        {gfx_data.tiles.breakAway, gfx_data.tiles.masks.solid},
        /* Tile 8 - OPEN (unused solid) */
        {gfx_data.tiles.empty8x8, gfx_data.tiles.masks.solid},
        /* Tile 9 - Bomb Broken */
        {gfx_data.tiles.bombBreak, gfx_data.tiles.masks.solid},
        /* Tile 10 - Bomb break looks like Tile 1 */
        {gfx_data.tiles.darkRocksSolid, gfx_data.tiles.masks.solid},
        /* Tile 11 - Top of Left Facing Door */
        {gfx_data.tiles.DoorOpenL1, gfx_data.tiles.masks.slopeUp},
        /* Tile 12 - Top Middle of Left Facing Door */
        {gfx_data.tiles.DoorOpenL2, gfx_data.tiles.masks.solid},
        /* Tile 13 - Bottom Middle of Left Facing Door */
        {gfx_data.tiles.DoorOpenL3, gfx_data.tiles.masks.solid},
        /* Tile 14 - Bottom of Left Facing Door */
        {gfx_data.tiles.DoorOpenL4, gfx_data.tiles.masks.roofDown},
        /* Tile 15 - Door Base Horizontal Piece 1 */
        {gfx_data.tiles.DoorBaseH1, NULL},
        /* Tile 16 - Door Base Horizontal Piece 2 */
        {gfx_data.tiles.DoorBaseH2, NULL},
        /* Tile 17 - Door Base Horizontal Piece 3 */
        {gfx_data.tiles.DoorBaseH3, NULL},
        /* Tile 18 - Door Base Horizontal Piece 4 */
        {gfx_data.tiles.DoorBaseH4, NULL},
        /* Tile 19 - Top of Right Facing Door */
        {gfx_data.tiles.DoorOpenR1, gfx_data.tiles.masks.slopeDown},
        /* Tile 20 - Top Middle of Right Facing Door */
        {gfx_data.tiles.DoorOpenR2, gfx_data.tiles.masks.solid},
        /* Tile 21 - Bottom Middle of Right Facing Door */
        {gfx_data.tiles.DoorOpenR3, gfx_data.tiles.masks.solid},
        /* Tile 22 - Bottom of Right Facing Door */
        {gfx_data.tiles.DoorOpenR4, gfx_data.tiles.masks.roofUp},
        /* Tile 23 - Grassy Bottom */
        {gfx_data.tiles.grassytop, gfx_data.tiles.masks.solid},
        /* Tile 24 - Rocky Bottom */
        {gfx_data.tiles.rockybottom, gfx_data.tiles.masks.solid},
        /* Tile 25 - Single Floor */
        {gfx_data.tiles.SingleFloor, gfx_data.tiles.masks.solid},
        /* Tile 26 - Metal Corner */
        {gfx_data.tiles.MetalCorner, gfx_data.tiles.masks.solid},
        /* Tile 27 - Metal Bar H */
        {gfx_data.tiles.MetalBarH, gfx_data.tiles.masks.solid},
        /* Tile 28 - Metal Bar V */
        {gfx_data.tiles.MetalBarV, gfx_data.tiles.masks.solid},
        /* Tile 29 - Metal Bar H pass-thru */
        {gfx_data.tiles.MetalBarH, NULL},
        /* Tile 30 - Block Left */
        {gfx_data.tiles.BlockLeft, gfx_data.tiles.masks.solid},
        /* Tile 31 - Block Right */
        {gfx_data.tiles.BlockRight, gfx_data.tiles.masks.solid},
        /* Tile 32 - Tile 1 of Up Facing Door */
        {gfx_data.tiles.DoorOpenU1, gfx_data.tiles.masks.slopeUp},
        /* Tile 33 - Tile 2 of Up Facing Door */
        {gfx_data.tiles.DoorOpenU2, gfx_data.tiles.masks.solid},
        /* Tile 34 - Tile 3 of Up Facing Door */
        {gfx_data.tiles.DoorOpenU3, gfx_data.tiles.masks.solid},
        /* Tile 35 - Tile 4 of Up Facing Door */
        {gfx_data.tiles.DoorOpenU4, gfx_data.tiles.masks.slopeDown},
        /* Tile 36 - Door Base Vertical Piece 1 */
        {gfx_data.tiles.DoorBaseV1, NULL},
        /* Tile 37 - Door Base Vertical Piece 2 */
        {gfx_data.tiles.DoorBaseV2, NULL},
        /* Tile 38 - Door Base Vertical Piece 3 */
        {gfx_data.tiles.DoorBaseV3, NULL},
        /* Tile 39 - Door Base Vertical Piece 4 */
        {gfx_data.tiles.DoorBaseV4, NULL},
        /* Tile 40 - Tile 1 of Down Facing Door */
        {gfx_data.tiles.DoorOpenD1, gfx_data.tiles.masks.roofDown},
        /* Tile 41 - Tile 2 of Down Facing Door */
        {gfx_data.tiles.DoorOpenD2, gfx_data.tiles.masks.solid},
        /* Tile 42 - Tile 3 of Down Facing Door */
        {gfx_data.tiles.DoorOpenD3, gfx_data.tiles.masks.solid},
        /* Tile 43 - Tile 4 of Down Facing Door */
        {gfx_data.tiles.DoorOpenD4, gfx_data.tiles.masks.roofUp},
        /* Tile 44 - Pipe Piece 1 */
        {gfx_data.tiles.pipe1, gfx_data.tiles.masks.solid},
        /* Tile 45 - Pipe Piece 2 */
        {gfx_data.tiles.pipe2, gfx_data.tiles.masks.solid},
        /* Tile 46 - Spike 1 */
        {gfx_data.tiles.Spikes1, gfx_data.tiles.masks.solid},
        /* Tile 47 - Spike 2 */
        {gfx_data.tiles.Spikes2, gfx_data.tiles.masks.solid},
        /* Tile 48 - Corrugated Pipe H */
        {gfx_data.tiles.corrugatedPipeH, gfx_data.tiles.masks.solid},
        /* Tile 49 - Corrugated Pipe H Breakaway */
        {gfx_data.tiles.corrugatedPipeH, gfx_data.tiles.masks.solid},
        /* Tile 50 - Spike 1 Break Away */
        {gfx_data.tiles.Spikes1, gfx_data.tiles.masks.solid},
        /* Tile 51 - Spike 2 Break Away */
        {gfx_data.tiles.Spikes2, gfx_data.tiles.masks.solid},
        /* Tile 52 - Solid Break Away */
        {gfx_data.tiles.darkRocksSolid, gfx_data.tiles.masks.solid},
        /* Tile 53 - Rock Top Soil */
        {gfx_data.tiles.darkRocksTop, NULL},
        /* Tile 54 - Rock Bottom Soil */
        {gfx_data.tiles.darkRocksBot, NULL},
        /* Tile 55 - Rock Left Soil */
        {gfx_data.tiles.darkRocksLeft, NULL},
        /* Tile 56 - Rock Right Soil */
        {gfx_data.tiles.darkRocksRight, NULL},
        /* Tile 57 - Pickup Stand */
        {gfx_data.tiles.stand, gfx_data.tiles.masks.solid},
        /* Tile 58 - Missile Item Feeder */
        {gfx_data.tiles.empty8x8, NULL},
        /* Tile 59 - Super Missile Item Feeder */
        {gfx_data.tiles.empty8x8, NULL},
        /* Tile 60 - PowerBomb Item Feeder */
        {gfx_data.tiles.empty8x8, NULL},
        /* Tile 61 - Energy Tank Feeder */
        {gfx_data.tiles.empty8x8, NULL},
        /* Tile 62 - Invisible Solid for Three Frames */
        {gfx_data.tiles.empty8x8, gfx_data.tiles.masks.solid},
        /* Tile 63 - Invisible Solid for Two Frames */
        {gfx_data.tiles.empty8x8, gfx_data.tiles.masks.solid},
        /* Tile 64 - Invisible Solid for One Frame */
        {gfx_data.tiles.empty8x8, gfx_data.tiles.masks.solid}
    };

    tiles = malloc(sizeof(t));
    if (!tiles) return;
    memcpy(tiles, t, sizeof(t));
}

/*
 * Helper macro: get tile index at map position (c, r) using the original
 * scrolling map indirection: map->array[1 - map->array[r*map->w] + r*map->w + c]
 * The first element of each row stores the scroll offset.
 */
#define TILE_AT(c, r) (map->array[1 - map->array[(r) * map->w] + (r) * map->w + (c)])

void onShot(int x, int y, struct object* shot) {
    int i;
    int tile_idx = TILE_AT(x / 8, y / 8);

    switch (tile_idx) {
        case 9:
        case 10: /* BOMB BLOCK */
            if (shot->behavior == bombShot) {
                pushObject(x, y + 8, 0, 0, &templates->effects.crumble);
                objects->first->value->timer = 80;
                objects->first->value->e1 = 9;
                TILE_AT(x / 8, y / 8) = 0;
            }
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
        case 43: /* DOORS */
            for (i = 0; i < map->d; i++) {
                if (map->doors[i].state > 0)
                    continue;
                switch (map->doors[i].dir) {
                    case 'l':
                    case 'r':
                        if (x / 8 - map->array[(y / 8) * map->w] == map->doors[i].x &&
                            y / 8 >= map->doors[i].y && y / 8 <= map->doors[i].y + 3)
                            map->doors[i].state = 1;
                        break;
                    case 'u':
                    case 'd':
                        if (y / 8 == map->doors[i].y &&
                            x / 8 - map->array[(y / 8) * map->w] >= map->doors[i].x &&
                            x / 8 - map->array[(y / 8) * map->w] <= map->doors[i].x + 3)
                            map->doors[i].state = 1;
                        break;
                }
                if (map->doors[i].state > 0)
                    break;
            }
            break;
    }
}

void onDraw(int c, int r) {
    int tile_idx = TILE_AT(c, r);

    switch (tile_idx) {
        case 64: /* MAKE ZERO */
            TILE_AT(c, r) = 0;
            break;

        case 62:
        case 63: /* INCREMENT (invisible solid countdown) */
            TILE_AT(c, r) = tile_idx + 1;
            break;

        case 46:
        case 47:
        case 50:
        case 51: /* SPIKE ANIMATION */
            if (game->frame % 5 == 0) {
                int current = TILE_AT(c, r);
                switch (current) {
                    case 46:
                    case 50:
                        TILE_AT(c, r) = current + 1;
                        break;
                    case 51:
                    case 47:
                        TILE_AT(c, r) = current - 1;
                        break;
                }
            }
            break;

        case 58: /* MISSILE TANK */
            TILE_AT(c, r) = 0;
            pushObject(c * 8, (r + 1) * 8, 0, 0, &templates->items.missile);
            break;
        case 59: /* SUPER MISSILE TANK */
            TILE_AT(c, r) = 0;
            pushObject(c * 8, (r + 1) * 8, 0, 0, &templates->items.superMissile);
            break;
        case 60: /* POWERBOMB TANK */
            TILE_AT(c, r) = 0;
            pushObject(c * 8, (r + 1) * 8, 0, 0, &templates->items.powerBomb);
            break;
        case 61: /* ENERGY TANK */
            TILE_AT(c, r) = 0;
            pushObject(c * 8, (r + 1) * 8, 0, 0, &templates->items.energy);
            break;
    }
}

int onCollide(char dir, int x, int y) {
    int tile_idx = TILE_AT(x / 8, y / 8);

    switch (tile_idx) {
        case 7:
        case 49:
        case 50:
        case 51:
        case 52: /* BREAKAWAY */
            if (dir == 't') {
                pushObject(x, y + 8, 0, 0, &templates->effects.crumble);
                objects->first->value->timer = 17;
                objects->first->value->e1 = 7;
                TILE_AT(x / 8, y / 8) = 63;
            }
            break;

        case 46:
        case 47: /* SPIKES */
            if (dir == 't') {
                hurtSamus(30);
            }
            break;

        case 1:
        case 2:
        case 3:
        case 4:
        case 5: /* SPIDER BALL BLOCKS */
            return 1;
            break;
    }
    return 0;
}
