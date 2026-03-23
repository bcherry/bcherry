/*
 * pickups.h - Pickup item behaviors for Metroid TI-89 SDL2 port
 * Ported from original TIGCC pickups.c
 */
#ifndef PICKUPS_H
#define PICKUPS_H

struct object;

int pickup(struct object* obj);
void energyPickup(struct object* obj);
void missilePickup(struct object* obj);
void superMissilePickup(struct object* obj);
void powerBombPickup(struct object* obj);

#endif /* PICKUPS_H */
