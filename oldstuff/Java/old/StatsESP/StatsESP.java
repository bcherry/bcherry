/**
 * Stats ESP Simulator
 * 10/26/04
 * 
 * Author: Ben Cherry
 */

import java.util.Random;
import tio.*;

class StatsESP {
    public static void main (String[] args) {
        System.out.println("How many trials?");
        int howMany = Console.in.readInt();
        boolean transcript = true;
        boolean transcriptNone = false;
        boolean keepGoing = false;
        int[] letters = {10,10,10,10,10,10,10,10,10,10};
        int[] results = {0,0,0,0,0,0,0,0,0,0};
        if(howMany > 1){
            if (transcriptNone = yesNoQuestion("Transcript none?")){
                transcript = false;
            }
            keepGoing = yesNoQuestion("Automate?");
        }
        for (int k = 1; k <= howMany; k++){
            if (!transcriptNone)transcript = yesNoQuestion("Show transcript?");
            Random rand = new Random();
            for (int x = 0; x <= 9; x++) letters[x] = 10;
            int i, count = 0;
            for (i = 0; i <= 9; i++){
                letters[i] = giveLetter(rand, letters, transcript);
                if(transcript) System.out.println("Person " + i +": " + letters[i]);
                if(letters[i] == i) count++;
            }
            System.out.println("Number correct: " + count);
            results[count]++;
            if(!keepGoing) {
                if(!yesNoQuestion("Keep Going?"))
                        break;
            }
        }
        for (int k = 0; k <= 9; k++) {
            System.out.println("Total at " + k + ": " + results[k]);
        }
    }
    
    static int giveLetter(Random rand, int[] letters, boolean transcript){
        int letter = rand.nextInt(10);
        for (int i = 0; i <= 9; i++){
            if (letters[i] == letter){
                return giveLetter(rand, letters, transcript);
            }
        }
        return letter;
    }
    static boolean yesNoQuestion(String prompt){
        while(true){
            System.out.println(prompt);
            String input = Console.in.readWord();
            if (input.equals("yes")) return true;
            else if (input.equals("no")) return false;
        }
    }
}