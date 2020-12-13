#include "game.h"
#include "errors.h"
#include "utility.h"
#include <stdlib.h>

/**
 * Initialises the game based on the given parameters.
 * Param:
 * gid - the game id of the game.
 * playerCount - the number of players.
 * pathString - contains the info about the path.
 * Returns the initialised game.
 */
Game* init_game(int gid, int playerCount, char* pathString) {
    Game* game = (Game*) malloc(sizeof(Game));
    game->gid = gid;
    game->playerCount = playerCount;
    game->turnCounter = 0;
    game->path = read_path(pathString, playerCount);
    if (game->path == NULL) {
        free(game);
        return NULL;
    }
    game->players = (Player**) malloc(sizeof(Player*) * playerCount);
    for (int i = 0; i < playerCount; i++) {
        game->players[i] = init_player(i);
    }
    return game;
}

/**
 * Frees the game given.
 * Param:
 * game - the game to be freed.
 */
void free_game(Game* game) {
    free_players(game->players, game->playerCount);
    free_path(game->path);
    free(game);
}

/**
 * Get the player whose turn it is next.
 * Param:
 * game - contains the current game info.
 * Returns the player ID of the player whose turn it is next.
 */
int get_next_turn(Game* game) {
    int turnOrder[game->playerCount];
    get_turn_order(game, turnOrder);
    return turnOrder[0];
}

/**
 * Swaps the player at i with the player at j.
 * Param:
 * players - the array of players
 * i - the index of a player.
 * j - the index of a player.
 */
void swap_players(Player** players, int i, int j) {
    Player* temp = players[i];
    players[i] = players[j];
    players[j] = temp;
}

/**
 * Sorts the players that are at the same site.
 * Param:
 * players - the players to be sorted.
 * playerCount - the number of players.
 */
void sort_players_same_site(Player** players, int playerCount) {
    bool sorted = false;
    while (!sorted) {
        sorted = true;
        for (int i = 0; i < playerCount - 1; i++) {
            if (players[i]->currentSite == players[i + 1]->currentSite) {
                if (players[i]->lastTurn < players[i + 1]->lastTurn) {
                    swap_players(players, i, i + 1);
                    sorted = false;
                } else if (players[i]->lastTurn == players[i + 1]->lastTurn &&
                        players[i]->pid > players[i + 1]->pid) {
                    swap_players(players, i, i + 1);
                    sorted = false;
                }
            }
        }
    }
}

/**
 * Fills turnOrder with the turn order of the players.
 * Param:
 * game - contains the current game info.
 * turnOrder - the array to be filled with the turn order of the players.
 */
void get_turn_order(Game* game, int* turnOrder) {
    Player* players[game->playerCount];
    for (int i = 0; i < game->playerCount; i++) {
        players[i] = game->players[i];
    }
    sort_players_by_site(players, game->playerCount);
    sort_players_same_site(players, game->playerCount);
    for (int i = 0; i < game->playerCount; i++) {
        turnOrder[i] = players[i]->pid;
    }
}

/**
 * Sorts players by their current site number.
 * Param:
 * players - the players to be sorted.
 * playerCount - the number of players.
 */
void sort_players_by_site(Player** players, int playerCount) {
    bool sorted = false;
    while (!sorted) {
        sorted = true;
        for (int i = 0; i < playerCount - 1; i++) {
            if (players[i]->currentSite > players[i + 1]->currentSite) {
                swap_players(players, i, i + 1);
                sorted = false;
            }
        }
    }
}

/**
 * Checks if the move is valid.
 * Param:
 * game - contains the current game info.
 * pid - the player ID of the player that is moving.
 * destSite - the site that the player is moving to.
 * Returns true if valid, else false.
 */
bool valid_move(Game* game, int pid, int destSite) {
    if (site_occupied(game, destSite)) {
        return false;
    }
    if (destSite > get_next_barrier(
            game->path, game->players[pid]->currentSite)) {
        return false;
    }
    return true;
}

/**
 * Checks if the site is fully occupied.
 * Param:
 * game - contains the current game info.
 * siteNum - the number of the site to be checked.
 * Returns true if the site is fully occupied, else false.
 */
bool site_occupied(Game* game, int siteNum) {
    int sitePlayerCount = 0;
    for (int i = 0; i < game->playerCount; i++) {
        if (game->players[i]->currentSite == siteNum) {
            sitePlayerCount++;
        }
    }
    if (sitePlayerCount >= game->path->sitePlayerCapacities[siteNum]) {
        return true;
    }
    return false;
}

/**
 * Checks if the game is over.
 * Param:
 * game - contains the current game info.
 * Returns true if the game is over, else false.
 */
bool game_over(Game* game) {
    for (int i = 0; i < game->playerCount; i++) {
        if (!(game->players[i]->currentSite == game->path->numOfSites - 1)) {
            return false;
        }
    }
    return true;
}

/**
 * Gets the site number of the next time the chosen site occurs in the path
 *          for the given player.
 * Param:
 * game - contains the current game info.
 * pid - the player ID of the player.
 * site - the site to be searched for.
 * Returns the site number of the next time the chosen site occurs in the
 *         path for the given player, else -1 if the site does not appear.
 */
int get_next_site(Game* game, int pid, Site site) {
    int currentSite = game->players[pid]->currentSite;
    for (int i = currentSite + 1; i < get_next_barrier(
            game->path, currentSite); i++) {
        if (game->path->sites[i] == site && !site_occupied(game, i)) {
            return i;
        }
    }
    return -1;
}