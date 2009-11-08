/**
   This represents
 */
class Rack {
    static final int NUM_TILES = 7;
    StringBuffer tiles = new StringBuffer(NUM_TILES);
    void fill(Tiles bag) {
	while (tiles.length() < NUM_TILES) {
	    tiles.append(bag.draw());
	}
    }
    boolean contains(StringBuffer word) {
	StringBuffer temp = new StringBuffer(tiles.toString());
	for (int i = 0; i < word.length(); i++) {
	    if (!remove(word.charAt(i),temp)) {
		// the letter was not in the word
		return false;
	    }
	}
	// all letters in word were successfully removed from the temp copy 
	// so the hand contained all of the letters in word
	return true;
    }
    boolean remove(StringBuffer word) {
	if (contains(word)) {
	    //  all letters are in the hand
	    for (int i = 0; i < word.length(); i++) {
		remove(word.charAt(i),tiles);
	    }
	    return true;
	}
	else {
	    return false;
	}
    }
    public String toString() {
	return tiles.toString();
    }
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
