#include "player.h"
#include "errors.h"
#include "utility.h"
#include <stdlib.h>
#include <string.h>

/**
 * Initialises the player.
 * Param:
 * pid - the player ID of the player.
 * Returns the initialised player.
 */
Player* init_player(int pid) {
    Player* player = (Player*) malloc(sizeof(Player));
    player->pid = pid;
    player->money = STARTING_MONEY;
    player->v1Counter = 0;
    player->v2Counter = 0;
    player->points = 0;
    player->currentSite = 0;
    player->lastTurn = 0;
    player->deck = create_deck(0, NULL);
    return player;
}

/**
 * Frees the players.
 * Param:
 * players - the players to be freed.
 * playerCount - the number of players.
 */
void free_players(Player** players, int playerCount) {
    for (int i = 0; i < playerCount; i++) {
        free_deck(players[i]->deck);
        free(players[i]);
    }
    free(players);
}

/**
 * Calculates the player's score.
 * Param:
 * player - the player to calculate the score of.
 * Returns the player's score.
 */
int calculate_player_score(Player* player) {
    return calculate_deck_score(player->deck) + player->points + 
            player->v1Counter + player->v2Counter;
}

