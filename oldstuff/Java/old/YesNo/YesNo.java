/**
 * Yes/No Questions
 * 10/23/04
 * 
 * A simple test case for a method to handle yes or no questions.
 * 
 * Author: Ben Cherry (bcherry@ucsc.edu)
 */
import tio.*;

class YesNo {
    static final int QUESTIONS = 6;
    /*
     * The main method is simply a thorough test and demonstration
     * of the functionality of the method for handling
     * yes/no questions with a prompt.
     */
    public static void main(String[] args){
        String[] prompts = {"Do you want to take a survey?",
                            "Do you like yes or no questions?",
                            "Will you continue answering these?",
                            "Are you some kind of robot?",
                            "Do you have special powers?",
                            "Do you use them for good or for awesome?",
                            "Will you use them on me?"};
        int i;
        for (i = 0; i <= QUESTIONS && yesNoQuestion(prompts[i]); i++);        
        if (i < 3) System.out.println("You are lame.");
        else System.out.println("You are mildly cool.");
        if (i >= 4) System.out.println("Actually, you're a cool robot.");
        if (i >= 5) System.out.println("...with cool powers.");
        if (i >= 6) System.out.println("Please use your powers on me!");
        if (i >= 7) System.out.println("Ooh, that feels good.");
    }
    /*
     * yesNoQuestion() is a method that takes the parameter
     * prompt and prints it as a yes/no question.  It takes standard
     * input and will return true if it was yes, or false if no.  
     * If neither then it prints the question again.
     */
    static boolean yesNoQuestion(String prompt){
        while(true){
            System.out.println(prompt);
            String input = Console.in.readWord();
            if (input.equals("yes")) return true;
            else if (input.equals("no")) return false;
        }
    }
}