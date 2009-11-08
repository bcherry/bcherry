/**
 * Lab 11 - Simple 21
 * 10/28/04
 * 
 * This program plays a simple game of twenty-one.  The player and dealer
 * each recieves two cards, and the player sees both of his and one of
 * the dealer's.  He then places a legal bet and the dealer reveals his
 * other card.  The player then wins, loses, or pushes and his money
 * is affected accordingly.
 * 
 * Author: Ben Cherry (bcherry@ucsc.edu)
 */
import tio.*;
import cards.*;

class TwentyOne {
    public static void main (String[] args) {
        int bet = -1, cash = 100;
        Deck deck = makeDeck(args);
        Hand playerHand, dealerHand;
        System.out.println("Welcome to simple twenty-one!");
        while(bet != 0 && cash > 0){
            deck.shuffle();
            playerHand = new Hand();
            dealerHand = new Hand();
            dealHands(playerHand, dealerHand, deck);
            showHand("You have: ", playerHand);
            System.out.println("The dealer is showing: " + dealerHand.showOne());
            bet = getBet(cash);
            showHand("Dealer had: ", dealerHand);
            cash += bet * resolveOutcome(playerHand.valueAce11(), dealerHand.valueAce11());
        }
        System.out.println("Thanks for playing.");
    }
    /*
     * This function creates and shuffles and returns a deck.  It has
     * an optional parameter for the seed to be used in randomizing.
     */
    static Deck makeDeck(String[] args) {
        Deck deck;
        if (args.length == 0)
            deck = new Deck();
        else
            deck = new Deck(Integer.parseInt(args[0]));
        return deck;
    }
    /*
     * This function takes both hands and the deck and deals out cards
     * one at a time to the dealer and the player.
     */
    static void dealHands(Hand playerHand, Hand dealerHand, Deck deck) {
        playerHand.add(deck.draw());
        dealerHand.add(deck.draw());
        playerHand.add(deck.draw());
        dealerHand.add(deck.draw());
    }
    /*
     * This function will print the parameter "phrase" followed by
     * the value of "hand".
     */
    static void showHand(String phrase, Hand hand) {
        System.out.println(phrase + hand);
    }
    /*
     * This function prompts and accepts a bet.  If the bet is illegal,
     * i.e. more than your cash (given as a parameter) then it asks
     * again.  It then returns the value of the bet given.
     */
    static int getBet(int cash) {
        System.out.println("You have $" + cash + ". How much do you want to bet? Bet 0 to quit.");
        int input = Console.in.readInt();
        if (input <= cash)
            return input;
        else {
            System.out.println("That is not a legal bet.");
            return getBet(cash);
        }
        
    }
    /*
     * This function takes the total value of each hand and compares them.
     * If the player won, it returns 1 and prints "You win.", if the
     * dealer won it returns -1 and prints "Better luck next time."
     * Finally, in a tie it prints "That's a push" and returns 0.
     * In the main loop the return value is multiplied by the bet to
     * correctly modify the player's cash after each hand.
     */
    static int resolveOutcome(int player, int dealer){
        if (player > dealer){
            System.out.println("You win.");
            return 1;
        }
        else if (player == dealer){
            System.out.println("That's a push.");
            return 0;
        }
        else {
            System.out.println("Better luck next time.");
            return -1;
        }
    }
}