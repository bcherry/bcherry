import tio.*;
import java.util.Random;

class Profiling {
    static final int MOHAMMED = 12;
    static final int AHMAD = 3;
    static final int TARIQ = 6;
    static final int ISLAM = 19;
    
    public static void main (String[] args) {
        System.out.println("Welcome to the racial profiling simulator!\n" +
                "We offer only the finest in middle-eastern racial profiling!\n" +
                "How many times would you like to detain Muslims today?");
        int numTrials = Console.in.readInt();
        Random rand = new Random();
        int numAllah = 0;
        for (int i = 0; i < numTrials; i++) {
            int detaineeOne = rand.nextInt(20) + 1;
            int detaineeTwo = rand.nextInt(20) + 1;
            boolean oneAllah = false;
            boolean twoAllah = false;
            while (detaineeOne == detaineeTwo)
                detaineeTwo = rand.nextInt(20) + 1;
            if (detaineeOne == MOHAMMED ||
                detaineeOne == AHMAD ||
                detaineeOne == TARIQ ||
                detaineeOne == ISLAM)
                oneAllah = true;
            if (detaineeTwo == MOHAMMED ||
                detaineeTwo == AHMAD ||
                detaineeTwo == TARIQ ||
                detaineeTwo == ISLAM)
                twoAllah = true;
            if (oneAllah && twoAllah)
                numAllah++;
        }
        System.out.println("Out of " + numTrials + " flights, we profiled " +
                numAllah + " times.\nThat's " + (float)((float)numAllah/(float)numTrials)*100 + "%!");
    }
}