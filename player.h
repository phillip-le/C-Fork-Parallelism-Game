#ifndef PLAYER_H
#define PLAYER_H

#include "deck.h"
#include "path.h"

#define STARTING_MONEY 7

/**
 * Representation of the player.
 */
typedef struct {
    int pid;
    int money;
    // Number of V1 sites visited so far
    int v1Counter;
    // Number of V2 sites visited so far
    int v2Counter;
    int points;
    int currentSite;
    // Stores the last turn this player moved
    int lastTurn;
    Deck* deck;
} Player;

/**
 * Initialises the player.
 * Param:
 * pid - the player ID of the player.
 * Returns the initialised player.
 */
Player* init_player(int pid);

/**
 * Frees the players.
 * Param:
 * players - the players to be freed.
 * playerCount - the number of players.
 */
void free_players(Player** players, int playerCount);

/**
 * Calculates the player's score.
 * Param:
 * player - the player to calculate the score of.
 * Returns the player's score.
 */
int calculate_player_score(Player* player);

#endif