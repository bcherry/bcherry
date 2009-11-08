/**
 * OrderTest
 * 10/19/04
 *
 * This program is an example of a method to check if 3 numbers are in order.
 * It is simply a test of the method, so it only prompts the use to enter
 * numbers to use.
 *
 * Author: Ben Cherry (bcherry@ucsc.edu)
 */

import tio.*;

class OrderTest{
    public static void main(String[] args){
        System.out.println("Please supply 3 integers:");
        System.out.println("Result is " +
                orderTest(Console.in.readInt(),Console.in.readInt(),
                        Console.in.readInt()));
    }
    /*
     * This method takes three integer parameters and returns
     * true if they are in non-decreasing order.  Otherwise
     * it will return false.
     */
    static boolean orderTest(int num1, int num2, int num3){
        if(num1 <= num2 && num2 <= num3) return true;
        return false;
    }
}