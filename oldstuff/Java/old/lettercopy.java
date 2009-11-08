/*
 * Programming Assignment #1 - Generating Letters Home
 * CMPS 12A, 10/07/04
 * 
 * Author: Ben Cherry (bcherry@ucsc.edu)
 */

import tio.*;//Import the tio library

class Letter{
    /*
     * This is the main method of the class Letter.
     * It runs the program, which generates a simple
     * letter based on user input.
     */
    public static void main(String[] args){
        String theLetter = "Dear Mom and Dad, \n";
        boolean stillGoing = true;
        System.out.println("Sentence # and variation:");
        while (stillGoing){
            //System.out.println("1");//debug
            int sentenceNum = Console.in.readInt();
            int choiceNum = Console.in.readInt();
            String wordChoice = "error";
            
            if (sentenceNum == 0) stillGoing = false;
            else if (sentenceNum == 1) {
               if (choiceNum == 1) wordChoice = "great";
               else if (choiceNum == 2) wordChoice = "ok";
               else System.out.println("Error, choices for that type" +
                           " are 1 or 2.");
               if (wordChoice != "error") theLetter = theLetter + "My classes are going " +
               							wordChoice + ".\n";
            } else if (sentenceNum == 2) {
                if (choiceNum == 1) wordChoice = "great";
                else if (choiceNum == 2) wordChoice = "foggy";
                else if (choiceNum == 3) wordChoice = "hot";
                else if (choiceNum == 4) wordChoice = "cold";
                else if (choiceNum == 5) wordChoice = "variable";
                else System.out.println("Error, choices for that type" +
                        " are 1-5.");
                if (wordChoice != "error") theLetter = theLetter + "The weather here has been " +
                						 wordChoice + ".\n";
            } else if (sentenceNum == 3) {
                if (choiceNum == 1) wordChoice = "after the quarter ends";
                else if (choiceNum ==2) wordChoice = "in a few weeks";
                else if (choiceNum ==3) wordChoice = "next weekend";
                else System.out.println("Error, choices for that type" +
                        " are 1-3.");
                if (wordChoice != "error") theLetter = theLetter + "I plan to come home for a visit " +
                						 wordChoice + ".\n";
            } else if (sentenceNum == 4) {
                theLetter = theLetter + "Do you think you could send me $" +
                						 choiceNum +"?\nI have to buy " +
                						 "another book for one of my classes.\n";
            } else if (sentenceNum == 5) {
                if (choiceNum == 1) wordChoice = "cookies";
                else if (choiceNum == 2) wordChoice = "stuff";
                else if (choiceNum == 3) wordChoice = "money";
                else System.out.println("Error, choices for that type" +
                        " are 1-3.");
                if (wordChoice != "error") theLetter = theLetter + "Thanks for the " +
                						 wordChoice + " you sent.\n";
            } else System.out.println("Error, that is not a choice.");
        }
        theLetter = theLetter + "Love, Ben";
        System.out.println("Success! Your letter:\n");
        System.out.println(theLetter);
    }
}