import java.awt.*;

public class Board {
    // setup table
    public static final int TILESIZE = 30, DIMESNION = 15, XOFFSET = 25,
            YOFFSET = 45;
    public static final int SIZE = DIMESNION * TILESIZE;
    public char[][] tiles = new char[DIMESNION][DIMESNION];

    // setup colors/multipliers
    public final Color t = new Color(204, 153, 102); // tan
    public final Color r = new Color(255, 0, 0); // red
    public final Color p = new Color(255, 153, 153); // pink
    public final Color b = new Color(0, 0, 255); // blue
    public final Color l = new Color(153, 204, 255); // light blue
    public final Color[] colorKey = { null, t, l, b, p, r };
    // this combines the colors of the board based on colorKey[]
    // add the multipliers for tile scores - 2 and 3 are tile multipliers
    // and (4,5)-2 for the word multipliers
    public final int[][] tileKey = {
            { 5, 1, 1, 2, 1, 1, 1, 5, 1, 1, 1, 2, 1, 1, 5 },
            { 1, 4, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 4, 1 },
            { 1, 1, 4, 1, 1, 1, 2, 1, 2, 1, 1, 1, 4, 1, 1 },
            { 2, 1, 1, 4, 1, 1, 1, 2, 1, 1, 1, 4, 1, 1, 2 },
            { 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1 },
            { 1, 3, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 3, 1 },
            { 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1 },
            { 5, 1, 1, 2, 1, 1, 1, 4, 1, 1, 1, 2, 1, 1, 5 },
            { 1, 1, 2, 1, 1, 1, 2, 1, 2, 1, 1, 1, 2, 1, 1 },
            { 1, 3, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 3, 1 },
            { 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1 },
            { 2, 1, 1, 4, 1, 1, 1, 2, 1, 1, 1, 4, 1, 1, 2 },
            { 1, 1, 4, 1, 1, 1, 2, 1, 2, 1, 1, 1, 4, 1, 1 },
            { 1, 4, 1, 1, 1, 3, 1, 1, 1, 3, 1, 1, 1, 4, 1 },
            { 5, 1, 1, 2, 1, 1, 1, 5, 1, 1, 1, 2, 1, 1, 5 } };

    // load tiles[][] with ' '
    Board() {
        for (int i = 0; i < tiles.length; i++)
            for (int j = 0; j < tiles[0].length; j++)
                tiles[i][j] = ' ';
    }

    // add a tile to the baord
    public void addTile(char letter, int x, int y) {
        tiles[x][y] = letter;
    }

    // draw the board 
    public void drawBoard(Graphics g) {
        for (int x = 0; x < DIMESNION; x++)
            for (int y = 0; y < DIMESNION; y++)
                drawFillRect(g, colorKey[tileKey[y][x]], Color.black, x
                        * TILESIZE + XOFFSET, y * TILESIZE + YOFFSET, TILESIZE,
                        TILESIZE);
    }

    // function that combines drawRect() and fillRect()
    public static void drawFillRect(Graphics g, Color fill, Color border,
            int x, int y, int w, int h) {
        g.setColor(fill);
        g.fillRect(x + 1, y + 1, w, h);
        g.setColor(border);
        g.drawRect(x, y, w, h);
    }
}
