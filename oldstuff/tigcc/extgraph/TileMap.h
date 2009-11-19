// Tilemap Engine v0.4 (02/05/2004)
// Julien Richard-Foy


// Quelques constantes
#define BIG_VSCREEN_SIZE 5440
#define GRAY_BIG_VSCREEN_SIZE (5440*2)
#define BIG_VSCREEN_WIDTH 272
#define BIG_VSCREEN_HEIGHT 160

// Structures
typedef struct
{
    void *matrix;  // Matrix of tiles
    unsigned short width;  // Map's width
    void *sprites; // Sprite array
    char *big_vscreen;  // Big virtual screen
    long mask;  // Used with the BLIT mode
    long reserved;  // Internal utilisation
    short force_update; // To refresh the big_vscreen
}Plane;

typedef struct
{
    Plane p;
    void *tabanim; // Matrix of Animations
    short nb_anim; // Number of animations
    short nb_step; // Number of animation's steps
    short step; // Current step number
    short step_length; // Length of a step (in frames)
    short frame; // Current frame number of the current step
}AnimatedPlane;

//typedef void (*TM_Type)(short asm("%d2"),void* asm("%a0"),void* asm("%a1"),void* asm("%a2"));
//typedef void (*TM_AnimType)(short asm("%d2"),void* asm("%a0"),void* asm("%a1"),void* asm("%a2"),void* asm("%a3"));
//typedef void (*TM_TilesType)(short asm("%d2"),void* asm("%a0"),void* asm("%a1"),void* asm("%a2"));
typedef void (*TM_Mode)(void* asm("%a0"),short asm("%d0"),short asm("%d1"),void* asm("%a1"));


