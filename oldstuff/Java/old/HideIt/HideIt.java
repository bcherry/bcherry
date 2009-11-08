/**
 * Programming Assignment #2
 * 10/19/04
 *
 * This program will encrypt or decrypt text based on a Vernan Cipher
 * sequence from a pseudo-random number generator.  It accepts two
 * command-line parameters.  The first is a flag, -e or -d, that
 * tells the program to encrypt or decrypt.  The second parameter
 * should be an integer to use as the seed for the random number
 * generator, and the same seed should be used to encrypt or decrypt.
 *
 * Authors:  Bodhi Crandall-Rus (jcrandal@ucsc.edu)
 *           Torin Kampa (tkampa@ucsc.edu)
 *           Ben Cherry (bcherry@ucsc.edu)
 */

import java.util.Random; // the random package
import tio.Console;      // simple i/o

class HideIt {
    static final int MAX_CHAR   = 126;
    static final int MIN_CHAR   =  32;
    static final int CHAR_RANGE =  95;
    /*
     * The main function.  It first will read the parameters and
     * seed the number generator.  Then it will enter a while
     * loop to encrypt or decrypt the message one character
     * at a time.
     */
    public static void main(String[] args) {
        // seed the random number generator with parameter 2
        Random rand = new Random(Integer.parseInt(args[1]));
        int eORd = 0; // to add or subtract encryption padding
        if (args[0].equals("-e"))
            eORd = 1; // add the encrpytion pad to the char to encrypt
        else if (args[0].equals("-d"))
            eORd = -1; // subtract the encryption pad to decrypt
        else {
            System.out.println("Unknown command line parameter.");
            return;
        }
        while (Console.in.hasMoreElements()) {
            // read in a character
            int currentChar = Console.in.readChar();
            // if that char is within the range, then modify it
            if (currentChar >= MIN_CHAR && currentChar <= MAX_CHAR){
                currentChar += rand.nextInt(CHAR_RANGE) * eORd;
                // wrap the encryption if neccesary
                if(currentChar < MIN_CHAR || currentChar > MAX_CHAR)
                    currentChar -= CHAR_RANGE * eORd;
            }
            System.out.print((char)currentChar);
        }
    }
}

