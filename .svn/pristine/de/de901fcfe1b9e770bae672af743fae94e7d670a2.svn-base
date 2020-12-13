#ifndef DECK_H
#define DECK_H

#include <stdbool.h>

#define UNIQUE_CARD_COUNT 5

/**
 * The types of cards a deck can have.
 */
typedef enum {
    CARD_INVALID = 0,
    CARD_A = 1,
    CARD_B = 2,
    CARD_C = 3,
    CARD_D = 4,
    CARD_E = 5
} Cards;

/**
 * Representation of a deck of cards.
 */
typedef struct {
    Cards* cards;
    int cardCount;
} Deck;

/**
 * Gets a Deck based on the information in the deckString.
 * Param:
 * deckString - contains info about the deck to be created.
 * Returns the Deck based on the information in the deckString.
 */
Deck* read_deck(char* deckString);

/**
 * Initialises a Deck based on the parameters given.
 * Param:
 * cardCount - the number of cards in the deck.
 * cards - the Cards in the deck
 * Returns the deck that was initialised.
 */
Deck* create_deck(int cardCount, Cards* cards);

/**
 * Frees the given deck.
 * Param:
 * deck - the deck to be freed.
 */
void free_deck(Deck* deck);

/**
 * Adds card to the deck.
 * Param:
 * deck - deck that the card is to be added to.
 * card - the card to be added
 * Returns true if the card was added successfully, else false.
 */
bool add_card(Deck* deck, Cards card);

/**
 * Calculates the score of the deck.
 * Param:
 * deck - the deck which is to be scored.
 * Returns the score of the deck.
 */
int calculate_deck_score(Deck* deck);

/**
 * Gets number of cards of each type.
 * Param:
 * deck - the deck of cards to check
 * Returns the number of cards of each type.
 */
int* get_card_counters(Deck* deck);

#endif