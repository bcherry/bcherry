/******************************************************************************
*
* E X T G R A P H    v 2 . 0 0    Beta  4
* Copyright © 2001-2004 TICT (TI-Chess Team)
*
*******************************************************************************
*
* ExtGraph is a compile-time library which contains speed optimized graphics
* routines for the TIGCC cross-compiler for TI-89, TI-89T, TI-92+ and TI-V200
* (TI-68k calculators).
*
* NOTE: using ExtGraph 2.xx requires TIGCC 0.95 and newer.
* Recompiling ExtGraph is currently not very easy for someone else, but
* that's not a problem since ExtGraph can be used as is (extgraph.h +
* tilemap.h + preshift.h + extgraph.a + tilemap.a); recompiling ExtGraph is
* therefore neither necessary nor recommended.
*
* Please use as few __stkparm__ functions as possible in your programs: most
* functions have __regparm__ versions (or a mix of __regparm__ and __stkparm__),
* which are themselves smaller and faster, and lead to smaller and faster code !
*
*
* library maintained, improved and extended by:
*                    Thomas Nussbaumer  (thomas.nussbaumer@gmx.net)
*                    Lionel Debroux     (lionel_debroux@yahoo.fr)
*                    Julien Richard-Foy (julien.rf@wanadoo.fr)
*                    various contributors.
*
*
*******************************************************************************
* [internal version] $Id: extgraph.h,v 1.13 2002/05/22 09:19:20 tnussb Exp $
*******************************************************************************/

#if !defined(__PRESHIFT__)
#define __PRESHIFT__

#include <gray.h>   // necessary for GrayGetPlane() macros in the grayscale
                    // support routines !!

//-----------------------------------------------------------------------------
// Reject TIGCC 0.94 and less (0.93 is not the official release anymore and
// it doesn't support __attribute__((__regparm__)) and 
// __attribute__((__stkparm__)); 0.94 won't compile some attributes used in
// the demos; 0.95 has a new good linker and is better overall)...
// Warn about other compilers because ExtGraph was tested only under TIGCC
// until further notice.
//-----------------------------------------------------------------------------
#include <version.h>

#ifndef __TIGCC__
#warning This compiler is unknown to ExtGraph. Until further notice, ExtGraph was tested only under standards-compliant TIGCC. Other compilers may not support ExtGraph.
#endif

#if (defined(__TIGCC__)&&(__TIGCC__<1)&&(defined(__TIGCC_MINOR__)&&(__TIGCC_MINOR__<95)))
#error ExtGraph needs TIGCC 0.95 or later ! Update TIGCC.
#endif


//-----------------------------------------------------------------------------
// Preshifted sprite functions: helper functions to create semi-preshifted
// sprites from B/W and gray non-masked interlaced 8x8 and 16x16 sprites;
// non-clipped OR and XOR macros (to save branches) with completely unrolled
// loops drawing those semi-preshifted sprites. This extreme brute force
// squeezes out nearly 900 sprites/sec (about half for function -> macro,
// half for extreme loop unrolling), but costs more than 200 bytes for each
// call to GrayPSprite16x16_OR_R !
//
// Only 1 shifting out of 2 is precomputed: this of course decreases average
// brute force performance if preshifted sprites are displayed at odd
// coordinates (not if they're displayed at even coordinates). However,
// semi-preshifted sprites remain significanly faster than equivalent
// GraySpriteX functions on average, and completely preshifted sprites take
// an insane amount of memory (2K per 16x16 preshifted grayscale sprite)
// which makes them merely unusable (only 31 16x16 preshifted grayscale
// sprites per 65518 bytes block !). Semi-preshifted sprites allow for 63
// preshifted 16x16 grayscale sprites per 65518 bytes block.
// Anyway, as you can see in the documentation, there is hardly ever ANY
// game that couldn't be done with normal sprites or a tilemap engine.
// Think that using simpler modes + background save / restore can be smarter
// than always redrawing everything.
//
// Semi-preshifted 8x8 sprites are written so as to take advantage of 1 word
// being enough for 4 shiftings out of 8. This allows for 170 preshifted 8x8
// grayscale sprites per 65518 bytes block.
//
// Credits go to PpHd for the movem.l (sprt)+,d0-d7; or.l d0...d7,(dest)+
// trick, taken in GenLib. It is <~1% faster than repeated 2-instruction
// move / or sequences, with a simple test code (it might be slightly slower
// with real-life programs, since it clobbers more registers, I don't know),
// but it is significantly smaller.
//-----------------------------------------------------------------------------
#define SIZE_OF_PSPRITE8x8    (1*(8*(4*2+4*4)))
#define SIZE_OF_PGSPRITE8x8   (2*(8*(4*2+4*4)))
#define SIZE_OF_PSPRITE16x16  (1*(16*(8*4)))
#define SIZE_OF_PGSPRITE16x16 (2*(16*(8*4)))
//extern void PreshiftSprite8x8(register const unsigned char *src asm("%a0"),register unsigned long *dest asm("%a1"));
extern void PreshiftSprite16x16(register const unsigned short *src asm("%a0"),register unsigned long *dest asm("%a1"));
//extern void PreshiftGrayISprite8x8(register unsigned char *src asm("%a0"),register unsigned long *dest asm("%a1"));
extern void PreshiftGrayISprite16x16(register const unsigned short *src asm("%a0"),register unsigned long *dest asm("%a1"));


