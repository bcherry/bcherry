/******************************************************************************
*
* E X T G R A P H    v 1 . 0 2               (c) 2001,2002 TiCT (TI-Chess Team)
*
*******************************************************************************
*
* ExtGraph is a compile-time library which contains speed optimized graphics
* routines for the TIGCC Development Enviroment for TI-89 and TI-92plus.
*
*
* library maintained by: Thomas Nussbaumer (thomas.nussbaumer@gmx.net)
*                        Lionel Debroux    (lionel_debroux@yahoo.fr)
*
*
*******************************************************************************
* [internal version] $Id: extgraph.h,v 1.13 2002/05/22 09:19:20 tnussb Exp $
*******************************************************************************/

#if !defined(__EXTGRAPH__)
#define __EXTGRAPH__

#include <gray.h>   // necessary for GetPlane() macros in the grayscale support
                    // routines !!

//-----------------------------------------------------------------------------
// support for -mregparm switch in TIGCC 0.94
//-----------------------------------------------------------------------------
#include <version.h>

#if (defined(__TIGCC__)&&(__TIGCC__>0))||(defined(__TIGCC_MINOR__)&&(__TIGCC_MINOR__>93))
#define EXTGRAPH_FUNC_ATTRIBUTES  __attribute__((__stkparm__))
#else
#define EXTGRAPH_FUNC_ATTRIBUTES
#endif


//-----------------------------------------------------------------------------
// to embed the extgraph version as string into your program the extgraph
// library contains the following global string variable which SHOULD be used
// for this!
//
// the string looks like this: "ExtGraph vX.YY" where X is the major version
// number and YY is the subversion number.
//
// EXTGRAPH_VERSION_PWDSTR looks like this: "powered by ExtGraph vX.YY"
//-----------------------------------------------------------------------------
#define EXTGRAPH_VERSION_STR      __egvs__
#define EXTGRAPH_VERSION_PWDSTR   __egvpwds__
extern char __egvs__[];
extern char __egvpwds__[];


//-----------------------------------------------------------------------------
// global variables embedded in the library are used for the version numbers
// to garantee that the version is reported correctly even if you use an
// outdated headerfile
//-----------------------------------------------------------------------------
#define EXTGRAPH_VERSION_MAIN    __egvmain__
#define EXTGRAPH_VERSION_SUB     __egvsub__
extern short __egvmain__;
extern short __egvsub__;


//-----------------------------------------------------------------------------
// useful macros which may help to protect a program from running on a wrong
// calctype
//
// usage:
//
// if (!CheckHWMatch()) {
//     ST_helpMsg("ERROR: can only run on a "DESIRED_CALCTYPE);
//     return;
// }
//-----------------------------------------------------------------------------
#define __TI89CHECK__  ((((void*)((*(unsigned long*)0xC8)&0x600000))==(void*)0x400000)==0)

#if defined(USE_TI89) && !defined(USE_TI92P)

#define CheckHWMatch()    (__TI89CHECK__ ? 1 : 0)
#define DESIRED_CALCTYPE  "TI89"

#elif !defined(USE_TI89) && defined(USE_TI92P)

#define CheckHWMatch()  (__TI89CHECK__ ? 0 : 1)
#define DESIRED_CALCTYPE  "TI92+"

#else

#define CheckHWMatch()    (1)
#define DESIRED_CALCTYPE  "TI89/92+"

#endif

// macro which returns the absolute value of a given short
#define EXT_SHORTABS(a)  ({register short ta=(a); (ta>=0) ? ta : -ta;})

// macro which exchanges the content of two variables using the ASM exg instruction
#define EXT_XCHG(a,b)    asm volatile ("exg %0,%1" : "=d" (a), "=d" (b) : "0" (a), "1" (b) : "cc")

// macro which returns the word swapped value (upper and lower word swapped) of the given long
#define EXT_LONGSWAP(val) ({register unsigned long tmp = val;asm volatile ("swap %0" : "=d" (tmp) : "0" (tmp));tmp;})

// macro which checks two bounding rectangles starting at (x0/y0) and (x1/y1) for
// collision. w is the width in pixels and h the height of the two bounding rectangles
#define BOUNDS_COLLIDE(x0,y0,x1,y1,w,h) \
   ((EXT_SHORTABS((x1)-(x0))<(w))&&(EXT_SHORTABS((y1)-(y0))<(h)))

