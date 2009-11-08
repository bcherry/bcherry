import tio.*;

class LetterHome {
    public static void main(String[] args) {
        int question, choice = 0;
        String output = "Dear Mom and Dad:\n\n";
        String[][] line = {{"My classes are going ",              "great","ok","","","",                                              ".\n"},
                          {"The weather here has been ",          "great","foggy","hot","cold","variable",                            ".\n"},
                          {"I plan to come home for a visit ",    "after the quarter ends", "in a few weeks","next weekend","","",    ".\n"},
                          {"",                                    "","","","","",                                                     ""},
                          {"Thanks for the ",                     "cookies","stuff","money","","",                                    " you sent me.\n"}};

        while(Console.in.hasMoreElements()){
            question =  Console.in.readInt() - 1;   // arrays start at zero
            choice =    Console.in.readInt();       // arrays start at zero but options start at [1]

            if (question == 3) output += "Do you think you could send me $" + choice + "?\n I have to buy another book for one of my classes.\n";
            else output += line[question][0] + line[question][choice] + line[question][6]; // beginning of sentence + option + end of sentence
        }
        System.out.println(output);
    }
}