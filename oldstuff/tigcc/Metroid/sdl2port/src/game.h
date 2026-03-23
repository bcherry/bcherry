/*
 * game.h - Core game struct and globals for Metroid TI-89 SDL2 port
 * Ported from original TIGCC all.h
 */
#ifndef GAME_H
#define GAME_H

#include "map_data.h"
#include "gfx_data.h"

/* Forward declarations */
struct sprite;
struct sprites;
struct tile;
struct enemy;
struct enemyList;
struct object;
struct objlist;
struct templates;
struct samus;

/* Game state struct */
struct game {
    int (*mode)(void);  /* Current game mode (playingMode or mapTransition) */
    int frame;
    struct {
        char up;
        char down;
        char left;
        char right;
        char fire;
        char sprint;
        char jump;
        char aim;
        char f1, f2, f3, f4, f5;
    } keys;
};

/* Global pointers - matching original all.h naming */
extern struct samus* samus;
extern struct cam* cam;
extern struct game* game;
extern struct map* map;
extern struct graphics* gfx;
extern struct tile* tiles;
extern struct enemy* enemyKey;
extern struct sprites* sprites;
extern struct templates* templates;
extern struct enemyList* enemies;
extern struct objlist* objects;

/* Function declarations */
int game_init(void);
void game_cleanup(void);
void getKeys(void);

/* Game mode functions (declared here, defined elsewhere) */
int playingMode(void);
int mapTransition(void);

/* Utility functions from original */
int boundingBoxCollide(int, int, int, int, int, int, int, int, int);

#endif /* GAME_H */
