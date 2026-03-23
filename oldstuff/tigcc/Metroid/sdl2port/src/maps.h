/*
 * maps.h - Map loading, drawing, and camera for Metroid TI-89 SDL2 port
 * Ported from original TIGCC Maps.h
 */
#ifndef MAPS_H
#define MAPS_H

#include "map_data.h"

/* Map functions */
void makeMaps(void);
void loadMap(int m, int d);
void drawMap(void);
void drawScreen(void);
void adjustCamera(void);
void clearDoor(int i);
void fillDoor(int i);
int checkDoors(void);
void drawDoors(void);

/* HUD */
void drawHud(void);

#endif /* MAPS_H */
