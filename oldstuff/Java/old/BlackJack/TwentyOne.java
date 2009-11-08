/**
 * Assignment #3: TwentyOne
 * Due Date: 11-4-04
 * 
 * This is a basic version of the game of Twenty-One. It uses the console
 * for input and output and was written in Java. The player starts with
 * 100 dollars and tries to make as much money as possible by betting against
 * the dealer. The object is to get as close to twenty-one without going over.
 * The dealer continues drawing more cards until his hand values 17 or higher.
 * The game ends when the player enters a bet of zero or runs out of money.
 * The program is run with "java TwentyOne [seed]"
 * 
 * @author Torin Kampa (tkampa@ucsc.edu)
 * @author Ben Cherry (bcherry@ucsc.edu)
 * @author Jonathan Bodhi Crandall-Rus (jcrandal@ucsc.edu)
 */

import tio.*; // simple i/o
import cards.*; // Deck and Hand classes

class TwentyOne {
    static final int BUST = 21;
    
    static final int DEALER_HOLD = 17;
    
    public static void main(String[] args) {
        Deck deck = makeDeck(args);
        Hand playerHand, dealerHand;
        int bet, cash = 100;
        welcome();
        while ((bet = getBet(cash)) != 0 && cash > 0) {
            deck.shuffle();
            playerHand = new Hand();
            dealerHand = new Hand();
            dealHands(playerHand, dealerHand, deck);
            showInitial(playerHand, dealerHand);
            playerLoop(playerHand, deck);
            showHand("Dealer has: ", dealerHand);
            if (valueOf(playerHand) <= BUST)
                dealerLoop(dealerHand, deck);
            cash += bet * resolveOutcome(valueOf(playerHand),
                    valueOf(dealerHand));
        }
    }
    
    /**
     *  Prints a welcome message to the user.
     */
    static void welcome() {
        System.out.println("Let's play twenty-one.");
    }
    
    /**
     * Recieves a legal bet from the player. Illegal bets are less than zero
     * or more than the player's current money cash.
     * 
     * @param cash Amount of money player has.
     * @return the bet entered by the player
     */
    static int getBet(int cash) {
        System.out.println("You have $" + cash
                + ". How much do you want to bet? Bet 0 to quit.");
        int input = Console.in.readInt();
        if (input <= cash)
            return input;
        else {
            System.out.println("That is not a legal bet.");
            return getBet(cash);
        }
        
    }
    
    /**
     * Deals out two cards into each hand.
     * 
     * @param playerHand the hand of the player
     * @param dealerHand the hand of the dealer
     */
    static void dealHands(Hand playerHand, Hand dealerHand, Deck deck) {
        playerHand.add(deck.draw());
        dealerHand.add(deck.draw());
        playerHand.add(deck.draw());
        dealerHand.add(deck.draw());
    }
    
    /**
     * Shows the player's hand and the one card the dealer is showing.
     * 
     * @param playerHand the hand of the player
     * @param dealerHand the hand of the dealer
     */
    static void showInitial(Hand playerHand, Hand dealerHand) {
        showHand("You have: ", playerHand);
        System.out.println("The dealer is showing: " + dealerHand.showOne());
    }
    
    /**
     * Prints out the given hand with the given message.
     * 
     * @param phrase the message
     * @param hand the hand to print
     */
    static void showHand(String phrase, Hand hand) {
        System.out.println(phrase + hand);
    }
    
    /**
     * Builds the deck with a seed if it is given
     * 
     * @param args contains an optional seed in index zero
     * @return the newly created deck
     */
    static Deck makeDeck(String[] args) {
        Deck deck;
        if (args.length == 0)
            deck = new Deck();
        else
            deck = new Deck(Integer.parseInt(args[0]));
        return deck;
    }
    
    /**
     * Prints the highest value of a hand, that is less than 21.
     * 
     * @param hand the hand to compute the value of
     * @return the best value of the hand
     */
    static int valueOf(Hand hand) {
        if (hand.valueAce11() <= BUST)
            return hand.valueAce11();
        else
            return hand.valueAce1();
    }
    
    /**
     * Asks the player if they want another card.
     * 
     * @param hand the player's hand
     * @return true if they want a card
     *         false if they don't
     */
    static boolean wantCard(Hand hand) {
        if (valueOf(hand) > BUST)
            return false;
        System.out.println("Do you want another card?[y/n]");
        char response = (char) Console.in.readCharNonwhite();
        Console.in.readLine();
        switch (response) {
        case 'y':
        case 'Y':
            return true;
        case 'n':
        case 'N':
            return false;
        default:
            return wantCard(hand);
        }
    }
    
    /**
     * Adds a card from deck to the hand.
     * 
     * @param hand the hand that needs a card
     * @param deck the source of the card
     */
    static void hit(Hand hand, Deck deck) {
        hand.add(deck.draw());
    }
    
    /**
     * As long as the player wants a card, hit, and show his hand.
     * 
     * @param hand the player's hand
     * @param deck the source of the card
     */
    static void playerLoop(Hand hand, Deck deck) {
        while (wantCard(hand)) {
            hit(hand, deck);
            showHand("Your hand is now: ", hand);
        }
    }
    
    /**
     * As long as the dealer is holding less than 17, hit, and output his hand.
     * 
     * @param dealerHand the dealer's hand
     * @param deck the source of the card
     */
    static void dealerLoop(Hand dealerHand, Deck deck) {
        while (valueOf(dealerHand) < DEALER_HOLD) {
            hit(dealerHand, deck);
            System.out.println("Dealer takes another card.");
            showHand("Dealer now has: ", dealerHand);
        }
    }
    
    /**
     * Determines whether the player wins and outputs an int to multiply
     * bet by.
     * 
     * @param player the value of the player's hand
     * @param dealer the value of the dealer's hand
     * @return 1 if player wins
     *         0 if push
     *         -1 if player losses
     */
    static int resolveOutcome(int player, int dealer) {
        if ((player <= BUST && player > dealer) || (dealer > BUST)) {
            System.out.println("You win.");
            return 1;
        } else if (player == dealer) {
            System.out.println("That's a push.");
            return 0;
        } else {
            System.out.println("Better luck next time.");
            return -1;
        }
    }
}


