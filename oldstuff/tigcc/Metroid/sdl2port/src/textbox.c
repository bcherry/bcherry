/*
 * textbox.c - Text overlay display for Metroid TI-89 SDL2 port
 * Ported from original TIGCC textBox.c
 *
 * Original implementation:
 *   - Drew a filled white rectangle centered on screen
 *   - Drew the text string centered using GrayDrawStrExt with 8x10 font
 *   - Blocked until the player pressed ENTER
 *
 * SDL2 port implementation:
 *   - Stores the text and a countdown timer in globals
 *   - drawTextBox() renders the overlay each frame if active
 *   - updateTextBox() decrements the timer each frame
 *   - No blocking: the game loop continues running
 *
 * Since we don't have a full font rendering system, we draw a filled
 * rectangle and render a simple representation of the text using the
 * HUD digit sprites for any numeric characters, and filled rectangles
 * as placeholder blocks for alphabetic characters. A proper font system
 * can be added later.
 */
#include <string.h>

#include "textbox.h"
#include "render.h"

/* Global textbox state */
char textbox_text[TEXTBOX_MAX_LEN] = {0};
int textbox_timer = 0;

/*
 * textBox - Set the text overlay message and start the display timer.
 * This replaces the original blocking textBox() call.
 */
void textBox(const char* string) {
    if (string == NULL)
        return;
    strncpy(textbox_text, string, TEXTBOX_MAX_LEN - 1);
    textbox_text[TEXTBOX_MAX_LEN - 1] = '\0';
    textbox_timer = TEXTBOX_DURATION;
}

/*
 * updateTextBox - Tick down the textbox display timer.
 * Call this once per game frame.
 */
void updateTextBox(void) {
    if (textbox_timer > 0)
        textbox_timer--;
}

/*
 * drawTextBox - Render the textbox overlay if active.
 * Call this from drawScreen after drawing all game elements.
 *
 * Draws a centered white rectangle with a black border,
 * and renders each character as a small 4x6 block pattern.
 * This is a simple placeholder until a proper bitmap font is added.
 */
void drawTextBox(void) {
    if (textbox_timer <= 0)
        return;

    int len = (int)strlen(textbox_text);
    if (len == 0)
        return;

    /* Calculate box dimensions.
     * Each character is rendered as a 4px wide block with 1px spacing.
     * Total text width = len * 5 - 1 (no trailing space). */
    int text_width = len * 5 - 1;
    int padding = 6;
    int box_w = text_width + padding * 2;
    int box_h = 16;

    /* Center the box on the 160x100 screen */
    int box_x = (LCD_WIDTH - box_w) / 2;
    int box_y = (LCD_HEIGHT - box_h) / 2;

    /* Draw filled white background */
    render_fill_rect(box_x, box_y, box_x + box_w - 1, box_y + box_h - 1, COLOR_WHITE);

    /* Draw black border (top, bottom, left, right edges) */
    render_fill_rect(box_x, box_y, box_x + box_w - 1, box_y, COLOR_BLACK);
    render_fill_rect(box_x, box_y + box_h - 1, box_x + box_w - 1, box_y + box_h - 1, COLOR_BLACK);
    render_fill_rect(box_x, box_y, box_x, box_y + box_h - 1, COLOR_BLACK);
    render_fill_rect(box_x + box_w - 1, box_y, box_x + box_w - 1, box_y + box_h - 1, COLOR_BLACK);

    /* Render each character as a small filled block.
     * Uppercase letters and digits get a dark block, spaces are empty.
     * This is a minimal representation until a proper font is available. */
    int char_x = box_x + padding;
    int char_y = box_y + (box_h - 6) / 2;  /* Vertically centered, 6px tall chars */

    for (int i = 0; i < len; i++) {
        char c = textbox_text[i];
        if (c != ' ') {
            /* Draw a 4x6 filled block for each visible character */
            render_fill_rect(char_x, char_y, char_x + 3, char_y + 5, COLOR_BLACK);
        }
        char_x += 5;
    }
}
