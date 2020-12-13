#include <stdlib.h>
#include <stdbool.h>
#include "game.h"
#include "player.h"
#include "path.h"
#include "utility.h"

/**
 * Checks if one player has more cards than everyone else.
 * Param:
 * game - contains the current game info.
 * maxCardCount - the highest number of cards any player has.
 * Returns true if there is one player with more cards than everyone else,
 *         else false.
 */
bool one_player_most_cards(Game* game, int maxCardCount) {
    int maxCardCounter = 0;
    for (int i = 0; i < game->playerCount; i++) {
        if (game->players[i]->deck->cardCount == maxCardCount) {
            maxCardCounter++;
        }
    }
    if (maxCardCounter == 1) {
        return true;
    } else {
        return false;
    }
}

/**
 * Gets the site that the player wants to move to.
 * Param:
 * game - contains the current game info.
 * pid - the player ID of the player that wants to move.
 * Returns the site number of the site that the player wants to move to.
 */
int get_move(Game* game, int pid) {
    int currentSite = game->players[pid]->currentSite;
    int nextSite = currentSite + 1;
    // Check if can go to nextSite
    Player* players[game->playerCount];
    for (int i = 0; i < game->playerCount; i++) {
        players[i] = game->players[i];
    }
    sort_players_by_site(players, game->playerCount);
    if (!site_occupied(game, nextSite)) {
        if (game->playerCount == 1 || (players[0]->pid == pid && 
                players[1]->currentSite > currentSite)) {
            return nextSite;
        }
    }

    // Check if can go to next Mo site
    if (game->players[pid]->money % 2 == 1 && 
            get_next_site(game, pid, MO) != -1) {
        return get_next_site(game, pid, MO);
    }
    
    // Check if can go to next Ri site
    int playerCardCounts[game->playerCount];
    for (int i = 0; i < game->playerCount; i++) {
        playerCardCounts[i] = game->players[i]->deck->cardCount;
    }
    int maxCardCount = max(playerCardCounts, game->playerCount);
    if ((((game->players[pid]->deck->cardCount == maxCardCount) &&
            one_player_most_cards(game, maxCardCount)) || maxCardCount == 0) 
            && get_next_site(game, pid, RI) != -1) {
        return get_next_site(game, pid, RI);
    }

    // Check if can go to next V2 site
    if (get_next_site(game, pid, V2) != -1) {
        return get_next_site(game, pid, V2);
    }

    // Get the next unoccupied site
    for (int i = nextSite; i < game->path->numOfSites; i++) {
        if (!site_occupied(game, i)) {
            return i;
        }
    }
    return -1; // Should never reach here
}