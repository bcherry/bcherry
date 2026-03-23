/*
 * tiles.h - Tile struct and tile behavior for Metroid TI-89 SDL2 port
 * Ported from original TIGCC Tiles.h
 */
#ifndef TILES_H
#define TILES_H

#include <stdint.h>

/* Forward declaration */
struct object;

struct tile {
    const void *s;            /* Pointer to tile sprite data ([3][8] array) */
    const unsigned char *m;   /* Pointer to collision mask (or NULL) */
};

#define NUM_TILES 65

void makeTiles(void);
void onShot(int x, int y, struct object* shot);
void onDraw(int c, int r);
int onCollide(char dir, int x, int y);

#endif /* TILES_H */
