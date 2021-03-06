#include <stdlib.h>
#include "deck.h"
#include <string.h>
#include "utility.h"
#include "errors.h"

// Defines the indexes for the types of sets for scoring purposes
#define FULL_SET_INDEX 0
#define FOUR_SET_INDEX 1
#define THREE_SET_INDEX 2
#define TWO_SET_INDEX 3
#define ONE_SET_INDEX 4

/**
 * Gets the card corresponding to the char given.
 * Param:
 * card - the char of the card to be matched with a Cards.
 * Returns the corresponding Cards to the char given.
 */
Cards get_card(char card) {
    switch (card) {
        case 'A':
            return CARD_A;
        case 'B':
            return CARD_B;
        case 'C':
            return CARD_C;
        case 'D':
            return CARD_D;
        case 'E':
            return CARD_E;
        default:
            return CARD_INVALID;
    }
}

/**
 * Gets a Deck based on the information in the deckString.
 * Param:
 * deckString - contains info about the deck to be created.
 * Returns the Deck based on the information in the deckString.
 */
Deck* read_deck(char* deckString) {
    // Checks that the deckString is the right length
    char* cardsString;
    int cardCount = strtol(deckString, &cardsString, 10);
    if (cardsString[0] == '\0' || (int) strlen(cardsString) != cardCount) {
        exit(dealer_error_msg(BAD_DECK));
    }

    // Extracts each card from the deckString
    Cards* cards = (Cards*) malloc(sizeof(Cards) * cardCount);
    for (int i = 0; i < cardCount; i++) {
        if (get_card(cardsString[i]) == CARD_INVALID) {
            free(cards);
            exit(dealer_error_msg(BAD_DECK));
        } else {
            cards[i] = get_card(cardsString[i]);
        }
    }

    Deck* deck = create_deck(cardCount, cards);
    free(cards);
    return deck;
}

/**
 * Initialises a Deck based on the parameters given.
 * Param:
 * cardCount - the number of cards in the deck.
 * cards - the Cards in the deck
 * Returns the deck that was initialised.
 */
Deck* create_deck(int cardCount, Cards* cards) {
    Deck* deck = (Deck*) malloc(sizeof(Deck));
    deck->cardCount = cardCount;
    deck->cards = (Cards*) malloc(sizeof(Cards) * cardCount);
    for (int i = 0; i < cardCount; i++) {
        deck->cards[i] = cards[i];
    }
    return deck;
}

/**
 * Frees the given deck.
 * Param:
 * deck - the deck to be freed.
 */
void free_deck(Deck* deck) {
    free(deck->cards);
    free(deck);
}

/**
 * Adds card to the deck.
 * Param:
 * deck - deck that the card is to be added to.
 * card - the card to be added
 * Returns true if the card was added successfully, else false.
 */
bool add_card(Deck* deck, Cards card) {
    deck->cardCount++;
    Cards* cards = realloc(deck->cards, sizeof(Cards) * deck->cardCount);
    if (cards == NULL) {
        return false;
    }
    deck->cards[deck->cardCount - 1] = card;
    return true;
}

/**
 * Calculates the score of the deck.
 * Param:
 * deck - the deck which is to be scored.
 * Returns the score of the deck.
 */
int calculate_deck_score(Deck* deck) {
    int* cardCounters = get_card_counters(deck);
    int setCounters[5] = {0, 0, 0, 0, 0};

    // Gets how many of each set there are.
    for (int i = 0; i < deck->cardCount; i++) {
        int count = count_greater_than(cardCounters, UNIQUE_CARD_COUNT, i);
        switch (count) {
            case 5:
                setCounters[FULL_SET_INDEX]++;
                break;
            case 4:
                setCounters[FOUR_SET_INDEX]++;
                break;
            case 3:
                setCounters[THREE_SET_INDEX]++;
                break;
            case 2:
                setCounters[TWO_SET_INDEX]++;
                break;
            case 1:
                setCounters[ONE_SET_INDEX]++;
                break;
        }
    }

    // Stores how much each set is worth
    int setScores[5] = {10, 7, 5, 3, 1};

    // Adds up the score based on how much each set is worth.
    int totalScore = 0;
    for (int i = 0; i < UNIQUE_CARD_COUNT; i++) {
        totalScore += setCounters[i] * setScores[i];
    }
    free(cardCounters);
    return totalScore;
}

/**
 * Gets number of cards of each type.
 * Param:
 * deck - the deck of cards to check
 * Returns the number of cards of each type.
 */
int* get_card_counters(Deck* deck) {
    int* cardCounters = (int*) malloc(sizeof(int) * UNIQUE_CARD_COUNT);
    for (int i = 0; i < UNIQUE_CARD_COUNT; i++) {
        cardCounters[i] = 0;
    }
    for (int i = 0; i < deck->cardCount; i++) {
        switch (deck->cards[i]) {
            case CARD_A:
                cardCounters[0]++;
                break;
            case CARD_B:
                cardCounters[1]++;
                break;
            case CARD_C:
                cardCounters[2]++;
                break;
            case CARD_D:
                cardCounters[3]++;
                break;
            case CARD_E:
                cardCounters[4]++;
                break;
            case CARD_INVALID:
                free(cardCounters);
                return NULL;
        }
    }
    return cardCounters;
}