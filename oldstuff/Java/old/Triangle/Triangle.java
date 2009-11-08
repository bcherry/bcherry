/**
 * Triangle.java
 * 10/25/04
 * 
 * This program is a simple demonstration of the method "drawTriangle()"
 * It contains a very simple implementation based on user input.
 * 
 * Author: Ben Cherry (bcherry@ucsc.edu)
 */
import tio.*;

public class Triangle {
    public static void main (String[] args) {
        System.out.println("I will build you a pyramid like the Egyptians!");
        System.out.println("Tell me how big and what to build it out of:");
        drawTriangle(Console.in.readInt(),(char)Console.in.readCharNonwhite());
        System.out.println("Your pyramid is worthy of the gods.");
    }
    /*
     * This method draws a triangle of height and base "size",
     * and is constructed of "symbol".
     */
    static void drawTriangle(int size, char symbol) {
        for(int i = 1; i <= size; i++) {
            for(int j = 1; j <= i; j++)
                System.out.print(symbol);
            System.out.println();
        }
    }
}