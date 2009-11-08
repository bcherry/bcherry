import java.util.Random;

class TileBag {
    private static final int[] values = {1,3,3,2,1,4,2,4,1,8,5,1,3,1,1,3,10,1,1,1,1,4,4,8,4,10};
    private StringBuffer tiles = new StringBuffer(100);
    private Random random;

    // Construct a bag of tiles using a random seed.
    TileBag() {
        // repetitions correspond to the scrabble rules for number of each letter
        tiles.append("AAAAAAAAABBCCDDDDEEEEEEEEEEEEFFGGGHHIIIIIIIII");
        tiles.append("JKLLLLMMNNNNNNOOOOOOOOPPQRRRRRRSSSSTTTTTTUUUUVVWWXYYZ");
        random = new Random();
    }

    // The seed is used to randomize the tiles in the bag.
    TileBag(int seed) {
        this();
        random = new Random(seed);
    }

    // How many tiles remain in the bag.
    int remaining() {
        return tiles.length();
    }

    /**
       @return one randomly drawn tile from the bag, removing the tile from the bag.
     */
    char draw() {
        int num = random.nextInt(tiles.length());
        char tile = tiles.charAt(num);
        tiles.deleteCharAt(num);
        return tile;
    }

    /**
       Return tiles to the bag.
       @param tiles - a StringBuffer of tiles to be returned to the bag
     */
    void returnTiles(StringBuffer tiles) {
        for (int i = 0; i < tiles.length(); i++) {
            this.tiles.append(tiles.charAt(i));
        }
    }

    /**
       Determine the value of a tile.
       @param c - the character that represents the tile (or non-tile)
       @return the value of the character if it is a tile character (a-z or ? representing blank),
           otherwise return 0 (blank also returns value  0)
     */
     static int valueOf(char c) {
        if (c < 'A' || c > 'Z')
            return 0;
        else
            return values[c-'A'];
    }

    /**
       Determine if a character is one represented by a tile.
       @param c - the character that represents the tile (or non-tile)
       @return true if the character is a tile character (a-z or ? representing blank),
           false otherwise
     */
    static boolean isTile(char c) {
        return ((c >= 'A' && c <= 'Z') || c == '?');
    }
}
