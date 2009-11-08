class Rack {
    static final int NUM_TILES = 7;
    Tile[] tiles = new Tile[NUM_TILES];
    int score;
    static final int x = 145, y = 520;
    
    Rack(TileBag bag) {
        for (int i = 0; i < NUM_TILES; i++) {
            tiles[i] = new Tile();
            tiles[i].c = bag.draw();
        }
        reset();
    }
    
    // refill the rack with tiles from the tile bag
    void refill(TileBag bag) {
        for (int i = 0; i < NUM_TILES; i++) {
            if (tiles[i].onBoard) {
                tiles[i].onBoard = false;
                tiles[i].c = bag.draw();
            }
        }
    }
    
    void reset() {
        for (int i=0; i<NUM_TILES; i++) {
            resetTile(i);
        }
    }
    
    void resetTile(int i) {
        tiles[i].x = x + Tile.SIZE*i;
        tiles[i].y = y;
        tiles[i].onBoard = false;
        tiles[i].dragging = false;
    }
    
    char at(int i) { return tiles[i].c; }
    
    char findTile(int x, int y){
        //System.out.println("findTile: x is " + x + " y is " + y);
        for(int i=0; i<NUM_TILES; i++)
            if(Board.getXIndex(tiles[i].x) == x && Board.getYIndex(tiles[i].y) == y)
                return tiles[i].c; 
        return ' ';
    }
}
