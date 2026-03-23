/*
 * map_data.h - Map data structures for Metroid TI-89 SDL2 port
 * Ported from original TIGCC Maps.h
 */
#ifndef MAP_DATA_H
#define MAP_DATA_H

#include <stdint.h>

#define NUM_MAPS 6
#define MAP_ARRAY_SIZE 1000
#define MAX_DOORS 5

#define BUFFER 2
#define CAMERA_MOVE 50

struct door {
    int x, y;
    char dir, state;
    int map_link, link_i;
};

struct map {
    int h, w, d, contrast, l, r, t, b;
    uint8_t array[MAP_ARRAY_SIZE];
    struct door doors[MAX_DOORS];
};

struct cam {
    int x;
    int y;
    int hud;
};

extern struct map maps[NUM_MAPS];

#endif /* MAP_DATA_H */
