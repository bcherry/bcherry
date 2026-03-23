#ifndef RENDER_H
#define RENDER_H

#include <stdint.h>
#include <SDL.h>

// TI-89 screen dimensions
#define LCD_WIDTH 160
#define LCD_HEIGHT 100
#define SCALE 4

// Grayscale values (2-bit: combine plane0 and plane1)
// 0=white, 1=dark gray (C1 only), 2=light gray (C0 only), 3=black (both)
#define COLOR_WHITE 0
#define COLOR_DGRAY 1
#define COLOR_LGRAY 2
#define COLOR_BLACK 3

// Initialize SDL2 rendering (window, renderer, texture)
int render_init(void);

// Clean up SDL2 resources
void render_cleanup(void);

// Clear the framebuffer to white (0)
void render_clear(void);

// Present the framebuffer to screen (copy to SDL texture and render)
void render_present(void);

// Blit an 8-pixel-wide sprite with mask (SMASK mode)
// c0: plane0 data (uint8_t array, one byte per row)
// c1: plane1 data (uint8_t array, one byte per row)
// mask: mask data (uint8_t array, 1=transparent bit)
// If mask is NULL, blit with OR mode (no mask)
void blit_sprite_8(int x, int y, int h, const uint8_t *c0, const uint8_t *c1, const uint8_t *mask);

// Blit a 16-pixel-wide sprite with mask (SMASK mode)
// c0: plane0 data (uint16_t array, one short per row)
// c1: plane1 data (uint16_t array, one short per row)
// mask: mask data (uint16_t array) - can be NULL for OR mode
void blit_sprite_16(int x, int y, int h, const uint16_t *c0, const uint16_t *c1, const uint16_t *mask);

// Blit 8-wide sprite using OR mode only (no mask clearing)
void blit_sprite_8_or(int x, int y, int h, const uint8_t *c0, const uint8_t *c1);

// Blit 8-wide sprite using TRANB mode (transparent where both planes are black/1)
void blit_sprite_8_tranb(int x, int y, int h, const uint8_t *c0, const uint8_t *c1);

// Blit 8-wide sprite using TRANW mode (transparent where both planes are white/0)
void blit_sprite_8_tranw(int x, int y, int h, const uint8_t *c0, const uint8_t *c1);

// Blit 32-wide sprite TRANW mode (transparent where both planes white)
void blit_sprite_32_tranw(int x, int y, int h, const uint32_t *c0, const uint32_t *c1);

// Fill a rectangle with a color
void render_fill_rect(int x1, int y1, int x2, int y2, uint8_t color);

// Invert a rectangle (for HUD weapon highlight)
void render_invert_rect(int x1, int y1, int x2, int y2);

// Get a pixel value from the framebuffer (for collision debug, etc.)
uint8_t render_get_pixel(int x, int y);

// Direct access to framebuffer for special effects
extern uint8_t framebuffer[LCD_HEIGHT][LCD_WIDTH];

#endif
