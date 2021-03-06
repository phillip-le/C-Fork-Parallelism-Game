#include <stdlib.h>
#include "dealer.h"
#include "errors.h"
#include "utility.h"

/**
 * Initialises the dealer based on the given parameters.
 * Param:
 * playerCount - the number of players.
 * deckFPath - contains the path to the deck file.
 * pathFPath - contains the path to the path file.
 * Returns the dealer that was initialised.
 */
Dealer* init_dealer(int playerCount, char* deckFPath, char* pathFPath) {
    Dealer* dealer = (Dealer*) malloc(sizeof(Dealer));

    // Get deck
    FILE* deckFile = fopen(deckFPath, "r");
    if (deckFile == NULL) {
        exit(dealer_error_msg(BAD_DECK));
    }
    char* deckString = read_line(deckFile);
    fclose(deckFile);
    dealer->deck = read_deck(deckString);
    if (dealer->deck == NULL) {
        exit(dealer_error_msg(BAD_DECK));
    }
    
    FILE* pathFile = fopen(pathFPath, "r");
    if (pathFile == NULL) {
        exit(dealer_error_msg(BAD_PATH));
    }
    char* pathString = read_line(pathFile);
    fclose(pathFile);

    dealer->game = init_game(DEALER_ID, playerCount, pathString);
    if (dealer->game == NULL) {
        exit(dealer_error_msg(BAD_PATH));
    }
    dealer->nextCard = 0;
    dealer->comStreams = (FILE***) malloc(sizeof(FILE**) * playerCount);
    for (int i = 0; i < playerCount; i++) {
        dealer->comStreams[i] = (FILE**) malloc(sizeof(FILE**) *
                STREAM_COUNT);
    }
    return dealer;
}

/**
 * Frees the dealer given.
 * Param:
 * dealer - the dealer to be freed.
 */
void free_dealer(Dealer* dealer) {
    for (int i = 0; i < dealer->game->playerCount; i++) {
        for (int j = 0; j < 2; j++) {
            fclose(dealer->comStreams[i][j]);
        }
        free(dealer->comStreams[i]);
    }
    free(dealer->comStreams);
    free_deck(dealer->deck);
    free_game(dealer->game);
    free(dealer);
}

/**
 * Increments the dealer's next card counter. Resets the counter to 0 if the
 *         end of the deck is reached.
 * Param:
 * dealer - the dealer of the game.
 */
void next_card(Dealer* dealer) {
    dealer->nextCard++;
    if (dealer->nextCard == dealer->deck->cardCount) {
        dealer->nextCard = 0;
    }
}