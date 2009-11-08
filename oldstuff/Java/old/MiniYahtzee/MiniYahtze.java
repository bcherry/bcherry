/**
   This is a skeleton in which to test the homework problem to create a method allTheSame().
   The method tests to see if a specified number of simulated dice rolls are all the same.
   Authors: Charlie McDowell
   			Ben Cherry (bcherry@ucsc.edu)
 */
class MiniYahtze {
    // execution starts here
    public static void main(String[] args) {
            if(allTheSame(5))
                System.out.println("Yahtzee!");
            else System.out.println("Fail!");
    }
    /*
      Math.random() returns a double greater than or equal to 0 but less than 1.
      Math.random()*6 will be greater than or equal to 0 but less than 6.
      Truncated to an int that will be between 0 and 5 inclusive.
      Adding 1 makes it 1-6.
     */
    static int rollDie() {
		return (int)(Math.random()*6)+1;
    }
    /*
     * This function takes as a parameter a number of dice to roll
     * It then simulates rolling them and returns true if they
     * are all the same, otherwise it returns false.  You cannot
     * give it a number of dice less than 2.
     */
	static boolean allTheSame(int numRolls){
		if(numRolls < 2) {
		    System.out.println("At least 2 dice, please.");
		    return false;
		}
		int roll = rollDie();
		for(int rolled = 1; rolled < numRolls; rolled++)
			if (rollDie() != roll) return false;
		return true;
	}
}
