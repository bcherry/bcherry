/**
 * Programming Assignment #1
 * Date: 10/05/04
 * Purpose: This program will accept standard input and analyze that
 *          input to print out a letter home with varying sentence
 *          order and word choice.
 *
 * Authors: Ben Cherry (bcherry@ucsc.edu)
 *          Torin Kampa (tkampa@ucsc.edu)
 *          Bodhi Crandall-Rus (jcrandal@ucsc.edu)
 */

import tio.Console; // simple text i/o class

class LetterHome {
    /*
     * The main function of this program.  It is the only method used
     * and will accept input (via file redirection), and use it to
     * reference an array to generate a letter, which is then printed
     * to standard output.
     */
     public static void main(String[] args){
         String theLetter = "Dear Mom and Dad:\n\n";
         int sentenceNumber = 0, wordNumber = 0;
         String[][] phraseTable =
            {{"My classes are going ",
              "great","ok","","","",
              ".\n"},
             {"The weather here has been ",
              "great","foggy","hot","cold","variable",
              ".\n"},
             {"I plan to come home for a visit ",
              "after the quarter ends", "in a few weeks","next weekend","","",
              ".\n"},
             {"Do you think you could send me $",
              "","","","","",
              "?\nI have to buy another book for one of my classes.\n"},
             {"Thanks for the ",
              "cookies","stuff","money","","",
              " you sent.\n"}};

         while(Console.in.hasMoreElements()) {
             sentenceNumber = Console.in.readInt() - 1; // read from zero
             wordNumber = Console.in.readInt();         // read from one

             // The first part of the addition
             theLetter += phraseTable[sentenceNumber][0];

             // The variable word
             if(sentenceNumber == 3)
                 theLetter += wordNumber;
             else
                 theLetter += phraseTable[sentenceNumber][wordNumber];

             // The last part of the addition
             theLetter += phraseTable[sentenceNumber][6];
         }

         System.out.println(theLetter);
     }
}