// Prototypes des fonctions de la librairie
// RefreshBuffer
void RefreshBuffer8B(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void RefreshBuffer8W(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void RefreshBuffer16B(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void RefreshBuffer16W(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));

void RefreshGrayBuffer8B(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void RefreshGrayBuffer8W(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void RefreshGrayBuffer16B(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void RefreshGrayBuffer16W(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));


// RefreshAnimatedBuffer
void RefreshAnimatedBuffer16B(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"),void *tabanim asm("%a3"));
void RefreshAnimatedBuffer16W(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"),void *tabanim asm("%a3"));
void RefreshAnimatedBuffer8B(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"),void *tabanim asm("%a3"));
void RefreshAnimatedBuffer8W(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"),void *tabanim asm("%a3"));

void RefreshGrayAnimatedBuffer16B(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"),void *tabanim asm("%a3"));
void RefreshGrayAnimatedBuffer16W(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"),void *tabanim asm("%a3"));
void RefreshGrayAnimatedBuffer8B(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"),void *tabanim asm("%a3"));
void RefreshGrayAnimatedBuffer8W(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"),void *tabanim asm("%a3"));


// DrawBuffer
void DrawBuffer_RPLC(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawBuffer_OR(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawBuffer_XOR(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawBuffer_AND(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawBuffer_MASK(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawBuffer_BLIT(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),long mask asm("%d2"),void *dest asm("%a1"));

void DrawBuffer89_RPLC(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawBuffer89_OR(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawBuffer89_XOR(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawBuffer89_AND(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawBuffer89_MASK(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawBuffer89_BLIT(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),long mask asm("%d2"),void *dest asm("%a1"));

void DrawGrayBuffer_RPLC(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawGrayBuffer_OR(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawGrayBuffer_XOR(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawGrayBuffer_AND(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawGrayBuffer_MASK(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawGrayBuffer_BLIT(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),long mask asm("%d2"),void *dest asm("%a1"));
void DrawGrayBuffer_TRANW(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawGrayBuffer_TRANB(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));

void DrawGrayBuffer89_RPLC(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawGrayBuffer89_OR(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawGrayBuffer89_XOR(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawGrayBuffer89_AND(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawGrayBuffer89_MASK(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawGrayBuffer89_BLIT(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),long mask asm("%d2"),void *dest asm("%a1"));
void DrawGrayBuffer89_TRANW(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));
void DrawGrayBuffer89_TRANB(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"));


// DrawBufferWithShifts
void DrawBufferWithShifts_RPLC(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawBufferWithShifts_OR(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawBufferWithShifts_XOR(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawBufferWithShifts_AND(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawBufferWithShifts_BLIT(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),long mask asm("%d2"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));

void DrawBufferWithShifts89_RPLC(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawBufferWithShifts89_OR(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawBufferWithShifts89_XOR(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawBufferWithShifts89_AND(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawBufferWithShifts89_BLIT(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),long mask asm("%d2"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));

void DrawGrayBufferWithShifts_RPLC(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawGrayBufferWithShifts_OR(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawGrayBufferWithShifts_XOR(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawGrayBufferWithShifts_AND(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawGrayBufferWithShifts_MASK(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawGrayBufferWithShifts_BLIT(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),long mask asm("%d2"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawGrayBufferWithShifts_TRANW(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawGrayBufferWithShifts_TRANB(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));

void DrawGrayBufferWithShifts89_RPLC(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawGrayBufferWithShifts89_OR(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawGrayBufferWithShifts89_XOR(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawGrayBufferWithShifts89_AND(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawGrayBufferWithShifts89_MASK(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawGrayBufferWithShifts89_BLIT(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),long mask asm("%d2"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawGrayBufferWithShifts89_TRANW(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));
void DrawGrayBufferWithShifts89_TRANB(void *src asm("%a0"),short x asm("%d0"),short y asm("%d1"),void *dest asm("%a1"),char *dh asm("%a2"),short *dv asm("%a3"));


// DrawTiles
void DrawTiles16B(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void DrawTiles16W(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void DrawTiles8B(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void DrawTiles8W(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));

void DrawTiles16B89(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void DrawTiles16W89(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void DrawTiles8B89(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void DrawTiles8W89(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));

void DrawGrayTiles16B(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void DrawGrayTiles16W(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void DrawGrayTiles8B(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void DrawGrayTiles8W(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));

void DrawGrayTiles16B89(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void DrawGrayTiles16W89(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void DrawGrayTiles8B89(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));
void DrawGrayTiles8W89(short larg asm("%d2"),void *tab asm("%a0"),void *dest asm("%a1"),void *tabsprt asm("%a2"));

#define DrawTiles(col,line,larg,tab,dest,tabsprt,type) type((larg),&((tab)[line][col]),(dest),(tabsprt))


// DrawPlane
void DrawPlane16W(short x asm("%d0"),short y asm("%d1"),Plane *plane asm("%a0"),void *dest asm("%a1"),TM_Mode mode asm("%a2"));
void DrawPlane16B(short x asm("%d0"),short y asm("%d1"),Plane *plane asm("%a0"),void *dest asm("%a1"),TM_Mode mode asm("%a2"));
void DrawPlane8W(short x asm("%d0"),short y asm("%d1"),Plane *plane asm("%a0"),void *dest asm("%a1"),TM_Mode mode asm("%a2"));
void DrawPlane8B(short x asm("%d0"),short y asm("%d1"),Plane *plane asm("%a0"),void *dest asm("%a1"),TM_Mode mode asm("%a2"));

void DrawGrayPlane16W(short x asm("%d0"),short y asm("%d1"),Plane *plane asm("%a0"),void *dest asm("%a1"),TM_Mode mode asm("%a2"));
void DrawGrayPlane16B(short x asm("%d0"),short y asm("%d1"),Plane *plane asm("%a0"),void *dest asm("%a1"),TM_Mode mode asm("%a2"));
void DrawGrayPlane8W(short x asm("%d0"),short y asm("%d1"),Plane *plane asm("%a0"),void *dest asm("%a1"),TM_Mode mode asm("%a2"));
void DrawGrayPlane8B(short x asm("%d0"),short y asm("%d1"),Plane *plane asm("%a0"),void *dest asm("%a1"),TM_Mode mode asm("%a2"));

#define DrawPlane(x,y,plane,dest,mode,type) type((x),(y),(plane),(dest),(mode))


// DrawAnimatedPlane
void DrawAnimatedPlane16W(short x asm("%d0"),short y asm("%d1"),AnimatedPlane *plane asm("%a0"),void *dest asm("%a1"),TM_Mode mode asm("%a2"));
void DrawAnimatedPlane16B(short x asm("%d0"),short y asm("%d1"),AnimatedPlane *plane asm("%a0"),void *dest asm("%a1"),TM_Mode mode asm("%a2"));
void DrawAnimatedPlane8W(short x asm("%d0"),short y asm("%d1"),AnimatedPlane *plane asm("%a0"),void *dest asm("%a1"),TM_Mode mode asm("%a2"));
void DrawAnimatedPlane8B(short x asm("%d0"),short y asm("%d1"),AnimatedPlane *plane asm("%a0"),void *dest asm("%a1"),TM_Mode mode asm("%a2"));

void DrawGrayAnimatedPlane16W(short x asm("%d0"),short y asm("%d1"),AnimatedPlane *plane asm("%a0"),void *dest asm("%a1"),TM_Mode mode asm("%a2"));
void DrawGrayAnimatedPlane16B(short x asm("%d0"),short y asm("%d1"),AnimatedPlane *plane asm("%a0"),void *dest asm("%a1"),TM_Mode mode asm("%a2"));
void DrawGrayAnimatedPlane8W(short x asm("%d0"),short y asm("%d1"),AnimatedPlane *plane asm("%a0"),void *dest asm("%a1"),TM_Mode mode asm("%a2"));
void DrawGrayAnimatedPlane8B(short x asm("%d0"),short y asm("%d1"),AnimatedPlane *plane asm("%a0"),void *dest asm("%a1"),TM_Mode mode asm("%a2"));

#define DrawAnimatedPlane(x,y,plane,dest,mode,type) type((x),(y),(plane),(dest),(mode))


// Macros
#define TM_8B DrawPlane8B
#define TM_8W DrawPlane8W
#define TM_16B DrawPlane16B
#define TM_16W DrawPlane16W

#define TM_G8B DrawGrayPlane8B
#define TM_G8W DrawGrayPlane8W
#define TM_G16B DrawGrayPlane16B
#define TM_G16W DrawGrayPlane16W


#define TM_A8B DrawAnimatedPlane8B
#define TM_A8W DrawAnimatedPlane8W
#define TM_A16B DrawAnimatedPlane16B
#define TM_A16W DrawAnimatedPlane16W

#define TM_GA8B DrawGrayAnimatedPlane8B
#define TM_GA8W DrawGrayAnimatedPlane8W
#define TM_GA16B DrawGrayAnimatedPlane16B
#define TM_GA16W DrawGrayAnimatedPlane16W


#define TM_T16B DrawTiles16B
#define TM_T16W DrawTiles16W
#define TM_T8B DrawTiles8B
#define TM_T8W DrawTiles8W

#define TM_T16B89 DrawTiles16B89
#define TM_T16W89 DrawTiles16W89
#define TM_T8B89 DrawTiles8B89
#define TM_T8W89 DrawTiles8W89

#define TM_GT16B DrawGrayTiles16B
#define TM_GT16W DrawGrayTiles16W
#define TM_GT8B DrawGrayTiles8B
#define TM_GT8W DrawGrayTiles8W

#define TM_GT16B89 DrawGrayTiles16B89
#define TM_GT16W89 DrawGrayTiles16W89
#define TM_GT8B89 DrawGrayTiles8B89
#define TM_GT8W89 DrawGrayTiles8W89


#define TM_RPLC DrawBuffer_RPLC
#define TM_OR DrawBuffer_OR
#define TM_XOR DrawBuffer_XOR
#define TM_AND DrawBuffer_AND
#define TM_MASK DrawBuffer_MASK
#define TM_BLIT (TM_Mode)DrawBuffer_BLIT

#define TM_RPLC89 DrawBuffer89_RPLC
#define TM_OR89 DrawBuffer89_OR
#define TM_XOR89 DrawBuffer89_XOR
#define TM_AND89 DrawBuffer89_AND
#define TM_MASK89 DrawBuffer89_MASK
#define TM_BLIT89 (TM_Mode)DrawBuffer89_BLIT

#define TM_GRPLC DrawGrayBuffer_RPLC
#define TM_GOR DrawGrayBuffer_OR
#define TM_GXOR DrawGrayBuffer_XOR
#define TM_GAND DrawGrayBuffer_AND
#define TM_GMASK DrawGrayBuffer_MASK
#define TM_GBLIT (TM_Mode)DrawGrayBuffer_BLIT
#define TM_GTRANW DrawGrayBuffer_TRANW
#define TM_GTRANB DrawGrayBuffer_TRANB

#define TM_GRPLC89 DrawGrayBuffer89_RPLC
#define TM_GOR89 DrawGrayBuffer89_OR
#define TM_GXOR89 DrawGrayBuffer89_XOR
#define TM_GAND89 DrawGrayBuffer89_AND
#define TM_GMASK89 DrawGrayBuffer89_MASK
#define TM_GBLIT89 (TM_Mode)DrawGrayBuffer89_BLIT
#define TM_GTRANW89 DrawGrayBuffer89_TRANW
#define TM_GTRANB89 DrawGrayBuffer89_TRANB
