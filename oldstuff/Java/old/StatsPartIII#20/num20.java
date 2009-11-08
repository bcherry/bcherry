import tio.*;
import java.util.Random;

class Num20 {
    public static void main (String[] args) {
        while (true) {
        Random rand = new Random();
        System.out.println("Pick 3 numbers from 1 - 20:");
        int[] guess = {Console.in.readInt(),
                Console.in.readInt(),
                Console.in.readInt()};
        int[] lotto = {rand.nextInt(20) + 1,
                rand.nextInt(20) + 1,
                rand.nextInt(20) + 1,
                rand.nextInt(20) + 1,
               rand.nextInt(20) + 1};
        boolean gotItRight = true;
        boolean gotOne = false;
        for (int i = 0; i < 2 && gotItRight; i++) {
            for (int j = 0; j < 4; j++) {
                if (guess[i] == lotto[j])
                    gotOne = true;
            }
            gotItRight = gotOne;
        }
        System.out.println("You got it right? " + gotItRight);
        }
    }
}