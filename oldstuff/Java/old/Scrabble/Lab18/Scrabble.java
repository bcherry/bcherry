/**
 * Lab 18: Scrabble - CMPS012a - Charlie McDowell
 * The beginnings of a Scrabble program. It creates a board,
 * two racks, and a bag of tiles. It accepts the first move
 * for player one and makes sure its a legal move
 *
 * @author Ben Cherry (bcherry@ucsc.edu)
 * @author Torin Kamapa (tkampa@ucsc.edu)
 * @author Jonathan Bodhi Crandall-Rus (jcrandal@ucsc.edu)
 */

import tio.*;

class Scrabble {
    public static void main (String[] args){
        Board board = new Board();
        Rack playerOne = new Rack();
        Rack playerTwo = new Rack();
        Tiles tileBag;
        if(args.length > 0)
            tileBag = new Tiles(Integer.parseInt(args[0]));
        else
            tileBag = new Tiles();
            
        playerOne.fill(tileBag);
        playerTwo.fill(tileBag);
        System.out.println("Player 1's rack is: " + playerOne);
        System.out.println("Player 2's rack is: " + playerTwo);
        
        String word;
        int x, y;
        boolean horizontal;
        do {
            System.out.println("Player 1 please give your move:");
            word = Console.in.readWord();
            x    = Console.in.readInt();
            y    = Console.in.readInt();
            horizontal = false;
            if (Console.in.readChar() == 'h')
                horizontal = true;
            Console.in.readLine();
        } while (!(playerOne.contains(new StringBuffer(word)) &&
                        x == 7 && y == 7));
        board.play(word, x, y, horizontal);
        System.out.println("Move accepted.");
    }
}
