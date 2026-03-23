/*
 * items.h - Major item pickup behaviors for Metroid TI-89 SDL2 port
 * Ported from original TIGCC Items.c
 */
#ifndef ITEMS_H
#define ITEMS_H

struct object;

void missileItem(struct object* obj);
void superMissileItem(struct object* obj);
void powerBombItem(struct object* obj);
void energyTankItem(struct object* obj);

#endif /* ITEMS_H */
