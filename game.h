#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "deck.h"
#include "path.h"

/**
 * Representation of the game.
 */
typedef struct {
    int gid;
    int playerCount;
    int turnCounter;
    Player** players;
    Path* path;
} Game;

/**
 * Initialises the game based on the given parameters.
 * Param:
 * gid - the game id of the game.
 * playerCount - the number of players.
 * pathString - contains the info about the path.
 * Returns the initialised game.
 */
Game* init_game(int gid, int playerCount, char* pathString);

/**
 * Frees the game given.
 * Param:
 * game - the game to be freed.
 */
void free_game(Game* game);

/**
 * Get the player whose turn it is next.
 * Param:
 * game - contains the current game info.
 * Returns the player ID of the player whose turn it is next.
 */
int get_next_turn(Game* game);

/**
 * Fills turnOrder with the turn order of the players.
 * Param:
 * game - contains the current game info.
 * turnOrder - the array to be filled with the turn order of the players.
 */
void get_turn_order(Game* game, int* turnOrder);

/**
 * Sorts players by their current site number.
 * Param:
 * players - the players to be sorted
 * playerCount - the number of players.
 */
void sort_players_by_site(Player** players, int playerCount);

/**
 * Checks if the move is valid.
 * Param:
 * game - contains the current game info.
 * pid - the player ID of the player that is moving.
 * destSite - the site that the player is moving to.
 * Returns true if valid, else false.
 */
bool valid_move(Game* game, int pid, int destSite);

/**
 * Checks if the site is fully occupied.
 * Param:
 * game - contains the current game info.
 * siteNum - the number of the site to be checked.
 * Returns true if the site is fully occupied, else false.
 */
bool site_occupied(Game* game, int siteNum);

/**
 * Checks if the game is over.
 * Param:
 * game - contains the current game info.
 * Returns true if the game is over, else false.
 */
bool game_over(Game* game);

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
int get_next_site(Game* game, int pid, Site site);

#endif