// SimpleInput.java-reading numbers from the keyboard
import tio.*;    // use the package tio 

class Fiddle {
  public static void main (String[] args) {
    while(true){
    System.out.println("Hello, please give 2 integers.");
    int a = Console.in.readInt();
    int b = Console.in.readInt();
    int op = (int)(10*Math.random())/2;
    String ans = "error";
    int num = 0;
    
    if (op==1){
        ans = "a";
        num = a/b;
    } else if (op==2){
        ans = "b";
        num = a*b;
    } else if (op==3){
        ans = "c";
        num = b%a;
    } else if (op==4){
        ans = "d";
        num = b/a+b*a-b;
    } else {
        ans = "e";
        num = (int)(1000*Math.random());
    }
    System.out.println("I magicked ("+op+") those two numbers and got this:\n"+num+
            "\nWhat did i do to them?\n" +
            "a. Num1 / Num2\n" +
            "b. Num1 * Num2\n" +
            "c. Num2 % Num1\n" +
            "d. Num2 / Num1 + Num2 * Num1 - Num2\n" +
            "e. Magic!");
    if (Console.in.readWord().compareTo(ans)==0)System.out.println("" +
    		"Correct, you are a winner!");
    else System.out.println("No, you suck, you fail!");
    }   
      
      /*while(true){
      int r = (int)(1000 * Math.random());
    System.out.println(""+r);
  }*/
  }
}