/*
 * hud.c - HUD drawing for Metroid TI-89 SDL2 port
 * Ported from original TIGCC HUD.c
 *
 * The HUD slides in from the top when activated (cam->hud > 1 counting down)
 * and slides back out (cam->hud < 0 counting up toward 0).
 * When cam->hud == 0, the HUD is not drawn at all.
 * When cam->hud == 1, the HUD is fully visible at y=0.
 *
 * Layout (original TI-89, 160px wide):
 *   LEFT panel at x=32, 32px wide
 *   MIDDLE panel at x=64, 32px wide
 *   RIGHT panel at x=96 (LCD_WIDTH - 32 - 32), 32px wide
 *
 * Left panel: energy tanks (up to 12, in 2 rows of 6)
 * Middle panel: HP digits (2 digits), weapon icons
 * Right panel: ammo display for selected weapon
 */
#include "hud.h"
#include "render.h"
#include "map_data.h"
#include "gfx_data.h"
#include "sprites.h"
#include "samus.h"
#include "objects.h"
#include "game.h"

#define HUD_LEFT 32
#define HUD_RIGHT (LCD_WIDTH - 32 - HUD_LEFT)

/*
 * drawHud - Render the HUD overlay at the top of the screen.
 *
 * Slide animation:
 *   cam->hud > 1: sliding in, y = 1 - cam->hud (starts off-screen, moves to 0)
 *   cam->hud == 1: fully visible at y = 0
 *   cam->hud < 0: sliding out, y = -10 - cam->hud (moves off-screen)
 *   cam->hud == 0: not drawn
 */
void drawHud(void) {
    int x, y = 0, i;

    /* Handle slide-in animation */
    if (cam->hud > 1)
        y = 1 - cam->hud--;
    else if (cam->hud < 1) {
        /* Slide-out animation */
        y = -10 - cam->hud;
        if (++cam->hud > 0)
            cam->hud = 0;
    }

    /* If hud == 0, it was sliding out and has finished - don't draw */
    if (cam->hud == 0 && y == 0)
        return;

    /* Draw the three 32-wide HUD panel backgrounds using TRANW mode */
    blit_sprite_32_tranw(HUD_LEFT, y, 12,
        gfx->HUD.left_C0, gfx->HUD.left_C1);
    blit_sprite_32_tranw(HUD_LEFT + 32, y, 12,
        gfx->HUD.middle_C0, gfx->HUD.middle_C1);
    blit_sprite_32_tranw(HUD_RIGHT, y, 12,
        gfx->HUD.right_C0, gfx->HUD.right_C1);

    /* Draw energy tanks in the left panel.
     * Up to 12 tanks in 2 rows of 6, spaced 5px apart.
     * Full tanks use TRANB mode (OR), empty tanks use TRANW mode. */
    for (i = 0; i < samus->items.tanks_owned; i++) {
        int tank_x = HUD_LEFT + 4 + (i % 6) * 5;
        int tank_y = (i < 6) ? y + 1 : y + 5;

        if (i < samus->items.tanks_full) {
            /* Full tank - draw with OR (TRANB) mode */
            blit_sprite_8_tranb(tank_x, tank_y, 3,
                gfx->HUD.tankFull_C0, gfx->HUD.tankFull_C1);
        } else {
            /* Empty tank - draw with TRANW mode (transparent where white) */
            blit_sprite_8_tranw(tank_x, tank_y, 3,
                gfx->HUD.tankEmpty_C0, gfx->HUD.tankEmpty_C1);
        }
    }

    /* Draw HP as 2 digits in the middle panel.
     * The HUD number sprites are 8x7 indexed 0-9. */
    x = HUD_LEFT + 34;
    {
        int tens = samus->HP / 10;
        int ones = samus->HP % 10;

        /* Clamp to valid range */
        if (tens < 0) tens = 0;
        if (tens > 9) tens = 9;
        if (ones < 0) ones = 0;
        if (ones > 9) ones = 9;

        blit_sprite_8_tranb(x, y + 1, 7,
            gfx->HUD.numbers[tens].C0, gfx->HUD.numbers[tens].C1);
        blit_sprite_8_tranb(x + 7, y + 1, 7,
            gfx->HUD.numbers[ones].C0, gfx->HUD.numbers[ones].C1);
    }

    /* Draw weapon indicator icons (32-wide sprite with missile/super/bomb icons) */
    x = HUD_LEFT + 50;
    blit_sprite_32_tranw(x, y + 1, 8,
        gfx->HUD.weapons_C0, gfx->HUD.weapons_C1);

    /* Highlight the currently selected weapon by inverting its icon region.
     * Also determine the ammo count to display. */
    int ammo = 0;
    if (samus->weapons.gun == &templates->weapons.missile) {
        render_invert_rect(x, y + 1, x + 7, y + 8);
        ammo = samus->weapons.missile_ammo;
    } else if (samus->weapons.gun == &templates->weapons.superMissile) {
        render_invert_rect(x + 9, y + 1, x + 16, y + 8);
        ammo = samus->weapons.superMissile_ammo;
    } else if (samus->weapons.bomb == &templates->weapons.powerBomb) {
        render_invert_rect(x + 18, y + 1, x + 26, y + 8);
        ammo = samus->weapons.powerBomb_ammo;
    } else {
        /* Beam selected, no ammo display needed */
        return;
    }

    /* Draw ammo count as 2 digits in the right panel area */
    x = HUD_LEFT + 78;
    {
        int tens = ammo / 10;
        int ones = ammo % 10;

        if (tens < 0) tens = 0;
        if (tens > 9) tens = 9;
        if (ones < 0) ones = 0;
        if (ones > 9) ones = 9;

        blit_sprite_8_tranb(x, y + 1, 7,
            gfx->HUD.numbers[tens].C0, gfx->HUD.numbers[tens].C1);
        blit_sprite_8_tranb(x + 7, y + 1, 7,
            gfx->HUD.numbers[ones].C0, gfx->HUD.numbers[ones].C1);
    }
}