// handy aliases for standard tile sizes (8x8 / 16x16 / 32x32)
#define BOUNDS_COLLIDE8(x0,y0,x1,y1)  BOUNDS_COLLIDE(x0,y0,x1,y1,8,8)
#define BOUNDS_COLLIDE16(x0,y0,x1,y1) BOUNDS_COLLIDE(x0,y0,x1,y1,16,16)
#define BOUNDS_COLLIDE32(x0,y0,x1,y1) BOUNDS_COLLIDE(x0,y0,x1,y1,32,32)

// checks for collision between 2 sprites of width 8
short TestCollide8(short x0,short y0,short x1,short y1,short height,unsigned char* data0, unsigned char* data1) EXTGRAPH_FUNC_ATTRIBUTES;

// checks for collision between 2 sprites of width 16
short TestCollide16(short x0,short y0,short x1,short y1,short height,unsigned short* data0, unsigned short* data1) EXTGRAPH_FUNC_ATTRIBUTES;


//-----------------------------------------------------------------------------
// enums used by some extgraph functions
//-----------------------------------------------------------------------------
enum GrayColors {COLOR_WHITE=1,COLOR_LIGHTGRAY=2,COLOR_DARKGRAY=4,COLOR_BLACK=8};
enum FillAttrs  {RECT_EMPTY=0,RECT_FILLED};
enum ExtAttrs   {A_CENTERED=0x40,A_SHADOWED=0x80};


//-----------------------------------------------------------------------------
// speedy pixel access routines
//-----------------------------------------------------------------------------
#define EXT_PIXOFFSET(x,y)  (((y)<<5)-((y)<<1)+((x)>>3))
#define EXT_PIXADDR(p,x,y)  (((unsigned char*)(p))+EXT_PIXOFFSET(x,y))
#define EXT_PIXMASK(x)      ((unsigned char)(0x80 >> ((x)&7)))

#define EXT_SETPIX_AM(a,m)   (*(a) |= (m))
#define EXT_CLRPIX_AM(a,m)   (*(a) &= ~(m))
#define EXT_XORPIX_AM(a,m)   (*(a) ^= (m))
#define EXT_GETPIX_AM(a,m)   (*(a) & (m))

#define EXT_PIXUP_AM(a)      ((a)-=30)
#define EXT_PIXDOWN_AM(a)    ((a)+=30)
#define EXT_PIXLEFT_AM(a,m)   asm("rol.b  #1,%0;bcc.s  0f;subq.l #1,%1;0:"\
                                  : "=d" (m), "=g" (a) : "0" (m), "1" (a))
#define EXT_PIXRIGHT_AM(a,m)  asm("ror.b  #1,%0;bcc.s  0f;addq.l #1,%1;0:"\
                                  : "=d" (m), "=g" (a) : "0" (m), "1" (a))

#define EXT_SETPIX(p,x,y)   EXT_SETPIX_AM(EXT_PIXADDR(p,x,y),EXT_PIXMASK(x))
#define EXT_CLRPIX(p,x,y)   EXT_CLRPIX_AM(EXT_PIXADDR(p,x,y),EXT_PIXMASK(x))
#define EXT_XORPIX(p,x,y)   EXT_XORPIX_AM(EXT_PIXADDR(p,x,y),EXT_PIXMASK(x))
#define EXT_GETPIX(p,x,y)   EXT_GETPIX_AM(EXT_PIXADDR(p,x,y),EXT_PIXMASK(x))


