import tio.*;

class HelloWorld {
    public static void main(String[] args){
		hello();
		String theWord = Console.in.readWord();
		System.out.println("You wrote: " + theWord);
	}
    public static void hello(){
	    System.out.println("Hello, world!");
	}
    
}
