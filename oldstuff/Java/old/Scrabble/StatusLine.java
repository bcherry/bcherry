/**
 * Programming Assignment 5 - Scrabble - UCSC CMPS12A
 * 
 * This source file contains the StatusLine class, which defines the line at the
 * bottom of the window that prints messages.
 * 
 * @author Ben Cherry (bcherry@ucsc.edu)
 * @author Torin Kampa (tkampa@ucsc.edu)
 * @author Bodhi Crandall-Rus (jcrandal@ucsc.edu)
 */

import java.awt.*;

class StatusLine {
    private String text = "";
    private Color color = Color.BLACK;

    /**
     * Prints out the status line to the window.
     * 
     * @param g
     *            The graphics to use.
     */
    public void print(Graphics g) {
        Color previous = g.getColor();
        g.setColor(color);
        g.drawString(text, 240 - (int) (text.length() * 2.85), Rack.y + 50);
        g.setColor(previous);
    }

    /**
     * Sets the status line to a new message.
     * 
     * @param text
     *            The message to set to the status line.
     */
    public void setText(String text) {
        this.text = text;
    }

    /**
     * Sets the status line to a new message and color.
     * 
     * @param text
     *            The message to set to the status line.
     * @param color
     *            The color to give the message.
     */
    public void setText(String text, Color color) {
        this.text = text;
        this.color = color;
    }
}