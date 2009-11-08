
import java.awt.*;
import java.awt.event.*;

public class Scrabble extends Frame implements ActionListener, WindowListener,
        MouseListener, MouseMotionListener {
    Image backbuffer;
    Graphics backg;
    Button Submit = new Button("Submit");
    Button Reset = new Button("Reset");

    Board board = new Board();
    TileBag tileBag = new TileBag();
    Rack playerOne = new Rack(tileBag);
    Rack playerTwo = new Rack(tileBag);
    Rack who = playerOne;

    /**
     * Initializes all racks, window, listeners, and backbuffer.
     */
    Scrabble() {
        // set window properties
        setTitle("Scrabble");
        setSize(500, 600);

        // setup layout for buttons
        setLayout(new BorderLayout());
        Panel p = new Panel();
        p.setLayout(new FlowLayout(FlowLayout.CENTER));
        Panel p2 = new Panel();
        p2.setLayout(new GridLayout(1,0,10,10));
        p2.add(Reset);
        p2.add(Submit);
        p.add(p2, BorderLayout.CENTER);
        add(p, BorderLayout.SOUTH);

        // button listeners
        Submit.addActionListener(this);
        Reset.addActionListener(this);

        // mouse listeners
        addMouseListener(this);
        addMouseMotionListener(this);

        // windows listener so exit button works
        addWindowListener(this);

        // display the window
        show();

        // setup backbuffer which renders applet to an image and then renders
        // that image on screen. this eliminates flickering.
        backbuffer = createImage(getSize().width, getSize().height);
        backg = backbuffer.getGraphics();

        // white background
        Color bg = getBackground();
        backg.setColor(bg);

        // inital render
        render();
    }

    /**
     * Creates and initalizes new Scrabble object.
     * @param args Passed to scrabble on program initialization.
     */
    public static void main(String[] args){
        Scrabble scrabble = new Scrabble();
    }

    /**
     * Paints the window. Called by repaint().
     */
    public void paint(Graphics g) {
        // call seperate function to prevent flickering errors
        update(g);
    }

    /**
     * Renders the backbuffer to screen.
     */
    public void update(Graphics g) {
        g.drawImage(backbuffer, 0, 0, this);
    }

    /**
     * Clears the screen, renders components to backbuffer, and repaint().
     */
    public void render(){
        // clear the screen
        backg.clearRect(0,0,getWidth(),getHeight());

        // draw the components
        board.drawBoard(backg);
        drawUI();
        drawTiles();
        drawRack(who);

        // paint()
        repaint();
    }

    /**
     * Draw the UI components including the score and turn.
     */
    public void drawUI(){
        // fonts
        int fontSize = 12;
        Font font = new Font("Arial", Font.PLAIN, fontSize );
        Font fontBold = new Font("Arial", Font.BOLD, fontSize );

        int x = Board.XOFFSET;
        int y = Rack.y-5;

        // print players turn
        backg.setFont(fontBold);
        if (who == playerOne){
            backg.setColor(Color.BLUE);
            backg.drawString("Player 1:", Rack.x, y);
        } else {
            backg.setColor(Color.RED);
            backg.drawString("Player 2:", Rack.x, y);
        }
        
        // print scores
        backg.drawString("Score:", x, y);
        backg.setColor(Color.BLUE);
        backg.setFont(font);
        backg.drawString("Player 1:   " + playerOne.score, x, y+(fontSize+2));
        backg.setColor(Color.RED);
        backg.drawString("Player 2:   " + playerTwo.score, x, y+(fontSize+2)*2);

        // transparent logo
//        Font fontBig = new Font("Brush Script MT", Font.BOLD, 114 );
//        Color transWhite = new Color(255, 255, 255, 100);
//        Color transBlack = new Color(0, 0, 0, 50);
//        backg.setFont(fontBig);
//        backg.setColor(transBlack);
//        backg.drawString("Scrabble!", Board.XOFFSET+12, Board.YOFFSET+253);
//        backg.setColor(transWhite);
//        backg.drawString("Scrabble!", Board.XOFFSET+10, Board.YOFFSET+250);
    }

    /**
     * Handles the listeners for the Reset and Submit buttons.
     */
public void actionPerformed(ActionEvent e) {
        // take the unlocked tiles off the table
        if(e.getSource() == Reset)
            who.reset();
        // submit turn
        else if(e.getSource() == Submit){
            int score = validMove(who);
            System.out.println(score);
            if(score > 0) {
                who.score += score;
                for(int i=0; i<Rack.NUM_TILES; i++)
                    if(who.tiles[i].onBoard)
                        board.addTile(who.tiles[i].c, Board.getXIndex(who.tiles[i].x), Board.getYIndex(who.tiles[i].y));
                // store letters on board in temp
//                StringBuffer temp = new StringBuffer("");
//                int points = 0, multiplier = 1;
//                for(int i=0; i<Rack.NUM_TILES; i++){
//                    int xindex = who.xindex[i], yindex = who.yindex[i];
//                    if(xindex >= 0 && yindex >= 0){
//                        board.addTile(who.tiles.charAt(i), xindex, yindex);
//                        temp.append(who.tiles.charAt(i));
//    
//                        // update player score
//                        if(board.tileKey[xindex][yindex] >= 4)
//                            multiplier += board.tileKey[xindex][yindex]-3;
//                        else
//                            points += TileBag.valueOf(who.tiles.charAt(i)) *
//                                    board.tileKey[xindex][yindex];
//                    }
//                }
//                who.score += points*multiplier;
//                // remove the letters on board from player's hand
//                who.remove(temp);
//                // add 50 for using all tiles
//                if(who.tiles.length() == 0)
//                    who.score += 50;
                // refill the player's hand
//                who.fill(tileBag);
                // switch players
                who.reset();
                who.refill(tileBag);
                // whose turn is it
                if(who == playerOne)
                    who = playerTwo;
                else
                    who = playerOne;
            }
            else{
                System.out.println("Invalid input!");
            }
            //}
        }
        // draw move
        render();
    }

    public void mousePressed(MouseEvent e) {
        // which one of you tiles has been pressed
        for(int i=0; i<Rack.NUM_TILES; i++)
            if(     e.getX() >= who.tiles[i].x &&
                    e.getX() <= (who.tiles[i].x + Tile.SIZE) &&
                    e.getY() >= who.tiles[i].y &&
                    e.getY() <= (who.tiles[i].y + Tile.SIZE)){
                who.tiles[i].dragging = true;
                // makes sure only one tiles is chosen
                break;
            }
        e.consume();
    }

    public void mouseReleased(MouseEvent e) {
        int i;
        for(i=0; i<Rack.NUM_TILES; i++){
            if (who.tiles[i].dragging) break;
        }
        int collisionOffset = Tile.SIZE/2;
        // check if tile is on board
        if(!(   who.tiles[i].x >= Board.XOFFSET - collisionOffset &&
                who.tiles[i].x < Board.XOFFSET+Board.SIZE - collisionOffset &&
                who.tiles[i].y >= Board.YOFFSET - collisionOffset &&
                who.tiles[i].y < Board.YOFFSET+Board.SIZE - collisionOffset - 5)){
            // moves tiles back to tray
            who.resetTile(i);
        }

        // snap to board script
        else{
            // put in correct place and add that info to the tile
            who.tiles[i].x = (int)((who.tiles[i].x+Board.XOFFSET)/Tile.SIZE)-1;
            who.tiles[i].x = (int)who.tiles[i].x*Tile.SIZE + Board.XOFFSET;
            who.tiles[i].y = (int)(who.tiles[i].y/Tile.SIZE)-1;
            who.tiles[i].y = (int)who.tiles[i].y*Tile.SIZE + Board.YOFFSET;
            who.tiles[i].onBoard = true;
            
            // space on board occupied?
            if(!board.isOpen(Board.getXIndex(who.tiles[i].x), Board.getYIndex(who.tiles[i].y)))
                who.resetTile(i);
                
            // dragged tile replaces tile on board if colliding
            for(int j=0; j<Rack.NUM_TILES; j++)
                if(     who.tiles[i].x >= (who.tiles[j].x - Tile.SIZE/2) &&
                        who.tiles[i].x <= (who.tiles[j].x + Tile.SIZE - Tile.SIZE/2) &&
                        who.tiles[i].y >= (who.tiles[j].y - Tile.SIZE/2) &&
                        who.tiles[i].y <= (who.tiles[j].y + Tile.SIZE - Tile.SIZE/2) &&
                        !who.tiles[i].dragging && i != j){
                    who.resetTile(i);
                }
        }
        // not dragging any tiles
        who.tiles[i].dragging = false;
        
        render();
        e.consume();
    }

    public void mouseDragged(MouseEvent e) {
        // drag pieces

        for(int i=0; i<Rack.NUM_TILES; i++)
            if(who.tiles[i].dragging){
                who.tiles[i].x = e.getX()-Tile.SIZE/2;
                who.tiles[i].y = e.getY()-Tile.SIZE/2;
            }

        render();
        e.consume();
    }

    public void mouseMoved(MouseEvent e) {}
    public void mouseEntered(MouseEvent e) {}
    public void mouseExited(MouseEvent e) {}
    public void mouseClicked(MouseEvent e) {}

    public void windowClosing(WindowEvent arg0) {
        this.dispose();
        System.exit(0);
    }
    public void windowActivated(WindowEvent arg0) {}
    public void windowClosed(WindowEvent arg0) {}
    public void windowDeactivated(WindowEvent arg0) {}
    public void windowDeiconified(WindowEvent arg0) {}
    public void windowIconified(WindowEvent arg0) {}
    public void windowOpened(WindowEvent arg0) {}

    public int validMove(Rack rack) {
        int x = -1, multiplier = 1, xmin = 0, xmax = 0, ymin = 0, ymax = 0;
        boolean horiz = true, vert = true;
        int score = 0;
        for (int i=0; i<Rack.NUM_TILES; i++) {
            if (rack.tiles[i].onBoard) {
                System.out.println("Raw X is " + rack.tiles[i].x + " Index is " + Board.getXIndex(rack.tiles[i].x));
                if (x == -1) {// set initial value
                    x = Board.getXIndex(rack.tiles[i].x);
                    System.out.println("X: " + x);
                    xmin = x;
                    xmax = x;
                } else if(x != Board.getXIndex(rack.tiles[i].x))
                    vert = false;
                if (Board.getXIndex(rack.tiles[i].x) > xmax)
                    xmax = Board.getXIndex(rack.tiles[i].x);
                else if (Board.getXIndex(rack.tiles[i].x) < xmin)
                    xmin = Board.getXIndex(rack.tiles[i].x);
            }
        }
        int y = -1;
        for (int i=0; i<Rack.NUM_TILES; i++) {
            if (rack.tiles[i].onBoard) {
                System.out.println("Raw Y is " + rack.tiles[i].y + " Index is " + Board.getYIndex(rack.tiles[i].y));
                if (y == -1) {// set initial value
                    y = Board.getYIndex(rack.tiles[i].y);
                    System.out.println("Y: " + y);
                    ymin = y;
                    ymax = y;
                } else if(y != Board.getYIndex(rack.tiles[i].y))
                    horiz = false;
                if (Board.getYIndex(rack.tiles[i].y) > ymax)
                    ymax = Board.getYIndex(rack.tiles[i].y);
                else if (Board.getYIndex(rack.tiles[i].y) < ymin)
                    ymin = Board.getYIndex(rack.tiles[i].y);
            }
        }
        System.out.println("H: " + horiz);
        System.out.println("V: " + vert);
        if (vert && horiz)     
            return 0;        
        if (horiz) {
            /*while(!board.isOpen(xmin - 1, y)) {
                if(!board.isOpen(xmin - 1, y))
                    xmin--;
            }*/
            
            while(!board.isOpen(xmin, y) && xmin > 0)
                xmin--;
            for (int i = xmin; i < Board.DIMENSION; i++) {
                char c;
                //System.out.println("pre findtile H " + y);
                if(board.isOpen(i, y) && rack.findTile(i, y) == ' ') {
                    if (i < xmax) {
                        System.out.println("its here, h" + (score*multiplier));
                        return 0;
                    } else
                        return score * multiplier;
                } else if (!board.isOpen(i, y)) {
                    c = board.tiles[i][y];
                } else {
                    c = rack.findTile(i, y);
                }
                int mult;
                if (!((mult = board.tileKey[i][y]) <= 3))
                    mult = 1;
                score += TileBag.valueOf(c)*mult;
                if(board.tileKey[i][y] > 3) {
                    if (multiplier == 1)
                        multiplier--;
                    multiplier += board.tileKey[i][y] - 2;
                }
            }
            return score * multiplier;
        } else {
            while(!board.isOpen(x, ymin) && ymin > 0)
                ymin--;
            for (int i = ymin; i < Board.DIMENSION; i++) {
                char c;
                //System.out.println("pre findtile V " + x);
                if(board.isOpen(x, i) && rack.findTile(x, i) == ' ') {
                    if (i < ymax) {
                        System.out.println("its here, v" + (score*multiplier));
                        return 0;
                    } else
                        return score * multiplier;
                } else if (!board.isOpen(x, i)) {
                    c = board.tiles[x][i];
                } else {
                    c = rack.findTile(x, i);
                }
                int mult;
                if (!((mult = board.tileKey[x][i]) <= 3))
                    mult = 1;
                score += TileBag.valueOf(c)*mult;
                if(board.tileKey[x][i] > 3){
                    if (multiplier == 1)
                        multiplier--;
                    multiplier += board.tileKey[x][i] - 2;
                }
            }
            return score * multiplier;
        }
    }

    // draw the player tiles that are not yet part of the board
    public void drawRack(Rack player){
        for(int i=0; i<Rack.NUM_TILES; i++)
            drawTile(player.at(i), player.tiles[i].x, player.tiles[i].y);
        // current player dragging tiles always on top
        for(int i=0; i<Rack.NUM_TILES; i++)
            if(player.tiles[i].dragging) drawTile(player.at(i), player.tiles[i].x, player.tiles[i].y);

    }

    // draw tiles already on the board
    public void drawTiles() {
        for (int x = 0; x < board.tiles.length; x++) {
            for (int y = 0; y < board.tiles[0].length; y++)
                if (board.tiles[x][y] != ' ')
                    drawTileIndex(board.tiles[x][y], x, y);
        }
    }

    // draw a tile based on its place on the board
    public void drawTileIndex(char letter, int x, int y) {
        x = x * Tile.SIZE + Board.XOFFSET;
        y = y * Tile.SIZE + Board.YOFFSET;
        drawTile(letter, x, y);
    }

    // draw a tile based on its screen position
    public void drawTile(char letter, int x, int y) {
        Color tan = new Color(255, 204, 153);
        Board.drawFillRect(backg, tan, Color.black, x, y, Tile.SIZE,
                Tile.SIZE);

        // draw letter
        Font font = new Font("Arial", Font.BOLD, 16);
        backg.setFont(font);
        // special case
        if(letter == 'I')
            backg.drawString(letter + "", x + 13, y + 20);
        else
            backg.drawString(letter + "", x + 9, y + 20);

        // draw value
        font = new Font("Arial", Font.BOLD, 10);
        backg.setFont(font);
        // special cases
        if(letter == 'Z' || letter == 'Q')
            backg.drawString(TileBag.valueOf(letter) + "", x + 16, y + 28);
        else
            backg.drawString(TileBag.valueOf(letter) + "", x + 21, y + 28);
    }
}
