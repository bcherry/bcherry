/**
 * Programming Assignment 5 - Scrabble - UCSC CMPS12A
 * 
 * This source file contains the Game class, which contains most of the core
 * game code for the scrabble game.
 * 
 * @author Ben Cherry (bcherry@ucsc.edu)
 * @author Torin Kampa (tkampa@ucsc.edu)
 * @author Bodhi Crandall-Rus (jcrandal@ucsc.edu)
 */

import java.awt.*;
import java.awt.event.*;
import java.io.*;
import java.net.*;
import javax.swing.*;

class Game extends JComponent implements ActionListener, MouseListener,
        MouseMotionListener, WindowListener {
    Board board = new Board();
    TileBag tileBag = new TileBag();
    Rack playerOne = new Rack(tileBag, "Super", Color.BLUE);
    Rack playerTwo = new Rack(tileBag, "Duper", Color.RED.darker());
    Rack who = playerOne;
    StatusLine status = new StatusLine();
    boolean noTesting = false;
    Button reset, submit, dump, pass, challenge;

    /**
     * Builds the game.
     * 
     * @param one Reset button.
     * @param two Submit button.
     * @param three Dump button.
     * @param four Pass button.
     * @param five Challenge button.
     */
    Game(Button one, Button two, Button three, Button four, Button five) {
        reset = one;
        submit = two;
        dump = three;
        pass = four;
        challenge = five;
        submit.setEnabled(false);
        reset.setEnabled(false);
        challenge.setEnabled(false);

        status.setText("Please play a word with one letter on the middle " + 
                "of the board.");
    }

    /**
     * Paints the window.
     * 
     * @param g The graphics to be used.
     */
    public void paint(Graphics g) {
        board.draw(g);
        drawUI(g);
        drawTiles(g);
    }

    /**
     * Draw the UI components including the score and turn.
     * 
     * @param g The graphics to be used.
     */
    public void drawUI(Graphics g) {
        // fonts
        int fontSize = 12;
        Font font = new Font("Arial", Font.PLAIN, fontSize);
        Font fontBold = new Font("Arial", Font.BOLD, fontSize);
        Font fontSmall = new Font("Arial", Font.PLAIN, 9);

        int x = Board.XOFFSET;
        int y = Rack.y - 3;

        // print players turn
        g.setFont(fontBold);
        g.setColor(who.color);
        g.drawString(who + "'s Turn:", Rack.x, y);

        // print scores
        g.setColor(Color.BLACK);
        g.drawString("Score:", x, y);
        g.setFont(font);
        g.setColor(playerOne.color);
        g.drawString("  " + playerOne + ": " + playerOne.score, x, y + 1
                * (fontSize + 2));
        g.setColor(playerTwo.color);
        g.drawString("  " + playerTwo + ": " + playerTwo.score, x, y + 2
                * (fontSize + 2));

        // print key
        x = 365;
        y = Rack.y - 8;
        int size = 8;
        g.setFont(fontSmall);

        g.setColor(Board.l);
        Scrabble.drawPiece(g, x, y + (size + 2) * 0, size, size);
        g.setColor(Color.BLACK);
        g.drawString("Double Letter Score", x + 12, y + size + (size + 2) * 0);

        g.setColor(Board.b);
        Scrabble.drawPiece(g, x, y + (size + 2) * 1, size, size);
        g.setColor(Color.BLACK);
        g.drawString("Triple Letter Score", x + 12, y + size + (size + 2) * 1);

        g.setColor(Board.p);
        Scrabble.drawPiece(g, x, y + (size + 2) * 2, size, size);
        g.setColor(Color.BLACK);
        g.drawString("Double Word Score", x + 12, y + size + (size + 2) * 2);

        g.setColor(Board.r);
        Scrabble.drawPiece(g, x, y + (size + 2) * 3, size, size);
        g.setColor(Color.BLACK);
        g.drawString("Triple Word Score", x + 12, y + size + (size + 2) * 3);

        // This is the optional logo on the board.
        // It requires the font Brush Script MT and slows down the program.
        // By default we have disabled it.
        //        Color transWhite = new Color(255, 255, 255, 75);
        //        Color transBlack = new Color(0, 0, 0, 38);
        //        Font fontBig = new Font("Brush Script MT", Font.BOLD, 114 );
        //        g.setFont(fontBig);
        //        g.setColor(transBlack);
        //        g.drawString("Scrabble!", Board.XOFFSET+12, Board.YOFFSET+253);
        //        g.setColor(transWhite);
        //        g.drawString("Scrabble!", Board.XOFFSET+10, Board.YOFFSET+250);
        //        g.setFont(font);

        g.setFont(font);
        status.print(g);
    }

    /**
     * Handles the listeners for the buttons.
     * 
     * @param e An ActionEvent to use.
     */
    public void actionPerformed(ActionEvent e) {
        // take the unlocked tiles off the table
        if (e.getSource() == reset) {
            who.reset();
            resetStatus();
        }
        // submit turn
        else if (e.getSource() == dump) {
            if (!who.dumping) {
                who.dumping = true;
                status.setText("Select which tiles you want to dump." + 
                        " Press Dump to exit.");
            } else {
                who.dumping = false;
                if (who.dump(tileBag))
                    switchTurns();
                resetStatus();
            }
        } else if (e.getSource() == pass) {
            who.reset();
            who.pass = true;
            switchTurns();
            resetStatus();
            if (who.pass)
                gameEnd();
        } else if (e.getSource() == submit) {
            int score = getScore(who, false);
            if (score > 0) {
                // check for all tiles played
                for (int i = 0; i < Rack.NUM_TILES; i++)
                    if (!who.tiles[i].onBoard) {
                        score -= 50;
                        break;
                    }
                // update player's score
                who.score += score + 50;
                // add the played tiles to the board
                for (int i = 0; i < Rack.NUM_TILES; i++)
                    if (who.tiles[i].onBoard)
                        board.addTile(who.tiles[i].c, Board
                                .getXIndex(who.tiles[i].x), Board
                                .getYIndex(who.tiles[i].y));
                // switch players
                who.refill(tileBag);
                if (tileBag.remaining() <= 0)
                    dump.setEnabled(false);
                who.reset();
                who.pass = false;
                // whose turn is it
                switchTurns();
            }
            resetStatus();
        } else if (e.getSource() == challenge) {
            int score = getScore(who, true);
            if (score < 0) {
                //challenge successful
                who.reset();
                String text = who + " lost his turn!";
                switchTurns();
                status.setText(who + "'s challenge was successful! " + text);
            } else if (score > 0) {
                // check for all tiles played
                for (int i = 0; i < Rack.NUM_TILES; i++)
                    if (!who.tiles[i].onBoard) {
                        score -= 50;
                        break;
                    }
                // update player's score
                who.score += score + 50;
                // add the played tiles to the board
                for (int i = 0; i < Rack.NUM_TILES; i++)
                    if (who.tiles[i].onBoard)
                        board.addTile(who.tiles[i].c, Board
                                .getXIndex(who.tiles[i].x), Board
                                .getYIndex(who.tiles[i].y));
                // switch players
                who.refill(tileBag);
                if (tileBag.remaining() <= 0)
                    dump.setEnabled(false);
                switchTurns();
                status.setText(who
                        + " made a failed challenge and lost a turn!");
                switchTurns();
                who.reset();
                who.pass = false;
            }
        }

        this.repaint();
    }

    /**
     * Switches turns between the two players
     */
    public void switchTurns() {
        if (who == playerOne)
            who = playerTwo;
        else
            who = playerOne;
        submit.setEnabled(false);
        challenge.setEnabled(false);
        reset.setEnabled(false);
    }

    /**
     * Quits the program when the window is closed.
     */
    public void windowClosing(WindowEvent arg0) {
        System.exit(0);
    }

    public void windowActivated(WindowEvent arg0) {}
    public void windowClosed(WindowEvent arg0) {}
    public void windowDeactivated(WindowEvent arg0) {}
    public void windowDeiconified(WindowEvent arg0) {}
    public void windowIconified(WindowEvent arg0) {}
    public void windowOpened(WindowEvent arg0) {}

    /**
     * Handles the pressing of the mouse, to begin dragging a tile.
     */
    public void mousePressed(MouseEvent e) {
        // which one of you tiles has been pressed
        for (int i = 0; i < Rack.NUM_TILES; i++)
            if (e.getX() >= who.tiles[i].x
                    && e.getX() <= (who.tiles[i].x + Tile.SIZE)
                    && e.getY() >= who.tiles[i].y
                    && e.getY() <= (who.tiles[i].y + Tile.SIZE)) {
                who.tiles[i].dragging = true;
                // makes sure only one tiles is chosen
                break;
            }
        resetStatus();
        e.consume();
    }

    /**
     * Handles the release of the mouse, dropping the tile held.
     */
    public void mouseReleased(MouseEvent e) {
        int dragIndex = -1;
        for (int i = 0; i < Rack.NUM_TILES; i++) {
            if (who.tiles[i].dragging) {
                dragIndex = i;
                break;
            }
        }
        if (dragIndex == -1)
            return;

        // check if tile is on board
        if (!board.onBoard(who.tiles[dragIndex])) {
            who.resetTile(dragIndex);
        }

        // snap to board script
        else {
            // put in correct place and add that info to the tile
            who.tiles[dragIndex].x = Board.getXIndex(who.tiles[dragIndex].x);
            who.tiles[dragIndex].x = who.tiles[dragIndex].x * Tile.SIZE
                    + Board.XOFFSET;
            who.tiles[dragIndex].y = Board.getYIndex(who.tiles[dragIndex].y);
            who.tiles[dragIndex].y = who.tiles[dragIndex].y * Tile.SIZE
                    + Board.YOFFSET;
            who.tiles[dragIndex].onBoard = true;

            // space on board occupied?
            if (!board.isOpen(Board.getXIndex(who.tiles[dragIndex].x), Board
                    .getYIndex(who.tiles[dragIndex].y)))
                who.resetTile(dragIndex);

            // dragged tile replaces tile on board if colliding
            for (int i = 0; i < Rack.NUM_TILES; i++)
                if (who.tileCollision(dragIndex, i) && dragIndex != i)
                    who.resetTile(i);
        }
        // not dragging any tiles
        who.tiles[dragIndex].dragging = false;

        statusCheckMove();
        resetStatus();
        this.repaint();
        e.consume();
    }

    /**
     * Handles the dragging of the mouse, dragging a tile with it.
     */
    public void mouseDragged(MouseEvent e) {
        // drag pieces
        if (!who.dumping) {
            for (int i = 0; i < Rack.NUM_TILES; i++)
                if (who.tiles[i].dragging) {
                    who.tiles[i].x = e.getX() - Tile.SIZE / 2;
                    who.tiles[i].y = e.getY() - Tile.SIZE / 2;
                }
        }
        this.repaint();
        e.consume();
    }

    /**
     * Handles a single mouse click, taking a tile off the board.
     */
    public void mouseClicked(MouseEvent e) {
        // which one of you tiles has been pressed
        for (int i = 0; i < Rack.NUM_TILES; i++)
            if (e.getX() >= who.tiles[i].x
                    && e.getX() <= (who.tiles[i].x + Tile.SIZE)
                    && e.getY() >= who.tiles[i].y
                    && e.getY() <= (who.tiles[i].y + Tile.SIZE)) {
                who.resetTile(i);
                if (who.dumping) {
                    if (who.tiles[i].selected)
                        who.tiles[i].selected = false;
                    else
                        who.tiles[i].selected = true;
                }
                // makes sure only one tiles is chosen
                break;
            }
        statusCheckMove();
        resetStatus();
        e.consume();
    }

    public void mouseExited(MouseEvent e) {}
    public void mouseEntered(MouseEvent e) {}
    public void mouseMoved(MouseEvent e) {}

    /**
     * Finds out the score for the move currently on the board.
     * 
     * @param rack The rack of the player to be checked.
     * @param testDict Should be true if a dictionary lookup is requested.
     * @return Returns an integer value of the score. If the move is invalid,
     *         returns 0, if the words are invalid, and a lookup was requested,
     *         returns -1.
     */
    public int getScore(Rack rack, boolean testDict) {
        int[] x = { -1, 0, 0 };
        int[] y = { -1, 0, 0 };
        final int MIN = 1, MAX = 2;
        char dir = checkLineUp(rack, x, y);
        // check if any tile first turn is at 7,7
        if (board.isOpen(7, 7) && who.findTileChar(7, 7) == ' ')
            return 0;
        int score = 0;
        rack.chained = false;
        if (dir == 'b') {
            int i, s = 0;
            for (i = 0; i < Rack.NUM_TILES && !rack.tiles[i].onBoard; i++);
            if (i >= Rack.NUM_TILES)
                return 0;
            if (adjacent(rack.tiles[i], 'h'))
                s += computeScore('h', rack, x, y, testDict);
            if (s < 0)
                return -1;
            if (adjacent(rack.tiles[i], 'v'))
                s += computeScore('v', rack, x, y, testDict);
            if (s < 0)
                return -1;
            score += s;
        } else if (dir == 'n') {
            return 0;
        } else
            score += computeScore(dir, rack, x, y, testDict);
        if (score == 0)
            return 0;
        else if (score < 0)
            return -1;

        if (dir == 'h')
            dir = 'v';
        else if (dir == 'v')
            dir = 'h';

        if (dir != 'b') {
            for (int i = 0; i < Rack.NUM_TILES; i++) {
                if (rack.tiles[i].onBoard) {
                    if (adjacent(rack.tiles[i], dir)) {
                        int[] x2 = { Board.getXIndex(rack.tiles[i].x),
                                Board.getXIndex(rack.tiles[i].x),
                                Board.getXIndex(rack.tiles[i].x) };
                        int[] y2 = { Board.getYIndex(rack.tiles[i].y),
                                Board.getYIndex(rack.tiles[i].y),
                                Board.getYIndex(rack.tiles[i].y) };
                        int s = computeScore(dir, rack, x2, y2, testDict);
                        if (s < 0)
                            return -1;
                        else
                            score += s;

                    }
                }
            }
        }
        if (rack.chained || board.isOpen(7, 7))
            return score;
        return 0;
    }

    /**
     * Determines if the tiles are placed in a straight line.
     * 
     * @param rack The rack of the player to be checked.
     * @param x An array that will recieve pertinent info.
     * @param y An array that will recieve pertinent info.
     * @return Returns 'h', or 'v' if the tiles line up. Returns 'b' if there is
     *         only one tile, and 'n' if they do not.
     */
    char checkLineUp(Rack rack, int[] x, int[] y) {
        boolean vert = true, horiz = true;
        for (int i = 0; i < Rack.NUM_TILES; i++) {
            if (rack.tiles[i].onBoard) {
                if (x[0] == -1 && y[0] == -1) {
                    x[2] = x[1] = x[0] = Board.getXIndex(rack.tiles[i].x);
                    y[2] = y[1] = y[0] = Board.getYIndex(rack.tiles[i].y);
                } else {
                    if (Board.getXIndex(rack.tiles[i].x) != x[0])
                        vert = false;
                    if (Board.getYIndex(rack.tiles[i].y) != y[0])
                        horiz = false;
                }
                if (Board.getXIndex(rack.tiles[i].x) > x[2])
                    x[2] = Board.getXIndex(rack.tiles[i].x);
                else if (Board.getXIndex(rack.tiles[i].x) < x[1])
                    x[1] = Board.getXIndex(rack.tiles[i].x);
                if (Board.getYIndex(rack.tiles[i].y) > y[2])
                    y[2] = Board.getYIndex(rack.tiles[i].y);
                else if (Board.getYIndex(rack.tiles[i].y) < y[1])
                    y[1] = Board.getYIndex(rack.tiles[i].y);
            }
        }
        if (vert == false && horiz == false)
            return 'n';
        else if (vert == false && horiz == true)
            return 'h';
        else if (vert == true && horiz == false)
            return 'v';
        else
            return 'b';
    }

    /**
     * Finds the score for a single word.
     * 
     * @param dir The direction in which the word lies, 'h' or 'v'.
     * @param rack The rack of the player to be checked.
     * @param xArray An array containing pertinent info about the word's position.
     * @param yArray An array containing pertinent info about the word's position.
     * @param testDict True if online lookup is required.
     * @return Returns the score of the word. O is returned if it is illegal, -1
     *         is returned if it is not a word and lookup was requested.
     */
    public int computeScore(char dir, Rack rack, int[] xArray, int[] yArray,
            boolean testDict) {
        while (xArray[1] > 0 && !board.isOpen(xArray[1] - 1, yArray[0])
                && dir == 'h')
            xArray[1]--;
        while (yArray[1] > 0 && !board.isOpen(xArray[0], yArray[1] - 1)
                && dir == 'v')
            yArray[1]--;
        int min = xArray[1], max = xArray[2], other = yArray[0];
        if (dir == 'v') {
            min = yArray[1];
            max = yArray[2];
            other = xArray[0];
        }
        StringBuffer word = new StringBuffer("");
        int x, y;
        int multiplier = 1, score = 0, mult = 1;
        char c = ' ';
        for (int i = min; i < Board.DIMENSION; i++) {
            x = i;
            y = other;
            if (dir == 'v') {
                x = other;
                y = i;
            }
            if (board.isOpen(x, y) && rack.findTileChar(x, y) == ' ') {
                if (i < max)
                    return 0;
                else if (testDict) {
                    if (!isWord(word))
                        return -1;
                }
                return score * multiplier;
            } else if (!board.isOpen(x, y)) {
                c = board.tiles[x][y];
                rack.chained = true;
            } else {
                c = rack.findTileChar(x, y);
                if (!((mult = board.tileKey[x][y]) <= 3))
                    mult = 1;
                if (board.tileKey[x][y] > 3) {
                    if (multiplier == 1)
                        multiplier--;
                    multiplier += board.tileKey[x][y] - 2;
                }
            }
            score += TileBag.valueOf(c) * mult;
            word.append(c);
        }
        if (testDict) {
            if (!isWord(word))
                return -1;
        }
        return score * multiplier;
    }

    /**
     * Finds if a tile has anything adjacent on the board on the specified axis.
     * 
     * @param t The tile to be checked.
     * @param d The direction to be checked, 'h' or 'v'.
     * @return Returns true if there is a tile adjacent, else false.
     */
    public boolean adjacent(Tile t, char d) {
        if (d == 'v') {
            if (Board.getYIndex(t.y) > 0) {
                if (!board.isOpen(Board.getXIndex(t.x),
                        Board.getYIndex(t.y) - 1))
                    return true;
            }
            if (Board.getYIndex(t.y) < Board.DIMENSION) {
                if (!board.isOpen(Board.getXIndex(t.x),
                        Board.getYIndex(t.y) + 1))
                    return true;
            }
        } else if (d == 'h') {
            if (Board.getXIndex(t.x) > 0) {
                if (!board.isOpen(Board.getXIndex(t.x) - 1, Board
                        .getYIndex(t.y)))
                    return true;
            }
            if (Board.getXIndex(t.x) < Board.DIMENSION) {
                if (!board.isOpen(Board.getXIndex(t.x) + 1, Board
                        .getYIndex(t.y)))
                    return true;
            }
        }
        return false;
    }

    /**
     * Looks a word up online at www.dictionary.com.
     * 
     * @param word The word requested.
     * @return Returns true if the word is found on dictionary.com, else false.
     */
    public boolean isWord(StringBuffer word) {
        URL url = null;
        boolean isLegal = false;
        String theWord = word + "";
        try {
            url = new URL("http://dictionary.reference.com/search?q="
                    + theWord.toLowerCase());
        } catch (MalformedURLException e) {
            ;
        }

        URLConnection connection = null;
        BufferedReader in = null;
        String line = "";

        try {
            connection = url.openConnection();
            connection.connect();
            in = new BufferedReader(new InputStreamReader(connection
                    .getInputStream()));
            while (!line.startsWith("<td><h2"))
                line = in.readLine();
            char c = line.charAt(36);
            if (c == 'N')
                isLegal = false;
            else
                isLegal = true;
            in.close();
        } catch (IOException e) {
            System.out.println("IO Exception: " + e.getMessage());
            noTesting = true;
            challenge.setEnabled(false);
        }
        return isLegal;
    }

    /**
     * Defines what to do at the end of the game and determines the winner.
     */
    public void gameEnd() {
        reset.setEnabled(false);
        dump.setEnabled(false);
        pass.setEnabled(false);
        submit.setEnabled(false);
        challenge.setEnabled(false);
        submit.setFocusable(false);

        Rack winner;
        if (playerOne.score > playerTwo.score)
            winner = playerOne;
        else
            winner = playerTwo;

        if (playerOne.score != playerTwo.score)
            status.setText("Congratulations! " + winner + " won with "
                    + winner.score + " points.");
        else
            status.setText("It was a tie.");
    }

    /**
     * Sets the status line back to default.
     */
    public void resetStatus() {
        boolean boardEmpty = true;
        for (int i = 0; i < Rack.NUM_TILES && boardEmpty; i++)
            if (who.tiles[i].onBoard)
                boardEmpty = false;

        if (boardEmpty) {
            if (board.isOpen(7, 7))
                status.setText("Please play a word with one letter " +
                   		"on the middle of the board.");
            else
                status.setText("Please play a word.");
            reset.setEnabled(false);
        } else
            reset.setEnabled(true);

        if (who.dumping) {
            pass.setEnabled(false);
            boolean anySelected = false;
            for (int i = 0; i < Rack.NUM_TILES && !anySelected; i++)
                if (who.tiles[i].selected)
                    anySelected = true;
            if (anySelected)
                status.setText("Press Dump to dump the selected tiles.");
            else
                status.setText("Select which tiles you want to dump. " +
                		"Press Dump to exit.");
        } else
            pass.setEnabled(true);
    }

    /**
     * Scores a move temporarily to print in the status line.
     */
    public void statusCheckMove() {
        int score = getScore(who, false);
        if (score > 0) {
            score += 50;
            for (int i = 0; i < Rack.NUM_TILES; i++)
                if (!who.tiles[i].onBoard) {
                    score -= 50;
                    break;
                }
            status.setText("Move will score " + score
                    + ". Press Sumbit to end your turn.");
            submit.setEnabled(true);
            if (!noTesting)
                challenge.setEnabled(true);
        } else {
            status.setText("Illegal move.");
            submit.setEnabled(false);
            challenge.setEnabled(false);
        }
    }

    /**
     * Draws the tiles to the screen.
     * 
     * @param g The graphics to be used.
     */
    public void drawTiles(Graphics g) {
        Color tan = new Color(255, 204, 153);
        Color ltan = new Color(255, 229, 204);
        Color lgreen = new Color(153, 204, 153);
        g.setColor(tan);
        Font font = new Font("Arial", Font.BOLD, 16);
        g.setFont(font);
        int dragIndex = -1;

        // draw tiles on the board
        for (int x = 0; x < board.tiles.length; x++) {
            for (int y = 0; y < board.tiles[0].length; y++)
                if (board.tiles[x][y] != ' ')
                    drawTileIndex(g, board.tiles[x][y], x, y);
        }

        // draw tiles in play
        for (int i = 0; i < Rack.NUM_TILES; i++) {
            if (who.tiles[i].selected)
                g.setColor(lgreen);
            else
                g.setColor(ltan);
            if (who.tiles[i].dragging)
                dragIndex = i;
            if (who.tiles[i].c != ' ')
                drawTile(g, who.tiles[i].c, who.tiles[i].x, who.tiles[i].y);
        }

        // draw dragging tile infront
        if (dragIndex != -1)
            if (who.tiles[dragIndex].c != ' ')
                drawTile(g, who.tiles[dragIndex].c, who.tiles[dragIndex].x,
                        who.tiles[dragIndex].y);
    }

    /**
     * Draw a tile based on its place on the board.
     * 
     * @param g The graphics to be used.
     * @param letter The letter on the tile.
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    public void drawTileIndex(Graphics g, char letter, int x, int y) {
        x = x * Tile.SIZE + Board.XOFFSET;
        y = y * Tile.SIZE + Board.YOFFSET;
        drawTile(g, letter, x, y);
    }

    /**
     * Draw a tile based on its screen position.
     * 
     * @param g The graphics to be used.
     * @param letter The letter on the tile.
     * @param x The x coordinate.
     * @param y The y coordinate.
     */
    public void drawTile(Graphics g, char letter, int x, int y) {
        Color previous = g.getColor();
        Font prevFont = g.getFont();

        // draw tilebg
        g.fillRect(x + 1, y + 1, Tile.SIZE, Tile.SIZE);
        g.setColor(Color.black);
        g.drawRect(x, y, Tile.SIZE, Tile.SIZE);

        // draw letter
        if (letter == 'I')
            g.drawString(letter + "", x + 13, y + 20);
        else
            g.drawString(letter + "", x + 9, y + 20);

        // draw value
        g.setFont(new Font("Arial", Font.BOLD, 10));
        if (letter == 'Z' || letter == 'Q')
            g.drawString(TileBag.valueOf(letter) + "", x + 16, y + 28);
        else
            g.drawString(TileBag.valueOf(letter) + "", x + 21, y + 28);

        // restore color
        g.setColor(previous);
        // and font
        g.setFont(prevFont);
    }

}