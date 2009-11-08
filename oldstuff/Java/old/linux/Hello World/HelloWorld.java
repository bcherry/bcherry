/**
 * My first program in Eclipse on Linux!
 * Yay!
 * 
 * @author Ben Cherry (bcherry@gmail.com)
 */
import tio.*;

class HelloWorld {
    public static void main (String[] args) {
        System.out.println("Hello World!");
        if (Console.in.readWord().equals("hello"))
            System.out.println("Yay I love you!");
        else
            System.out.println("Oh, you bastard...");
    }
}