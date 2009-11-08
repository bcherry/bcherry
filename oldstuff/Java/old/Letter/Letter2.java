/**
 * Assignment #1
 * This program uses standard input to generate letters home.
 * 
 * Authors: Ben Cherry (bcherry@ucsc.edu)
 * 			Torin Kampa (tkampa@ucsc.edu)
 * 			Bodhi Crandall-Rus (jcrandal@ucsc.edu)
 */

import tio.*;

class Letter {
    /*
     * The main function of this program.  It is the only method used
     * and will accept input (via file redirection), and use it to
     * reference arrays to generate a letter, which is then printed
     * to standard output.
     */
    public static void main(String[] args) {
        /*-------------------------------------*
         *     Variable Initializations        *
         *-------------------------------------*/
        String theLetter = "Dear Mom and Dad:\n\n";
        int sentNum, wordNum;
        String word = "error";/*In case it doesnt work*/
        /* --- The Arrays --- */
        String[] words[] = {{"great","ok","","",""},
                {"great","foggy","hot","cold","variable"},
                {"after the quarter ends","in a few weeks",
                    "next weekend","",""},
                {"","","","",""},
                {"cookies","stuff","money","",""}};
        String[] sentences[] = {{"My classes are going ", ".\n"},
                {"The weather here has been ",".\n"},
                {"I plan to come home for a visit ",".\n"},
                {"Do you think you could send me $","?\nI have to buy " +
                    "another book for one of my classes.\n"},
                {"Thanks for the "," you sent.\n"}};
        /*-------------------------------------*
         *     The Main Loop of the Program    *
         * ------------------------------------*/
        while(Console.in.hasMoreElements()){
            sentNum = Console.in.readInt()-1;
            wordNum = Console.in.readInt()-1;
            if(sentNum != 3)word = words[sentNum][wordNum];
            else word = "" + (wordNum+1);
            theLetter = theLetter + sentences[sentNum][0] + 
            	word + sentences[sentNum][1];
        }
        /*--Print the Letter--*/
        System.out.println(theLetter);
    }
}
