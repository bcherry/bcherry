class Tile {
    static final int SIZE = 30;
    char c = ' ';
    int x, y;
    boolean onBoard = false;
    boolean dragging;
    Tile() { }
    Tile(int x, int y) {
        this.x = x;
        this.y = y;
    }
}