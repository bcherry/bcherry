class Rack {
    static final int NUM_TILES = 7;
    StringBuffer tiles = new StringBuffer(NUM_TILES);

    // tile coordinate position
    int[] x = new int[NUM_TILES];
    int[] y = new int[NUM_TILES];
    // tile coordinate position in rack
    int[] initx = new int[NUM_TILES];
    int[] inity = new int[NUM_TILES];
    // tile position on board
    int[] xindex = new int[NUM_TILES];
    int[] yindex = new int[NUM_TILES];
    // currently being dragged
    boolean[] dragging = new boolean[NUM_TILES];
    int score = 0;

    // refill the rack with tiles for the tile bag
    void fill(TileBag bag) {
                while (tiles.length() < NUM_TILES) {
                    tiles.append(bag.draw());
                }
    }

    // checks to see if word can be made from the rack tiles
    boolean contains(StringBuffer word) {
                StringBuffer temp = new StringBuffer(tiles.toString());
                for (int i = 0; i < word.length(); i++) {
                            if (!remove(word.charAt(i),temp)) {
                                        // the letter was not in the word
                                        return false;
                            }
                }
                return true;
    }

    // removes the word from the rack
    boolean remove(StringBuffer word) {
        if (contains(word)) {
                    //  all letters are in the hand
                    for (int i = 0; i < word.length(); i++)
                        remove(word.charAt(i),tiles);
                    return true;
                }
                else
                    return false;
    }

    // returns a string of the tiles
    public String toString() {
        return tiles.toString();
    }
    
     // removes a char from a StringBuffer
    static boolean remove(char c, StringBuffer sbuf) {
                for(int i = 0; i < sbuf.length(); i++) {
                    if (sbuf.charAt(i) == c) {
                        sbuf.deleteCharAt(i);
                        return true;
                    }
                }
                return false;
    }
}

