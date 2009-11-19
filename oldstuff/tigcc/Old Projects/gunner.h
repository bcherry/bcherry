// C Header File
// Created 11/15/03; 5:37:48 PM

typedef struct {
	unsigned short int x : 8, y : 8;
} POSITION;

typedef struct {
	char done:1, draw:1/*, xchange:2, ychange:2*/, aae:1, aaf:1;
	char xchange;
	char ychange;
} BITS;

int game_friction = 50;
int game_speed = 30;

enum ArrowKeys {UP,DOWN,LEFT,RIGHT,SECOND,SHIFT,DIAMOND,ALPHA};
enum KeyMatrix {TI89_ESCROW = ~0x0040, TI89_ESCKEY = 0x0001, TI89_ENTEROW = 0b10, TI89_ENTER = 0b1, ARROW_ROW = ~0x0001};

inline void delay(unsigned int);
inline short int button(int,int);
inline void drawGunner(POSITION);
inline void title(void);
inline void CLRSCR(void);
atexit_t leaving(void);

unsigned short int gunmanLIGHT[]= {0x3018,0x4824,0x7038,0x7038,0x701C,0x701C,0x701C,0x76DC,0x7EFC,0x3FD0,0x2130,0x2844,0xE004,0x7B4C,0x1B28,0x04C0};
unsigned short int gunmanDARK[]= {0x0000,0x3018,0x381c,0x381c,0x3838,0x3838,0x3838,0x3838,0x3018,0x1008,0x1ec8,0x1498,0x1018,0x0430,0x04c0,0x0000};