//-----------------------------------------------------------------------------
// double buffer variants of grayscale support routines
// NOTE: graph functions modifies active plane with PortSet() !!
//-----------------------------------------------------------------------------
extern void ClearGrayScreen2B(void* lightplane,void* darkplane) EXTGRAPH_FUNC_ATTRIBUTES;
extern void DrawGrayRect2B(short x0,short y0,short x1, short y1, short color,short fill,void* lightplane,void* darkplane) EXTGRAPH_FUNC_ATTRIBUTES;
extern void InvertGrayRect2B(short x0,short y0,short x1, short y1,void* lightplane,void* darkplane) EXTGRAPH_FUNC_ATTRIBUTES;
extern void DrawGrayLine2B(short x0,short y0,short x1, short y1, short color,void* lightplane,void* darkplane) EXTGRAPH_FUNC_ATTRIBUTES;
extern void FastDrawGrayLine2B(short x0,short y0,short x1, short y1, short color,void* lightplane,void* darkplane) EXTGRAPH_FUNC_ATTRIBUTES;
extern void FastDrawGrayHLine2B(short x0,short x1, short y, short color,void* lightplane,void* darkplane) EXTGRAPH_FUNC_ATTRIBUTES;
extern void DrawGrayChar2B(short x,short y,char c,short attr,void* lightplane,void* darkplane) EXTGRAPH_FUNC_ATTRIBUTES;
extern void DrawGrayStr2B(short x,short y,char* s,short attr,void* lightplane,void* darkplane) EXTGRAPH_FUNC_ATTRIBUTES;
extern void DrawGrayStrExt2B(short x,short y,char* s,short attr,short font,void* lightplane,void* darkplane) EXTGRAPH_FUNC_ATTRIBUTES;


//-----------------------------------------------------------------------------
// NOTE: graph functions modifies active plane with PortSet() !!
//-----------------------------------------------------------------------------
#define ClearGrayScreen()                    ClearGrayScreen2B(GetPlane(LIGHT_PLANE),GetPlane(DARK_PLANE))
#define DrawGrayRect(x0,y0,x1,y1,color,fill) DrawGrayRect2B(x0,y0,x1,y1,color,fill,GetPlane(LIGHT_PLANE),GetPlane(DARK_PLANE))
#define InvertGrayRect(x0,y0,x1,y1)          InvertGrayRect2B(x0,y0,x1,y1,GetPlane(LIGHT_PLANE),GetPlane(DARK_PLANE))
#define DrawGrayLine(x0,y0,x1,y1,color)      DrawGrayLine2B(x0,y0,x1,y1,color,GetPlane(LIGHT_PLANE),GetPlane(DARK_PLANE))
#define FastDrawGrayHLine(x0,x1,y,color)     FastDrawGrayHLine2B(x0,x1,y,color,GetPlane(LIGHT_PLANE),GetPlane(DARK_PLANE))
#define FastDrawGrayLine(x0,y0,x1,y1,color)  FastDrawGrayLine2B(x0,y0,x1,y1,color,GetPlane(LIGHT_PLANE),GetPlane(DARK_PLANE))

#define DrawGrayChar(x,y,c,attr)             DrawGrayChar2B(x,y,c,attr,GetPlane(LIGHT_PLANE),GetPlane(DARK_PLANE))
#define DrawGrayStr(x,y,s,attr)              DrawGrayStr2B(x,y,s,attr,GetPlane(LIGHT_PLANE),GetPlane(DARK_PLANE))
#define DrawGrayStrExt(x,y,s,attr,font)      DrawGrayStrExt2B(x,y,s,attr,font,GetPlane(LIGHT_PLANE),GetPlane(DARK_PLANE))


//-----------------------------------------------------------------------------
// screen scrolling routines
//-----------------------------------------------------------------------------
extern void ScrollLeft160(unsigned short* buffer,unsigned short lines) EXTGRAPH_FUNC_ATTRIBUTES;
extern void ScrollLeft240(unsigned short* buffer,unsigned short lines) EXTGRAPH_FUNC_ATTRIBUTES;
extern void ScrollRight160(unsigned short* buffer,unsigned short lines) EXTGRAPH_FUNC_ATTRIBUTES;
extern void ScrollRight240(unsigned short* buffer,unsigned short lines) EXTGRAPH_FUNC_ATTRIBUTES;
extern void ScrollUp160(unsigned short* buffer,unsigned short lines) EXTGRAPH_FUNC_ATTRIBUTES;
extern void ScrollUp240(unsigned short* buffer,unsigned short lines) EXTGRAPH_FUNC_ATTRIBUTES;
extern void ScrollDown160(unsigned short* buffer,unsigned short lines) EXTGRAPH_FUNC_ATTRIBUTES;
extern void ScrollDown240(unsigned short* buffer,unsigned short lines) EXTGRAPH_FUNC_ATTRIBUTES;