#define PSprite16x16_OR_R(x,y,sprt,dest)\
asm("movem.l %%d5-%%d7,-(%%sp);\
    move.w   %0,%%d0;\
    move.w   %1,%%d1;\
    move.l   %2,%%a1;\
    move.l   %3,%%a0;\
    move.w   %%d1,%%d2;\
    lsl.w    #4,%%d1;\
    sub.w    %%d2,%%d1;\
    move.w   %%d0,%%d2;\
    lsr.w    #4,%%d2;\
    add.w    %%d2,%%d1;\
    add.w    %%d1,%%d1;\
    adda.w   %%d1,%%a0;\
    move.w   %%d0,%%d2;\
    andi.w   #0xE,%%d0;\
    lsl.w    #5,%%d0;\
    adda.w   %%d0,%%a1;\
    andi.w   #0x1,%%d2;\
    jbne     __loop2_PS16x16_OR_R_;\
__loop1_PS16x16_OR_R_:\
    movem.l  (%%a1)+,%%d0-%%d7;\
    or.l     %%d0,(%%a0);\
    or.l     %%d1,30(%%a0);\
    or.l     %%d2,60(%%a0);\
    or.l     %%d3,90(%%a0);\
    or.l     %%d4,120(%%a0);\
    or.l     %%d5,150(%%a0);\
    or.l     %%d6,180(%%a0);\
    or.l     %%d7,210(%%a0);\
    movem.l  (%%a1)+,%%d0-%%d7;\
    or.l     %%d0,240(%%a0);\
    or.l     %%d1,270(%%a0);\
    or.l     %%d2,300(%%a0);\
    or.l     %%d3,330(%%a0);\
    or.l     %%d4,360(%%a0);\
    or.l     %%d5,390(%%a0);\
    or.l     %%d6,420(%%a0);\
    or.l     %%d7,450(%%a0);\
    jbra     __end_PS16x16_OR_R_;\
__loop2_PS16x16_OR_R_:\
    movem.l  (%%a1)+,%%d0-%%d7;\
    ror.l    #1,%%d0;\
    or.l     %%d0,(%%a0);\
    ror.l    #1,%%d1;\
    or.l     %%d1,30(%%a0);\
    ror.l    #1,%%d2;\
    or.l     %%d2,60(%%a0);\
    ror.l    #1,%%d3;\
    or.l     %%d3,90(%%a0);\
    ror.l    #1,%%d4;\
    or.l     %%d4,120(%%a0);\
    ror.l    #1,%%d5;\
    or.l     %%d5,150(%%a0);\
    ror.l    #1,%%d6;\
    or.l     %%d6,180(%%a0);\
    ror.l    #1,%%d7;\
    or.l     %%d7,210(%%a0);\
    movem.l  (%%a1)+,%%d0-%%d7;\
    ror.l    #1,%%d0;\
    or.l     %%d0,240(%%a0);\
    ror.l    #1,%%d1;\
    or.l     %%d1,270(%%a0);\
    ror.l    #1,%%d2;\
    or.l     %%d2,300(%%a0);\
    ror.l    #1,%%d3;\
    or.l     %%d3,330(%%a0);\
    ror.l    #1,%%d4;\
    or.l     %%d4,360(%%a0);\
    ror.l    #1,%%d5;\
    or.l     %%d5,390(%%a0);\
    ror.l    #1,%%d6;\
    or.l     %%d6,420(%%a0);\
    ror.l    #1,%%d7;\
    or.l     %%d7,450(%%a0);\
__end_PS16x16_OR_R_:\
    movem.l  (%%sp)+,%%d5-%%d7;\
" : : "d"(x),"d"(y),"a"(sprt),"a"(dest) : "d0","d1","d2","d3","d4","a0","a1","cc");

