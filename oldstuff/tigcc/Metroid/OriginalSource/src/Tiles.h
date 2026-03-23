// Header File
// Created 2/3/2005; 9:07:42 PM

/*********
	STRUCTS
 *********/
struct object;
struct tile {
  void *s;
 	const unsigned char *m;
 };
/************
	PROTOTYPES
 ************/
void onShot(int, int, struct object*);
void onDraw(int, int);
int onCollide(char, int, int);

void makeTiles(void);