/**
 * This program uses a simple Caesar cipher method to encrypt
 * text from standard input by adding 1 to the ascii code.
 * It will then return the encrypted text.
 * 
 * Author: Ben Cherry (bcherry@ucsc.edu)
 */ 
import tio.*;

class HideIt {
    public static void main(String[] args){
        while (Console.in.hasMoreElements()){
            int encryptedChar = (int)Console.in.readChar() + 1;
            System.out.print((char)encryptedChar);
        }
    }
}