#define PSprite16x16_XOR_R(x,y,sprt,dest)\
asm("movem.l %%d5-%%d7,-(%%sp);\
    move.w   %0,%%d0;\
    move.w   %1,%%d1;\
    move.l   %2,%%a1;\
    move.l   %3,%%a0;\
    move.w   %%d1,%%d2;\
    lsl.w    #4,%%d1;\
    sub.w    %%d2,%%d1;\
    move.w   %%d0,%%d2;\
    lsr.w    #4,%%d2;\
    add.w    %%d2,%%d1;\
    add.w    %%d1,%%d1;\
    adda.w   %%d1,%%a0;\
    move.w   %%d0,%%d2;\
    andi.w   #0xE,%%d0;\
    lsl.w    #5,%%d0;\
    adda.w   %%d0,%%a1;\
    andi.w   #0x1,%%d2;\
    jbne     __loop2_PS16x16_XOR_R_;\
__loop1_PS16x16_XOR_R_:\
    movem.l  (%%a1)+,%%d0-%%d7;\
    eor.l    %%d0,(%%a0);\
    eor.l    %%d1,30(%%a0);\
    eor.l    %%d2,60(%%a0);\
    eor.l    %%d3,90(%%a0);\
    eor.l    %%d4,120(%%a0);\
    eor.l    %%d5,150(%%a0);\
    eor.l    %%d6,180(%%a0);\
    eor.l    %%d7,210(%%a0);\
    movem.l  (%%a1)+,%%d0-%%d7;\
    eor.l    %%d0,240(%%a0);\
    eor.l    %%d1,270(%%a0);\
    eor.l    %%d2,300(%%a0);\
    eor.l    %%d3,330(%%a0);\
    eor.l    %%d4,360(%%a0);\
    eor.l    %%d5,390(%%a0);\
    eor.l    %%d6,420(%%a0);\
    eor.l    %%d7,450(%%a0);\
    jbra     __end_PS16x16_XOR_R_;\
__loop2_PS16x16_XOR_R_:\
    movem.l  (%%a1)+,%%d0-%%d7;\
    ror.l    #1,%%d0;\
    eor.l    %%d0,(%%a0);\
    ror.l    #1,%%d1;\
    eor.l    %%d1,30(%%a0);\
    ror.l    #1,%%d2;\
    eor.l    %%d2,60(%%a0);\
    ror.l    #1,%%d3;\
    eor.l    %%d3,90(%%a0);\
    ror.l    #1,%%d4;\
    eor.l    %%d4,120(%%a0);\
    ror.l    #1,%%d5;\
    eor.l    %%d5,150(%%a0);\
    ror.l    #1,%%d6;\
    eor.l    %%d6,180(%%a0);\
    ror.l    #1,%%d7;\
    eor.l    %%d7,210(%%a0);\
    movem.l  (%%a1)+,%%d0-%%d7;\
    ror.l    #1,%%d0;\
    eor.l    %%d0,240(%%a0);\
    ror.l    #1,%%d1;\
    eor.l    %%d1,270(%%a0);\
    ror.l    #1,%%d2;\
    eor.l    %%d2,300(%%a0);\
    ror.l    #1,%%d3;\
    eor.l    %%d3,330(%%a0);\
    ror.l    #1,%%d4;\
    eor.l    %%d4,360(%%a0);\
    ror.l    #1,%%d5;\
    eor.l    %%d5,390(%%a0);\
    ror.l    #1,%%d6;\
    eor.l    %%d6,420(%%a0);\
    ror.l    #1,%%d7;\
    eor.l    %%d7,450(%%a0);\
__end_PS16x16_XOR_R_:\
    movem.l  (%%sp)+,%%d5-%%d7;\
" : : "d"(x),"d"(y),"a"(sprt),"a"(dest) : "d0","d1","d2","d3","d4","a0","a1","cc");

