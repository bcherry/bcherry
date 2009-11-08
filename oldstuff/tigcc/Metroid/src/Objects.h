// Header File
// Created 3/21/2005; 10:22:37 PM

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

struct templates {
	struct {
		struct object powerBeam, /*spazer, waveBeam, plasmaBeam, iceBeam;
		struct object spazerWave, spazerIce, waveIce, wavePlasma, plasmaIce;
		struct object spazerWaveIce, wavePlasmaIce;
		struct object */missile, superMissile,/*
		struct object */bomb, powerBomb;
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
void crumblingFX(struct object*);
void bombExplodeFX(struct object*);
void simpleEnemyExplosionFX(struct object*);

void basicShot(struct object*);
void bombShot(struct object*);
void missileShot(struct object*);

int pickup(struct object*);
void energyPickup(struct object*);
void missilePickup(struct object*);
void superMissilePickup(struct object*);
void powerBombPickup(struct object*);

void missileItem(struct object*);
void superMissileItem(struct object*);
void powerBombItem(struct object*);
void energyTankItem(struct object*);

void freeObjects(void);
void makeObjects(void);
void doObjects(void);
void drawObjects(void);
void drawObject(struct object*);
void pushObject(int, int, int, int, struct object*);