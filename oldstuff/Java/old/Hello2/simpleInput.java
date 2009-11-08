import tio.*;

class simpleInput{
    public static void main(String[] args){
        int width, height, area;
        
        System.out.println("type two integers for" +
                " the width and height of a box");
        width = Console.in.readInt();
        height = Console.in.readInt();
        area = width * height;
        System.out.print("The area is ");
        System.out.println(area);
    }
}