//-----------------------------------------------------------------------------
// fast copy routine for a complete screen (240x128 pixels == 3840 bytes)
//-----------------------------------------------------------------------------
extern void FastCopyScreen(void* src,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;


//-----------------------------------------------------------------------------
// fast alternative functions for line drawing
//-----------------------------------------------------------------------------
extern void FastDrawHLine(unsigned char* plane,short x1,short x2,short y,short mode) EXTGRAPH_FUNC_ATTRIBUTES;
extern void FastDrawVLine(unsigned char* plane,short x,short y1,short y2,short mode) EXTGRAPH_FUNC_ATTRIBUTES;

extern void FastDrawLine(unsigned char* plane,short x1,short y1,short x2,short y2,short mode) EXTGRAPH_FUNC_ATTRIBUTES;


//-----------------------------------------------------------------------------
// fast alternative functions for SpriteX() functions
//-----------------------------------------------------------------------------
extern void  Sprite8_OR(short x,short y,short h,unsigned char* sprite,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void Sprite16_OR(short x,short y,short h,unsigned short* sprite,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void Sprite32_OR(short x,short y,short h,unsigned long* sprite,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void SpriteX8_OR(short x,short y,short h,unsigned char* sprite,short bytewidth,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void  GraySprite8_OR(short x,short y,short h,unsigned char* sprite1,unsigned char* sprite2,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;
extern void GraySprite16_OR(short x,short y,short h,unsigned short* sprite1,unsigned short* sprite2,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;
extern void GraySprite32_OR(short x,short y,short h,unsigned long* sprite1,unsigned long* sprite2,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;
extern void GraySpriteX8_OR(short x,short y,short h,unsigned char* sprite1,unsigned char* sprite2,short bytewidth,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;

extern void  Sprite8_AND(short x,short y,short h,unsigned char* sprite,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void Sprite16_AND(short x,short y,short h,unsigned short* sprite,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void Sprite32_AND(short x,short y,short h,unsigned long* sprite,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void SpriteX8_AND(short x,short y,short h,unsigned char* sprite,short bytewidth,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void  GraySprite8_AND(short x,short y,short h,unsigned char* sprite1,unsigned char* sprite2,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;
extern void GraySprite16_AND(short x,short y,short h,unsigned short* sprite1,unsigned short* sprite2,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;
extern void GraySprite32_AND(short x,short y,short h,unsigned long* sprite1,unsigned long* sprite2,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;
extern void GraySpriteX8_AND(short x,short y,short h,unsigned char* sprite1,unsigned char* sprite2,short bytewidth,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;

extern void  Sprite8_XOR(short x,short y,short h,unsigned char* sprite,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void Sprite16_XOR(short x,short y,short h,unsigned short* sprite,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void Sprite32_XOR(short x,short y,short h,unsigned long* sprite,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void SpriteX8_XOR(short x,short y,short h,unsigned char* sprite,short bytewidth,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void  GraySprite8_XOR(short x,short y,short h,unsigned char* sprite1,unsigned char* sprite2,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;
extern void GraySprite16_XOR(short x,short y,short h,unsigned short* sprite1,unsigned short* sprite2,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;
extern void GraySprite32_XOR(short x,short y,short h,unsigned long* sprite1,unsigned long* sprite2,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;
extern void GraySpriteX8_XOR(short x,short y,short h,unsigned char* sprite1,unsigned char* sprite2,short bytewidth,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;


//-----------------------------------------------------------------------------
// additionally sprite functions (MASK & BLIT)
//-----------------------------------------------------------------------------
extern void  Sprite8_MASK(short x,short y,short h,unsigned char* sprite,unsigned char* mask,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void Sprite16_MASK(short x,short y,short h,unsigned short* sprite,unsigned short* mask,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void Sprite32_MASK(short x,short y,short h,unsigned long* sprite,unsigned long* mask,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void SpriteX8_MASK(short x,short y,short h,unsigned char* sprite,unsigned char* mask,short bytewidth,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void  GraySprite8_MASK(short x,short y,short h,unsigned char* sprite1,unsigned char* sprite2,unsigned char* mask1,unsigned char* mask2,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;
extern void GraySprite16_MASK(short x,short y,short h,unsigned short* sprite1,unsigned short* sprite2,unsigned short* mask1,unsigned short* mask2,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;
extern void GraySprite32_MASK(short x,short y,short h,unsigned long* sprite1,unsigned long* sprite2,unsigned long* mask1,unsigned long* mask2,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;
extern void GraySpriteX8_MASK(short x,short y,short h,unsigned char* sprite1,unsigned char* sprite2,unsigned char* mask1,unsigned char* mask2,short bytewidth,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;

extern void  Sprite8_BLIT(short x,short y,short h,unsigned char* sprite,unsigned char maskval,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void Sprite16_BLIT(short x,short y,short h,unsigned short* sprite,unsigned short maskval,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void Sprite32_BLIT(short x,short y,short h,unsigned long* sprite,unsigned long maskval,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void SpriteX8_BLIT(short x,short y,short h,unsigned char* sprite,unsigned char* maskval,short bytewidth,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void  GraySprite8_BLIT(short x,short y,short h,unsigned char* sprite1,unsigned char* sprite2,unsigned char maskval,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;
extern void GraySprite16_BLIT(short x,short y,short h,unsigned short* sprite1,unsigned short* sprite2,unsigned short maskval,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;
extern void GraySprite32_BLIT(short x,short y,short h,unsigned long* sprite1,unsigned long* sprite2,unsigned long maskval,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;
extern void GraySpriteX8_BLIT(short x,short y,short h,unsigned char* sprite1,unsigned char* sprite2,unsigned char* maskval,short bytewidth,void* dest1,void* dest2) EXTGRAPH_FUNC_ATTRIBUTES;


//-----------------------------------------------------------------------------
// fast alternative functions for BitmapGet() functionality (not compatible)
//-----------------------------------------------------------------------------
extern void  Sprite8Get(short x,short y,short h,void* src,unsigned char* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void Sprite16Get(short x,short y,short h,void* src,unsigned short* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void Sprite32Get(short x,short y,short h,void* src,unsigned long* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void SpriteX8Get(short x,short y,short h,void* src,unsigned char* dest,short bytewidth) EXTGRAPH_FUNC_ATTRIBUTES;


//-----------------------------------------------------------------------------
// sprite scaling routines by Jim Haskell (jimhaskell@yahoo.com)
//
// scales a square input sprite to any extent (WARNING: no clipping is done!)
//
// [routines slightly modified to fit the needs]
//-----------------------------------------------------------------------------
extern void ScaleSprite8_OR(unsigned char *sprite,unsigned char *dest,short x0,short y0,short sizex,short sizey) EXTGRAPH_FUNC_ATTRIBUTES;
extern void ScaleSprite16_OR(unsigned short *sprite,unsigned char *dest,short x0,short y0,short sizex,short sizey) EXTGRAPH_FUNC_ATTRIBUTES;
extern void ScaleSprite32_OR(unsigned long *sprite,unsigned char *dest,short x0,short y0,short sizex,short sizey) EXTGRAPH_FUNC_ATTRIBUTES;
extern void ScaleSprite64_OR(unsigned long long *sprite,unsigned char *dest,short x0,short y0,short sizex,short sizey) EXTGRAPH_FUNC_ATTRIBUTES;

extern void ScaleSprite8_AND(unsigned char *sprite,unsigned char *dest,short x0,short y0,short sizex,short sizey) EXTGRAPH_FUNC_ATTRIBUTES;
extern void ScaleSprite16_AND(unsigned short *sprite,unsigned char *dest,short x0,short y0,short sizex,short sizey) EXTGRAPH_FUNC_ATTRIBUTES;
extern void ScaleSprite32_AND(unsigned long *sprite,unsigned char *dest,short x0,short y0,short sizex,short sizey) EXTGRAPH_FUNC_ATTRIBUTES;
extern void ScaleSprite64_AND(unsigned long long *sprite,unsigned char *dest,short x0,short y0,short sizex,short sizey) EXTGRAPH_FUNC_ATTRIBUTES;

extern void ScaleSprite8_XOR(unsigned char *sprite,unsigned char *dest,short x0,short y0,short sizex,short sizey) EXTGRAPH_FUNC_ATTRIBUTES;
extern void ScaleSprite16_XOR(unsigned short *sprite,unsigned char *dest,short x0,short y0,short sizex,short sizey) EXTGRAPH_FUNC_ATTRIBUTES;
extern void ScaleSprite32_XOR(unsigned long *sprite,unsigned char *dest,short x0,short y0,short sizex,short sizey) EXTGRAPH_FUNC_ATTRIBUTES;
extern void ScaleSprite64_XOR(unsigned long long *sprite,unsigned char *dest,short x0,short y0,short sizex,short sizey) EXTGRAPH_FUNC_ATTRIBUTES;


//-----------------------------------------------------------------------------
// floodfill routines (courtesy of Zeljko Juric)
//-----------------------------------------------------------------------------
extern void FloodFill(short x,short y,unsigned short shade,void* tmpplane,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void FloodFillMF(short x,short y,unsigned short shade,void* dest) EXTGRAPH_FUNC_ATTRIBUTES;


//-----------------------------------------------------------------------------
// sprite x8 mirror functions
//-----------------------------------------------------------------------------
extern void SpriteX8_MIRROR_H(short h,unsigned char* src,short bytewidth,unsigned char* dest) EXTGRAPH_FUNC_ATTRIBUTES;
extern void SpriteX8_MIRROR_V(short h,unsigned char* src,short bytewidth,unsigned char* dest) EXTGRAPH_FUNC_ATTRIBUTES;


//=============================================================================
//=============================================================================
//=============================================================================
//
// TTUNPACK.H (now extgraph.h contains everything necessary for unpacking)
// taken from TIGCC Tools Suite (c) 2000,2001,2002 TiCT
//
//=============================================================================
//=============================================================================
//=============================================================================
#ifndef __TTUNPACK_H__
#define __TTUNPACK_H__

typedef struct {
    unsigned char  osize_lo;   // original size lowbyte
    unsigned char  osize_hi;   // original size highbyte
    unsigned char  magic1;     // must be equal to TTUNPACK_MAGIC1
    unsigned char  magic2;     // must be equal to TTUNPACK_MAGIC2
    unsigned char  csize_lo;   // compressed size lowbyte
    unsigned char  csize_hi;   // compressed size lowbyte
    unsigned char  esc1;       // escape >> (8-escBits)
    unsigned char  notused3;
    unsigned char  notused4;
    unsigned char  esc2;       // escBits
    unsigned char  gamma1;     // maxGamma + 1
    unsigned char  gamma2;     // (1<<maxGamma)
    unsigned char  extralz;    // extraLZPosBits
    unsigned char  notused1;
    unsigned char  notused2;
    unsigned char  rleentries; // rleUsed
} TTUNPACK_HEADER;

#define TTUNPACK_MAGIC1 0x54
#define TTUNPACK_MAGIC2 0x50

#define ttunpack_size(_p_)  ((unsigned short)(((TTUNPACK_HEADER*)(_p_))->osize_lo | (((TTUNPACK_HEADER*)(_p_))->osize_hi << 8)))
#define ttunpack_valid(_p_) (((TTUNPACK_HEADER*)(_p_))->magic1 == TTUNPACK_MAGIC1 && ((TTUNPACK_HEADER*)(_p_))->magic2 == TTUNPACK_MAGIC2)

#define TTUNPACK_OKAY             0
#define TTUNPACK_NOESCFOUND     248
#define TTUNPACK_ESCBITS        249
#define TTUNPACK_MAXGAMMA       250
#define TTUNPACK_EXTRALZP       251
#define TTUNPACK_NOMAGIC        252
#define TTUNPACK_OUTBUFOVERRUN  253
#define TTUNPACK_LZPOSUNDERRUN  254

#define ttunpack_decompress UnpackBuffer
short UnpackBuffer(unsigned char *src, unsigned char *dest) EXTGRAPH_FUNC_ATTRIBUTES;

#else
#error EXTGRAPH.H already contains TTUNPACK.H defines (remove ttunpack.h include!)
#endif

//=============================================================================
//=============================================================================
//=============================================================================
//
// TTARCHIVE.H (now extgraph.h contains everything for archive handling)
// taken from TIGCC Tools Suite (c) 2000,2001,2002 TiCT
//
//=============================================================================
//=============================================================================
//=============================================================================
#ifndef __TTARCHIVE_H__
#define __TTARCHIVE_H__

//-----------------------------------------------------------------------------
// entry of archive structure
//-----------------------------------------------------------------------------
typedef struct {
   unsigned short offset;     // offset to the entry data from end of entry list
   unsigned short length;     // length of entry
   char           name[8];    // entry name
   unsigned char  misc1[2];   // info from cfg file (may be queried by a program)
   unsigned char  misc2[2];   // info from cfg file (may be queried by a program)
} TTARCHIVE_ENTRY;

//-----------------------------------------------------------------------------
// header of a TTARCHIVE file
//-----------------------------------------------------------------------------
typedef struct {
   unsigned long   magic;    // must be equal to TTARCHIVE_MAGIC
   unsigned short  nr;       // number of entries
   TTARCHIVE_ENTRY entry[0]; // here comes the list of TTARCHIVE_ENTRY structures
} TTARCHIVE_HEADER;

#define TTARCHIVE_MAGIC 0x74746100L

//-----------------------------------------------------------------------------
// smart macros to access ttarchive
//
// _ph_  ... pointer to archive start address
// _i_   ... index of entry
//
// NOTE: No checking is done in the macros !!
//-----------------------------------------------------------------------------

#define ttarchive_valid(_p_)     (((TTARCHIVE_HEADER*)(_p_))->magic == TTARCHIVE_MAGIC)
#define ttarchive_entries(_p_)   (((TTARCHIVE_HEADER*)(_p_))->nr)
#define ttarchive_desc(_p_,_i_)  (&(((TTARCHIVE_HEADER*)(_p_))->entry[_i_]))
#define ttarchive_data(_p_,_i_)  (((unsigned char*)&(((TTARCHIVE_HEADER*)(_p_))->entry[((TTARCHIVE_HEADER*)(_p_))->nr]))+\
                                 ((TTARCHIVE_HEADER*)(_p_))->entry[_i_].offset)

#define ttarchive_size(_p_)      ({TTARCHIVE_ENTRY* e=&(((TTARCHIVE_HEADER*)(_p_))->entry[((TTARCHIVE_HEADER*)(_p_))->nr-1]);\
                                  ((unsigned char*)e)+sizeof(TTARCHIVE_ENTRY)+e->offset+e->length-(unsigned char*)(_p_);})

#define ttarchive_info(_p_)      ({char* p=((char*)(_p_))+ttarchive_size(_p_)+21;\
                                  ((!(*p) && (*(p+1) ==((char)0xad)))?(p-20):NULL);})

#define TTARCHIVE_INFOLENGTH 20

#else
#error EXTGRAPH.H already contains TTARCHIVE.H defines (remove ttarchive.h include!)
#endif

#endif

//#############################################################################
// Revision History
//#############################################################################
//
// $Log: extgraph.h,v $
// Revision 1.13  2002/05/22 09:19:20  tnussb
// for TIGCC versions greater than 0.93 all functions are declared with
// "__attribute__((__stkparm__))" to work correctly with commandline
// switch -mregparm
//
// Revision 1.12  2002/05/08 19:32:46  tnussb
// version number raised (just a bug in unpack function was fixed)
//
// Revision 1.11  2002/04/05 13:47:28  tnussb
// changes for v1.00
//
// Revision 1.10  2002/04/02 18:10:35  tnussb
// version number raised
//
// Revision 1.9  2002/03/21 12:20:39  tnussb
// FastDrawVLine added and version number changed
//
// Revision 1.8  2002/02/25 17:01:01  tnussb
// ttunpack stuff moved in front of ttarchive stuff
//
// Revision 1.7  2002/02/25 13:16:42  tnussb
// ttarchive.h and ttunpack.h integrated
//
// Revision 1.6  2002/02/22 17:13:57  tnussb
// fixed double use of EXTGRAPH_VERSION_MAIN and EXTGRAPH_VERSION_SUB (thanx, Kevin!)
//
// Revision 1.5  2002/02/22 16:31:03  tnussb
// (1) version depended stuff added (like EXTGRAPH_VERSION_STR)
// (2) complete header revised
// (3) CheckHWMatch() and DESIRED_CALCTYPE added
//
// Revision 1.4  2002/02/11 13:43:54  tnussb
// version number raised
//
// Revision 1.3  2002/02/11 10:24:37  tnussb
// generic commit for v0.87
//
// Revision 1.2  2001/06/22 14:33:01  Thomas Nussbaumer
// grayscale sprite drawing routines added
//
// Revision 1.1  2001/06/20 21:34:08  Thomas Nussbaumer
// initial check-in
//
//