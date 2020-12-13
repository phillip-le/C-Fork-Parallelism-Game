#ifndef ERRORS_H
#define ERRORS_H

/**
 * Dealer error codes.
 */
typedef enum {
    DEALER_OK = 0,
    DEALER_ARGS = 1,
    BAD_DECK = 2,
    BAD_PATH = 3,
    BAD_PLAYER = 4,
    DEALER_COMS = 5
} DealerErrors;

/**
 * Player error codes.
 */
typedef enum {
    PLAYER_OK = 0,
    PLAYER_ARGS = 1,
    PLAYER_COUNT = 2,
    PLAYER_BAD_ID = 3,
    PLAYER_BAD_PATH = 4,
    EARLY_GAME_END = 5,
    PLAYER_COMS = 6
} PlayerErrors;

/**
 * Sends to stderr the appropriate error message based on the dealer error 
 * type.
 * Param:
 * type - the type of dealer error
 * Returns the error given.
 */
DealerErrors dealer_error_msg(DealerErrors type);

/**
 * Sends to stderr the appropriate error message based on the player error 
 * type.
 * Param:
 * type - the type of player error
 * Returns the error given.
 */
PlayerErrors player_error_msg(PlayerErrors type);

#endif