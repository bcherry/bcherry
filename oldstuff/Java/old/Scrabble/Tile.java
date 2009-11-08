/**
 * Programming Assignment 5 - Scrabble - UCSC CMPS12A
 * 
 * This source file contains the Tile class, which is a single Scrabble tile,
 * including an x and y coordinates, and a character.
 * 
 * @author Ben Cherry (bcherry@ucsc.edu)
 * @author Torin Kampa (tkampa@ucsc.edu)
 * @author Bodhi Crandall-Rus (jcrandal@ucsc.edu)
 */

class Tile {
    static final int SIZE = 30;
    char c = ' ';
    int x, y;
    boolean onBoard, dragging, selected;

    /**
     * Plain constructor. Makes a tile.
     */
    Tile() {}

    /**
     * Makes a tile with specific coordinates.
     * 
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    Tile(int x, int y) {
        this.x = x;
        this.y = y;
    }
}

