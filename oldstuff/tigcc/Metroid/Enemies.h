// Header File
// Created 2/10/2005; 4:17:24 PM

struct object;

struct enemy {
	int x, y;
	int h, w;
	int vel_x, vel_y;
	int extra1, extra2, extra3;
	int HP, ATK, E, M, SM, PB;
	char dead, hurt;
	int frames;
	struct sprite* sprt;
	int immunity;
	void (*behavior)(struct enemy*);
	void (*onDeath)(struct enemy*);
};

struct enemyNode {
	struct enemy* value;
	struct enemyNode* next;
};

struct enemyList {
	struct enemyNode* first;
};

void freeEnemies(void);
void drawEnemy(struct enemy*);
void pushEnemy(int, int, int);
void doEnemies(void);
void drawEnemies(void);
void makeEnemies(void);
void hurtEnemy(struct enemy*, int, int);
void spawnPickups(struct enemy*, int);

void simpleDeath(struct enemy*);