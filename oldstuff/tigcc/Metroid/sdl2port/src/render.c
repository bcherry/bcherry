#include "render.h"
#include <string.h>
#include <stdio.h>

// ---------------------------------------------------------------------------
// Framebuffer — each cell holds a 2-bit grayscale value (0-3)
// ---------------------------------------------------------------------------
uint8_t framebuffer[LCD_HEIGHT][LCD_WIDTH];

// ---------------------------------------------------------------------------
// SDL objects
// ---------------------------------------------------------------------------
static SDL_Window   *window   = NULL;
static SDL_Renderer *renderer = NULL;
static SDL_Texture  *texture  = NULL;

// ---------------------------------------------------------------------------
// Palette: 2-bit grayscale → ARGB8888
// ---------------------------------------------------------------------------
static const uint32_t palette[4] = {
    0xFFDDDDDD,  // 0 — white  (slightly off-white for retro feel)
    0xFF777777,  // 1 — dark gray
    0xFFAAAAAA,  // 2 — light gray
    0xFF222222,  // 3 — black
};

// ---------------------------------------------------------------------------
// render_init — create window, renderer, and streaming texture
// ---------------------------------------------------------------------------
int render_init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0"); // nearest-neighbor

    window = SDL_CreateWindow(
        "Metroid",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        LCD_WIDTH * SCALE, LCD_HEIGHT * SCALE,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow error: %s\n", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_RenderSetLogicalSize(renderer, LCD_WIDTH, LCD_HEIGHT);

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        LCD_WIDTH, LCD_HEIGHT
    );
    if (!texture) {
        fprintf(stderr, "SDL_CreateTexture error: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    render_clear();
    return 0;
}

// ---------------------------------------------------------------------------
// render_cleanup
// ---------------------------------------------------------------------------
void render_cleanup(void)
{
    if (texture)  { SDL_DestroyTexture(texture);   texture  = NULL; }
    if (renderer) { SDL_DestroyRenderer(renderer);  renderer = NULL; }
    if (window)   { SDL_DestroyWindow(window);      window   = NULL; }
    SDL_Quit();
}

// ---------------------------------------------------------------------------
// render_clear — fill framebuffer with white (0)
// ---------------------------------------------------------------------------
void render_clear(void)
{
    memset(framebuffer, COLOR_WHITE, sizeof(framebuffer));
}

// ---------------------------------------------------------------------------
// render_present — convert framebuffer to ARGB8888, push to SDL
// ---------------------------------------------------------------------------
void render_present(void)
{
    void *tex_pixels;
    int   tex_pitch;

    if (SDL_LockTexture(texture, NULL, &tex_pixels, &tex_pitch) != 0) {
        fprintf(stderr, "SDL_LockTexture error: %s\n", SDL_GetError());
        return;
    }

    for (int row = 0; row < LCD_HEIGHT; row++) {
        uint32_t *dst = (uint32_t *)((uint8_t *)tex_pixels + row * tex_pitch);
        for (int col = 0; col < LCD_WIDTH; col++) {
            dst[col] = palette[framebuffer[row][col] & 0x03];
        }
    }

    SDL_UnlockTexture(texture);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

// ---------------------------------------------------------------------------
// blit_sprite_8 — 8-pixel-wide sprite, SMASK mode
//
// mask bit = 1 → transparent (skip)
// mask bit = 0 → write combined grayscale from c0/c1
// MSB is leftmost pixel (bit 7 = x+0 … bit 0 = x+7)
// If mask is NULL, falls through to OR mode.
// ---------------------------------------------------------------------------
void blit_sprite_8(int x, int y, int h,
                   const uint8_t *c0, const uint8_t *c1,
                   const uint8_t *mask)
{
    if (!mask) {
        blit_sprite_8_or(x, y, h, c0, c1);
        return;
    }

    for (int row = 0; row < h; row++) {
        int py = y + row;
        if (py < 0 || py >= LCD_HEIGHT) continue;

        uint8_t p0 = c0[row];
        uint8_t p1 = c1[row];
        uint8_t m  = mask[row];

        for (int bit = 7; bit >= 0; bit--) {
            int px = x + (7 - bit);
            if (px < 0 || px >= LCD_WIDTH) continue;

            if ((m >> bit) & 1) continue; // transparent

            uint8_t gray = (uint8_t)(((p0 >> bit) & 1) << 1) |
                           (uint8_t)((p1 >> bit) & 1);
            framebuffer[py][px] = gray;
        }
    }
}

// ---------------------------------------------------------------------------
// blit_sprite_8_or — 8-pixel-wide sprite, OR mode (no mask)
//
// OR the grayscale bits into the existing framebuffer.  Bits that are set
// in c0/c1 will darken the destination; bits that are clear leave the
// destination unchanged.
// ---------------------------------------------------------------------------
void blit_sprite_8_or(int x, int y, int h,
                      const uint8_t *c0, const uint8_t *c1)
{
    for (int row = 0; row < h; row++) {
        int py = y + row;
        if (py < 0 || py >= LCD_HEIGHT) continue;

        uint8_t p0 = c0[row];
        uint8_t p1 = c1[row];

        for (int bit = 7; bit >= 0; bit--) {
            int px = x + (7 - bit);
            if (px < 0 || px >= LCD_WIDTH) continue;

            uint8_t gray = (uint8_t)(((p0 >> bit) & 1) << 1) |
                           (uint8_t)((p1 >> bit) & 1);
            framebuffer[py][px] |= gray;
        }
    }
}

// ---------------------------------------------------------------------------
// blit_sprite_8_tranb — 8-pixel-wide sprite, TRANB mode
//
// Transparent where BOTH c0 and c1 bits are 1 (black).
// All other pixels overwrite the destination with their combined value.
// Used for HUD numbers/text drawn on top of panel backgrounds.
// ---------------------------------------------------------------------------
void blit_sprite_8_tranb(int x, int y, int h,
                         const uint8_t *c0, const uint8_t *c1)
{
    for (int row = 0; row < h; row++) {
        int py = y + row;
        if (py < 0 || py >= LCD_HEIGHT) continue;

        uint8_t p0 = c0[row];
        uint8_t p1 = c1[row];

        for (int bit = 7; bit >= 0; bit--) {
            int px = x + (7 - bit);
            if (px < 0 || px >= LCD_WIDTH) continue;

            uint8_t b0 = (p0 >> bit) & 1;
            uint8_t b1 = (p1 >> bit) & 1;
            if (b0 == 1 && b1 == 1) continue;  /* transparent (black) */
            framebuffer[py][px] = (b0 << 1) | b1;
        }
    }
}

// ---------------------------------------------------------------------------
// blit_sprite_8_tranw — 8-pixel-wide sprite, TRANW mode
//
// Only draws pixels where at least one of c0/c1 bits is set (non-white).
// White pixels (both bits 0) are transparent.
// ---------------------------------------------------------------------------
void blit_sprite_8_tranw(int x, int y, int h,
                         const uint8_t *c0, const uint8_t *c1)
{
    for (int row = 0; row < h; row++) {
        int py = y + row;
        if (py < 0 || py >= LCD_HEIGHT) continue;

        uint8_t p0 = c0[row];
        uint8_t p1 = c1[row];

        for (int bit = 7; bit >= 0; bit--) {
            int px = x + (7 - bit);
            if (px < 0 || px >= LCD_WIDTH) continue;

            uint8_t b0 = (p0 >> bit) & 1;
            uint8_t b1 = (p1 >> bit) & 1;
            if (b0 == 0 && b1 == 0) continue;  /* transparent */
            framebuffer[py][px] = (b0 << 1) | b1;
        }
    }
}

// ---------------------------------------------------------------------------
// blit_sprite_16 — 16-pixel-wide sprite, SMASK mode
//
// Bit 15 is the leftmost pixel (x+0), bit 0 is x+15.
// If mask is NULL, uses OR mode instead.
// ---------------------------------------------------------------------------
void blit_sprite_16(int x, int y, int h,
                    const uint16_t *c0, const uint16_t *c1,
                    const uint16_t *mask)
{
    for (int row = 0; row < h; row++) {
        int py = y + row;
        if (py < 0 || py >= LCD_HEIGHT) continue;

        uint16_t p0 = c0[row];
        uint16_t p1 = c1[row];
        uint16_t m  = mask ? mask[row] : 0;

        for (int bit = 15; bit >= 0; bit--) {
            int px = x + (15 - bit);
            if (px < 0 || px >= LCD_WIDTH) continue;

            if (mask && ((m >> bit) & 1)) continue; // transparent

            uint8_t gray = (uint8_t)(((p0 >> bit) & 1) << 1) |
                           (uint8_t)((p1 >> bit) & 1);

            if (mask) {
                framebuffer[py][px] = gray;
            } else {
                framebuffer[py][px] |= gray;
            }
        }
    }
}

// ---------------------------------------------------------------------------
// blit_sprite_32_tranw — 32-pixel-wide sprite, TRANW mode
//
// Transparent where BOTH c0 and c1 bits are 0 (i.e. white).
// Otherwise writes combined grayscale value.
// Bit 31 is the leftmost pixel (x+0), bit 0 is x+31.
// ---------------------------------------------------------------------------
void blit_sprite_32_tranw(int x, int y, int h,
                          const uint32_t *c0, const uint32_t *c1)
{
    for (int row = 0; row < h; row++) {
        int py = y + row;
        if (py < 0 || py >= LCD_HEIGHT) continue;

        uint32_t p0 = c0[row];
        uint32_t p1 = c1[row];

        for (int bit = 31; bit >= 0; bit--) {
            int px = x + (31 - bit);
            if (px < 0 || px >= LCD_WIDTH) continue;

            uint8_t b0 = (p0 >> bit) & 1;
            uint8_t b1 = (p1 >> bit) & 1;

            if (b0 == 0 && b1 == 0) continue; // transparent (white)

            uint8_t gray = (uint8_t)(b0 << 1) | b1;
            framebuffer[py][px] = gray;
        }
    }
}

// ---------------------------------------------------------------------------
// render_fill_rect — fill a rectangle with a solid 2-bit color
// Coordinates are inclusive: (x1,y1) to (x2,y2).
// ---------------------------------------------------------------------------
void render_fill_rect(int x1, int y1, int x2, int y2, uint8_t color)
{
    // Clamp to framebuffer bounds
    if (x1 < 0) x1 = 0;
    if (y1 < 0) y1 = 0;
    if (x2 >= LCD_WIDTH)  x2 = LCD_WIDTH  - 1;
    if (y2 >= LCD_HEIGHT) y2 = LCD_HEIGHT - 1;

    for (int row = y1; row <= y2; row++) {
        memset(&framebuffer[row][x1], color, (size_t)(x2 - x1 + 1));
    }
}

// ---------------------------------------------------------------------------
// render_invert_rect — XOR every pixel in the rectangle with 0x03
// Used for the HUD weapon-select highlight.
// ---------------------------------------------------------------------------
void render_invert_rect(int x1, int y1, int x2, int y2)
{
    if (x1 < 0) x1 = 0;
    if (y1 < 0) y1 = 0;
    if (x2 >= LCD_WIDTH)  x2 = LCD_WIDTH  - 1;
    if (y2 >= LCD_HEIGHT) y2 = LCD_HEIGHT - 1;

    for (int row = y1; row <= y2; row++) {
        for (int col = x1; col <= x2; col++) {
            framebuffer[row][col] ^= 0x03;
        }
    }
}

// ---------------------------------------------------------------------------
// render_get_pixel — read a single pixel from the framebuffer
// Returns 0 if out of bounds.
// ---------------------------------------------------------------------------
uint8_t render_get_pixel(int x, int y)
{
    if (x < 0 || x >= LCD_WIDTH || y < 0 || y >= LCD_HEIGHT) return 0;
    return framebuffer[y][x];
}