#define GrayPSprite16x16_OR_R(x,y,sprt,dest0,dest1)\
asm("movem.l %%d5-%%d7,-(%%sp);\
    move.w   %0,%%d0;\
    move.w   %1,%%d1;\
    move.l   %2,%%a2;\
    move.l   %3,%%a0;\
    move.l   %4,%%a1;\
    move.w   %%d1,%%d2;\
    lsl.w    #4,%%d1;\
    sub.w    %%d2,%%d1;\
    move.w   %%d0,%%d2;\
    lsr.w    #4,%%d2;\
    add.w    %%d2,%%d1;\
    add.w    %%d1,%%d1;\
    adda.w   %%d1,%%a0;\
    adda.w   %%d1,%%a1;\
    move.w   %%d0,%%d2;\
    andi.w   #0xE,%%d0;\
    lsl.w    #6,%%d0;\
    adda.w   %%d0,%%a2;\
    andi.w   #0x1,%%d2;\
    jbne     __loop2_GPS16x16_OR_R_;\
__loop1_GPS16x16_OR_R_:\
    movem.l  (%%a2)+,%%d0-%%d7;\
    or.l     %%d0,(%%a0);\
    or.l     %%d1,(%%a1);\
    or.l     %%d2,30(%%a0);\
    or.l     %%d3,30(%%a1);\
    or.l     %%d4,60(%%a0);\
    or.l     %%d5,60(%%a1);\
    or.l     %%d6,90(%%a0);\
    or.l     %%d7,90(%%a1);\
    movem.l  (%%a2)+,%%d0-%%d7;\
    or.l     %%d0,120(%%a0);\
    or.l     %%d1,120(%%a1);\
    or.l     %%d2,150(%%a0);\
    or.l     %%d3,150(%%a1);\
    or.l     %%d4,180(%%a0);\
    or.l     %%d5,180(%%a1);\
    or.l     %%d6,210(%%a0);\
    or.l     %%d7,210(%%a1);\
    movem.l  (%%a2)+,%%d0-%%d7;\
    or.l     %%d0,240(%%a0);\
    or.l     %%d1,240(%%a1);\
    or.l     %%d2,270(%%a0);\
    or.l     %%d3,270(%%a1);\
    or.l     %%d4,300(%%a0);\
    or.l     %%d5,300(%%a1);\
    or.l     %%d6,330(%%a0);\
    or.l     %%d7,330(%%a1);\
    movem.l  (%%a2)+,%%d0-%%d7;\
    or.l     %%d0,360(%%a0);\
    or.l     %%d1,360(%%a1);\
    or.l     %%d2,390(%%a0);\
    or.l     %%d3,390(%%a1);\
    or.l     %%d4,420(%%a0);\
    or.l     %%d5,420(%%a1);\
    or.l     %%d6,450(%%a0);\
    or.l     %%d7,450(%%a1);\
    jbra     __end_GPS16x16_OR_R_;\
__loop2_GPS16x16_OR_R_:\
    movem.l  (%%a2)+,%%d0-%%d7;\
    ror.l    #1,%%d0;\
    or.l     %%d0,(%%a0);\
    ror.l    #1,%%d1;\
    or.l     %%d1,(%%a1);\
    ror.l    #1,%%d2;\
    or.l     %%d2,30(%%a0);\
    ror.l    #1,%%d3;\
    or.l     %%d3,30(%%a1);\
    ror.l    #1,%%d4;\
    or.l     %%d4,60(%%a0);\
    ror.l    #1,%%d5;\
    or.l     %%d5,60(%%a1);\
    ror.l    #1,%%d6;\
    or.l     %%d6,90(%%a0);\
    ror.l    #1,%%d7;\
    or.l     %%d7,90(%%a1);\
    movem.l  (%%a2)+,%%d0-%%d7;\
    ror.l    #1,%%d0;\
    or.l     %%d0,120(%%a0);\
    ror.l    #1,%%d1;\
    or.l     %%d1,120(%%a1);\
    ror.l    #1,%%d2;\
    or.l     %%d2,150(%%a0);\
    ror.l    #1,%%d3;\
    or.l     %%d3,150(%%a1);\
    ror.l    #1,%%d4;\
    or.l     %%d4,180(%%a0);\
    ror.l    #1,%%d5;\
    or.l     %%d5,180(%%a1);\
    ror.l    #1,%%d6;\
    or.l     %%d6,210(%%a0);\
    ror.l    #1,%%d7;\
    or.l     %%d7,210(%%a1);\
    movem.l  (%%a2)+,%%d0-%%d7;\
    ror.l    #1,%%d0;\
    or.l     %%d0,240(%%a0);\
    ror.l    #1,%%d1;\
    or.l     %%d1,240(%%a1);\
    ror.l    #1,%%d2;\
    or.l     %%d2,270(%%a0);\
    ror.l    #1,%%d3;\
    or.l     %%d3,270(%%a1);\
    ror.l    #1,%%d4;\
    or.l     %%d4,300(%%a0);\
    ror.l    #1,%%d5;\
    or.l     %%d5,300(%%a1);\
    ror.l    #1,%%d6;\
    or.l     %%d6,330(%%a0);\
    ror.l    #1,%%d7;\
    or.l     %%d7,330(%%a1);\
    movem.l  (%%a2)+,%%d0-%%d7;\
    ror.l    #1,%%d0;\
    or.l     %%d0,360(%%a0);\
    ror.l    #1,%%d1;\
    or.l     %%d1,360(%%a1);\
    ror.l    #1,%%d2;\
    or.l     %%d2,390(%%a0);\
    ror.l    #1,%%d3;\
    or.l     %%d3,390(%%a1);\
    ror.l    #1,%%d4;\
    or.l     %%d4,420(%%a0);\
    ror.l    #1,%%d5;\
    or.l     %%d5,420(%%a1);\
    ror.l    #1,%%d6;\
    or.l     %%d6,450(%%a0);\
    ror.l    #1,%%d7;\
    or.l     %%d7,450(%%a1);\
__end_GPS16x16_OR_R_:\
    movem.l  (%%sp)+,%%d5-%%d7;\
" : : "d"(x),"d"(y),"a"(sprt),"a"(dest0),"a"(dest1) : "d0","d1","d2","d3","d4","a0","a1","a2","cc");

