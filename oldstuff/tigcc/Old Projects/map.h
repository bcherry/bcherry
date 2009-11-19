// C Header File
// Created 11/15/03; 5:37:48 PM

typedef struct {
	signed long int x : 16, y : 16;
} POSITION;

typedef struct {
	char one:1, xright:1, xleft:1, yup:1, ydown:1, draw:1, seven:1, eight:1;
} BITS;

unsigned char box[]   = {0xFF,0xE7,0xFF,0xBD,0xBD,0xFF,0xE7,0xFF};

enum ArrowKeys {UP,DOWN,LEFT,RIGHT,SECOND,SHIFT,DIAMOND,ALPHA};
enum KeyMatrix {TI89_ESCROW = ~0x0040, TI89_ESCKEY = 0x0001, TI89_ENTEROW = 0b10, TI89_ENTER = 0b1, ARROW_ROW = ~0x0001};

inline void delay(unsigned int);
inline short int button(int,int);
inline void title(void);

unsigned char thing[]= {0x3C,0x7E,0xFF,0xFF,0xFF,0xFF,0x7E,0x3C};