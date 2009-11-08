// SimpleInput.java-reading numbers from the keyboard
import tio.*;    // use the package tio 

class Fiddle {
  public static void main (String[] args) {
    String theInput,theRest;
    char xyz;
    int someNumber;
    
    System.out.println("type \tsomething");
    theInput = Console.in.readWord();
 //   theRest = Console.in.readLine(); // skip the rest of the line
    System.out.println("You said: " + theInput);
//    System.out.println("And the rest of the line is : " 
 //                   +theRest);
    xyz = (char)Console.in.readCharNonwhite();
    System.out.println("The character read was: " + xyz);
    someNumber = xyz;
    System.out.println("The value of that char is: " + 
                        someNumber);
  }
}