#define GrayPSprite16x16_XOR_R(x,y,sprt,dest0,dest1)\
asm("movem.l %%d5-%%d7,-(%%sp);\
    move.w   %0,%%d0;\
    move.w   %1,%%d1;\
    move.l   %2,%%a2;\
    move.l   %3,%%a0;\
    move.l   %4,%%a1;\
    move.w   %%d1,%%d2;\
    lsl.w    #4,%%d1;\
    sub.w    %%d2,%%d1;\
    move.w   %%d0,%%d2;\
    lsr.w    #4,%%d2;\
    add.w    %%d2,%%d1;\
    add.w    %%d1,%%d1;\
    adda.w   %%d1,%%a0;\
    adda.w   %%d1,%%a1;\
    move.w   %%d0,%%d2;\
    andi.w   #0xE,%%d0;\
    lsl.w    #6,%%d0;\
    adda.w   %%d0,%%a2;\
    andi.w   #0x1,%%d2;\
    jbne     __loop2_GPS16x16_XOR_R_;\
__loop1_GPS16x16_XOR_R_:\
    movem.l  (%%a2)+,%%d0-%%d7;\
    eor.l    %%d0,(%%a0);\
    eor.l    %%d1,(%%a1);\
    eor.l    %%d2,30(%%a0);\
    eor.l    %%d3,30(%%a1);\
    eor.l    %%d4,60(%%a0);\
    eor.l    %%d5,60(%%a1);\
    eor.l    %%d6,90(%%a0);\
    eor.l    %%d7,90(%%a1);\
    movem.l  (%%a2)+,%%d0-%%d7;\
    eor.l    %%d0,120(%%a0);\
    eor.l    %%d1,120(%%a1);\
    eor.l    %%d2,150(%%a0);\
    eor.l    %%d3,150(%%a1);\
    eor.l    %%d4,180(%%a0);\
    eor.l    %%d5,180(%%a1);\
    eor.l    %%d6,210(%%a0);\
    eor.l    %%d7,210(%%a1);\
    movem.l  (%%a2)+,%%d0-%%d7;\
    eor.l    %%d0,240(%%a0);\
    eor.l    %%d1,240(%%a1);\
    eor.l    %%d2,270(%%a0);\
    eor.l    %%d3,270(%%a1);\
    eor.l    %%d4,300(%%a0);\
    eor.l    %%d5,300(%%a1);\
    eor.l    %%d6,330(%%a0);\
    eor.l    %%d7,330(%%a1);\
    movem.l  (%%a2)+,%%d0-%%d7;\
    eor.l    %%d0,360(%%a0);\
    eor.l    %%d1,360(%%a1);\
    eor.l    %%d2,390(%%a0);\
    eor.l    %%d3,390(%%a1);\
    eor.l    %%d4,420(%%a0);\
    eor.l    %%d5,420(%%a1);\
    eor.l    %%d6,450(%%a0);\
    eor.l    %%d7,450(%%a1);\
    jbra     __end_GPS16x16_XOR_R_;\
__loop2_GPS16x16_XOR_R_:\
    movem.l  (%%a2)+,%%d0-%%d7;\
    ror.l    #1,%%d0;\
    eor.l    %%d0,(%%a0);\
    ror.l    #1,%%d1;\
    eor.l    %%d1,(%%a1);\
    ror.l    #1,%%d2;\
    eor.l    %%d2,30(%%a0);\
    ror.l    #1,%%d3;\
    eor.l    %%d3,30(%%a1);\
    ror.l    #1,%%d4;\
    eor.l    %%d4,60(%%a0);\
    ror.l    #1,%%d5;\
    eor.l    %%d5,60(%%a1);\
    ror.l    #1,%%d6;\
    eor.l    %%d6,90(%%a0);\
    ror.l    #1,%%d7;\
    eor.l    %%d7,90(%%a1);\
    movem.l  (%%a2)+,%%d0-%%d7;\
    ror.l    #1,%%d0;\
    eor.l    %%d0,120(%%a0);\
    ror.l    #1,%%d1;\
    eor.l    %%d1,120(%%a1);\
    ror.l    #1,%%d2;\
    eor.l    %%d2,150(%%a0);\
    ror.l    #1,%%d3;\
    eor.l    %%d3,150(%%a1);\
    ror.l    #1,%%d4;\
    eor.l    %%d4,180(%%a0);\
    ror.l    #1,%%d5;\
    eor.l    %%d5,180(%%a1);\
    ror.l    #1,%%d6;\
    eor.l    %%d6,210(%%a0);\
    ror.l    #1,%%d7;\
    eor.l    %%d7,210(%%a1);\
    movem.l  (%%a2)+,%%d0-%%d7;\
    ror.l    #1,%%d0;\
    eor.l    %%d0,240(%%a0);\
    ror.l    #1,%%d1;\
    eor.l    %%d1,240(%%a1);\
    ror.l    #1,%%d2;\
    eor.l    %%d2,270(%%a0);\
    ror.l    #1,%%d3;\
    eor.l    %%d3,270(%%a1);\
    ror.l    #1,%%d4;\
    eor.l    %%d4,300(%%a0);\
    ror.l    #1,%%d5;\
    eor.l    %%d5,300(%%a1);\
    ror.l    #1,%%d6;\
    eor.l    %%d6,330(%%a0);\
    ror.l    #1,%%d7;\
    eor.l    %%d7,330(%%a1);\
    movem.l  (%%a2)+,%%d0-%%d7;\
    ror.l    #1,%%d0;\
    eor.l    %%d0,360(%%a0);\
    ror.l    #1,%%d1;\
    eor.l    %%d1,360(%%a1);\
    ror.l    #1,%%d2;\
    eor.l    %%d2,390(%%a0);\
    ror.l    #1,%%d3;\
    eor.l    %%d3,390(%%a1);\
    ror.l    #1,%%d4;\
    eor.l    %%d4,420(%%a0);\
    ror.l    #1,%%d5;\
    eor.l    %%d5,420(%%a1);\
    ror.l    #1,%%d6;\
    eor.l    %%d6,450(%%a0);\
    ror.l    #1,%%d7;\
    eor.l    %%d7,450(%%a1);\
__end_GPS16x16_XOR_R_:\
    movem.l  (%%sp)+,%%d5-%%d7;\
" : : "d"(x),"d"(y),"a"(sprt),"a"(dest0),"a"(dest1) : "d0","d1","d2","d3","d4","a0","a1","a2","cc");

#endif
