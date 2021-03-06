#include <stdio.h>
#include "errors.h"

/**
 * Sends to stderr the appropriate error message based on the dealer error 
 * type.
 * Param:
 * type - the type of dealer error
 * Returns the error given.
 */
DealerErrors dealer_error_msg(DealerErrors type)
{
    const char* msg = "";
    switch (type) {
        case DEALER_OK:
            return DEALER_OK;
        case DEALER_ARGS:
            msg = "Usage: 2310dealer deck path p1 {p2}";
            break;
        case BAD_DECK:
            msg = "Error reading deck";
            break;
        case BAD_PATH:
            msg = "Error reading path";
            break;
        case BAD_PLAYER:
            msg = "Error starting process";
            break;
        case DEALER_COMS:
            msg = "Communications error";
            break;
    }
    fprintf(stderr, "%s\n", msg);
    return type;
}

/**
 * Sends to stderr the appropriate error message based on the player error 
 * type.
 * Param:
 * type - the type of player error
 * Returns the error given.
 */
PlayerErrors player_error_msg(PlayerErrors type)
{
    const char* msg = "";
    switch (type) {
        case PLAYER_OK:
            return PLAYER_OK;
        case PLAYER_ARGS:
            msg = "Usage: player pcount ID";
            break;
        case PLAYER_COUNT:
            msg = "Invalid player count";
            break;
        case PLAYER_BAD_ID:
            msg = "Invalid ID";
            break;
        case PLAYER_BAD_PATH:
            msg = "Invalid path";
            break;
        case EARLY_GAME_END:
            msg = "Early game over";
            break;
        case PLAYER_COMS:
            msg = "Communications error";
            break;
    }
    fprintf(stderr, "%s\n", msg);
    return type;
}
