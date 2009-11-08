// Header File
// Created 1/27/2005; 8:17:47 PM

struct door {
	int x, y;
	char dir, state;
	int map_link, link_i;
};

struct map {
	int h, w, d, contrast, l, r, t, b;
	unsigned char array [1000];
	struct door doors[5];
};

struct cam {
	int x;
	int y;
	int hud;
};


#define BUFFER 2
#define CAMERA_MOVE 50

void makeMaps(void);
void drawMap(void);
void drawScreen(void);
void adjustCamera(void);
void loadMap(int, int, int, int);
void switchMaps(void);
void drawHud(void);