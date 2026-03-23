/*
 * textbox.h - Text overlay display for Metroid TI-89 SDL2 port
 * Ported from original TIGCC textBox.c
 *
 * The original blocked the game loop waiting for ENTER.
 * This port uses a timed overlay instead - set the text and a countdown
 * timer, and the render loop draws it until the timer expires.
 */
#ifndef TEXTBOX_H
#define TEXTBOX_H

/* Maximum length of a textbox message */
#define TEXTBOX_MAX_LEN 32

/* Duration in frames to display the textbox */
#define TEXTBOX_DURATION 120

/* Global textbox state */
extern char textbox_text[TEXTBOX_MAX_LEN];
extern int textbox_timer;

/* Show a text message as a timed overlay */
void textBox(const char* string);

/* Draw the textbox if active (call from drawScreen) */
void drawTextBox(void);

/* Tick down the textbox timer (call from game update) */
void updateTextBox(void);

#endif /* TEXTBOX_H */
