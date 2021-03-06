#ifndef DEALER_H
#define DEALER_H

#include <stdio.h>
#include "game.h"
#include "deck.h"

#define DEALER_ID -1
#define STREAM_COUNT 3

typedef struct {
    Game* game;
    Deck* deck;
    int nextCard;
    FILE*** comStreams;
} Dealer;

/**
 * Initialises the dealer based on the given parameters.
 * Param:
 * playerCount - the number of players.
 * deckFPath - contains the path to the deck file.
 * pathFPath - contains the path to the path file.
 * Returns the dealer that was initialised.
 */
Dealer* init_dealer(int playerCount, char* deckFPath, char* pathFPath);

/**
 * Frees the dealer given.
 * Param:
 * dealer - the dealer to be freed.
 */
void free_dealer(Dealer* dealer);

/**
 * Increments the dealer's next card counter. Resets the counter to 0 if the
 *         end of the deck is reached.
 * Param:
 * dealer - the dealer of the game.
 */
void next_card(Dealer* dealer);

#endif