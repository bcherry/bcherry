// C Source File
// Created 21/Oct/03; 15:55:12

#define USE_TI89              // Compile for TI-89
#define USE_TI92PLUS          // Compile for TI-92 Plus
#define USE_V200              // Compile for V200

#define OPTIMIZE_ROM_CALLS    // Use ROM Call Optimization

#define SAVE_SCREEN           // Save/Restore LCD Contents

#include <tigcclib.h>         // Include All Header Files

#define DECK_SIZE	52

typedef struct {
	int face;
	int suit;
} Card;

void initDeck(Card *deck) {
	int loop;

	// assign initial values to the deck of cards
	for (loop = 0; loop < DECK_SIZE; loop++) {
		deck[loop].face = loop % 13;
		deck[loop].suit = loop / 13;
	}
}

void shuffle(Card *deck) {
	int loop, randNum;
	Card temp;

	// shuffle the deck
	for (loop = 0; loop < DECK_SIZE; loop++) {
		randNum = rand() % DECK_SIZE;

		// swap the card at the current position with
		// the one at the randomly selected position
		temp = deck[loop];
		deck[loop] = deck[randNum];
		deck[randNum] = temp;
	}
}

void printDeck(Card *deck, const char *faces[], const char *suits[]) {
	int loop;

	// clear the screen
	clrscr();

	// print the deck one card at a time
	for (loop = 0; loop < 52; loop++) {
		// print the face and suit of the card
		printf("%s of %s\n",faces[deck[loop].face],suits[deck[loop].suit]);

		// pause for each 8 cards displayed
		if ((loop % 8) == 0 && loop != 0) {
			printf("Press a key to continue...\n");
			ngetchx();
		}
	}
}

// Main Function
void _main(void) {
	Card *deck = NULL;
	const char *faces[] = {"Ace","Two","Three","Four","Five","Six","Seven","Eight",
				"Nine","Ten","Jack","Queen","King"};
	const char *suits[] = {"Spades","Diamonds","Clubs","Hearts"};

	// allocate memory for the deck of cards
	if ((deck = (Card *)calloc(DECK_SIZE,sizeof(Card))) == NULL) {
		DlgMessage("DMA Error","Unable to allocate card structure",BT_OK,BT_NONE);
		return;
	}

	// initialize the random number generator
	randomize();

	// initialize the deck of cards
	initDeck(deck);

	// shuffle the deck
	shuffle(deck);

	// print the deck of cards
	printDeck(deck,faces,suits);

	// free the memory used by the deck of cards
	free(deck);

	// wait for user input before exiting the program
	ngetchx();
}