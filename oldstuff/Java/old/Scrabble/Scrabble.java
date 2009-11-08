
import java.awt.*;
import java.awt.event.*;

public class Scrabble extends Frame implements ActionListener, WindowListener,
        MouseListener, MouseMotionListener {
    Image backbuffer;
    Graphics backg;
    Button Submit = new Button("Submit");
    Button Reset = new Button("Reset");

    Board myBoard = new Board();
    Rack playerOne = new Rack();
    Rack playerTwo = new Rack();
    Rack currentPlayer = new Rack();
    TileBag tileBag = new TileBag();

    static final int RACKX = 145;
    static final int RACKY = 520;
    int playersTurn = 1;

    /**
     * Initializes all racks, window, listeners, and backbuffer.
     */
    Scrabble() {
        // setup each player's hand
        playerOne.fill(tileBag);
        playerTwo.fill(tileBag);
        setupRack(playerOne);
        setupRack(playerTwo);

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

        // whose turn is it
        if(playersTurn == 1)

            currentPlayer = playerOne;
        else
            currentPlayer = playerTwo;

        // draw the components
            myBoard.drawBoard(backg);
            drawUI();
            drawTiles();
            drawRack(currentPlayer);

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
        Font fontBig = new Font("Brush Script MT", Font.BOLD, 114 );

        int x = Board.XOFFSET;
        int y = RACKY-5;

        // print players turn
        backg.setFont(fontBold);
        backg.drawString("Player" + playersTurn + ":", RACKX, y);

        // print scores
        backg.drawString("Score:", x, y);
        backg.setColor(Color.BLUE);
        backg.setFont(font);
        backg.drawString("Player 1:   " + playerOne.score, x, y+(fontSize+2));
        backg.setColor(Color.RED);
        backg.drawString("Player 2:   " + playerTwo.score, x, y+(fontSize+2)*2);

        // transparent logo
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
            for(int i=0; i<Rack.NUM_TILES; i++)
                resetTile(currentPlayer, i);
        // submit turn
        else if(e.getSource() == Submit){
            // store letters on board in temp
            StringBuffer temp = new StringBuffer("");
            int points = 0, multiplier = 1;
            for(int i=0; i<currentPlayer.tiles.length(); i++){
                int xindex = currentPlayer.xindex[i], yindex = currentPlayer.yindex[i];
                if(xindex >= 0 && yindex >= 0){
                    myBoard.addTile(currentPlayer.tiles.charAt(i), xindex, yindex);
                    temp.append(currentPlayer.tiles.charAt(i));

                    // update player score
                    if(myBoard.tileKey[xindex][yindex] >= 4)
                        multiplier += myBoard.tileKey[xindex][yindex]-3;
                    else
                        points += TileBag.valueOf(currentPlayer.tiles.charAt(i)) *
                                myBoard.tileKey[xindex][yindex];
                }
            }
            currentPlayer.score += points*multiplier;
            // remove the letters on board from player's hand
            currentPlayer.remove(temp);
            // add 50 for using all tiles
            if(currentPlayer.tiles.length() == 0)
                currentPlayer.score += 50;
            // refill the player's hand
            currentPlayer.fill(tileBag);
            // switch players
            if(playersTurn == 1){
                // make new draggable tiles
                setupRack(playerOne);
                playersTurn = 2;
            }
            else{
                // make new draggable tiles
                setupRack(playerTwo);
                playersTurn = 1;
            }
        }
        // draw move
        render();
    }

    public void mousePressed(MouseEvent e) {
        // which tiles has been pressed
        for(int i=0; i<Rack.NUM_TILES; i++)
            if(         e.getX() >= currentPlayer.x[i] &&

                    e.getX() <= (currentPlayer.x[i] + Board.TILESIZE) &&
                    e.getY() >= currentPlayer.y[i] &&
                    e.getY() <= (currentPlayer.y[i] + Board.TILESIZE)){
                currentPlayer.dragging[i] = true;
                // makes sure only one tiles is chosen
                break;
            }
        e.consume();
    }

    public void mouseReleased(MouseEvent e) {
        for(int i=0; i<Rack.NUM_TILES; i++){
            int collisionOffset = Board.TILESIZE/2;
            // check if tile is on board
            if(!(       currentPlayer.x[i] >= Board.XOFFSET - collisionOffset &&
                    currentPlayer.x[i] < Board.XOFFSET+Board.SIZE - collisionOffset &&
                    currentPlayer.y[i] >= Board.YOFFSET - collisionOffset &&
                    currentPlayer.y[i] < Board.YOFFSET+Board.SIZE - collisionOffset - 5)){
                // moves tiles back to tray
                resetTile(currentPlayer, i);
            }

            // snap to board script
            else{
                // put in correct place and add that info to the tile
                currentPlayer.x[i] = (int)((currentPlayer.x[i]+Board.XOFFSET)/Board.TILESIZE)-1;
                currentPlayer.xindex[i] = currentPlayer.x[i];
                currentPlayer.x[i] = (int)currentPlayer.x[i]*Board.TILESIZE + Board.XOFFSET;
                currentPlayer.y[i] = (int)(currentPlayer.y[i]/Board.TILESIZE)-1;
                currentPlayer.yindex[i] = currentPlayer.y[i];
                currentPlayer.y[i] = (int)currentPlayer.y[i]*Board.TILESIZE + Board.YOFFSET;

                // dragged tile replaces tile on board if colliding
                for(int j=0; j<Rack.NUM_TILES; j++)
                    if(         currentPlayer.x[i] >= (currentPlayer.x[j] - Board.TILESIZE/2) &&
                            currentPlayer.x[i] <= (currentPlayer.x[j] + Board.TILESIZE - Board.TILESIZE/2) &&
                            currentPlayer.y[i] >= (currentPlayer.y[j] - Board.TILESIZE/2) &&
                            currentPlayer.y[i] <= (currentPlayer.y[j] + Board.TILESIZE - Board.TILESIZE/2) &&
                            !currentPlayer.dragging[i] && i != j){
                        currentPlayer.x[i] = currentPlayer.initx[i];
                        currentPlayer.y[i] = currentPlayer.inity[i];
                    }
            }
            // not dragging any tiles
            currentPlayer.dragging[i] = false;
        }
        render();
        e.consume();
    }

    public void mouseDragged(MouseEvent e) {
        // drag pieces

        for(int i=0; i<Rack.NUM_TILES; i++)
            if(currentPlayer.dragging[i]){
                currentPlayer.x[i] = e.getX()-Board.TILESIZE/2;
                currentPlayer.y[i] = e.getY()-Board.TILESIZE/2;
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

    public void resetTile(Rack player, int index){
        currentPlayer.x[index] = currentPlayer.initx[index];
        currentPlayer.y[index] = currentPlayer.inity[index];
        currentPlayer.xindex[index] = -1;
        currentPlayer.yindex[index] = -1;
    }

    // loads the rack with initial positions
    public void setupRack(Rack player){
        for(int i=0; i<Rack.NUM_TILES; i++){
            player.x[i] = i*30+RACKX;
            player.initx[i] = player.x[i];
            player.y[i] = RACKY;
            player.inity[i] = player.y[i];
            player.xindex[i] = -1;
            player.yindex[i] = -1;
        }
    }

    // draw the player tiles that are not yet part of the board
    public void drawRack(Rack player){
        for(int i=0; i<Rack.NUM_TILES; i++)
            drawTile(player.tiles.charAt(i), player.x[i], player.y[i]);
        // current player dragging tiles always on top
        for(int i=0; i<Rack.NUM_TILES; i++)
            if(player.dragging[i]) drawTile(player.tiles.charAt(i), player.x[i], player.y[i]);

    }

    // draw tiles already on the board
    public void drawTiles() {
        for (int x = 0; x < myBoard.tiles.length; x++) {
            for (int y = 0; y < myBoard.tiles[0].length; y++)
                if (myBoard.tiles[x][y] != ' ')
                    drawTileIndex(myBoard.tiles[x][y], x, y);
        }
    }

    // draw a tile based on its place on the board
    public void drawTileIndex(char letter, int x, int y) {
        x = x * Board.TILESIZE + Board.XOFFSET;
        y = y * Board.TILESIZE + Board.YOFFSET;
        drawTile(letter, x, y);
    }

    // draw a tile based on its screen position
    public void drawTile(char letter, int x, int y) {
        Color tan = new Color(255, 204, 153);
        Board.drawFillRect(backg, tan, Color.black, x, y, Board.TILESIZE,
                Board.TILESIZE);

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
