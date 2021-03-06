#ifndef COMS_H
#define COMS_H

#include <stdio.h>
#include "game.h"
#include "dealer.h"

// Indexes used for decoding dealer messages about player moves.
#define PLAYER_ID_INDEX 0
#define NEW_SITE_INDEX 1
#define POINTS_INDEX 2
#define MONEY_INDEX 3
#define CARD_INDEX 4

/**
 * Sends the msg to all players.
 * Param:
 * dealer - the dealer keeping track of the players.
 * msg - the message to be sent
 */
void send_msg_to_all_player(Dealer* dealer, char* msg);

/**
 * Checks that the player is ready and sends the path information. Exits if
 *         a communication error occurs.
 * Param:
 * dealer - the dealer of the game.
 * pathString - string containing the path information.
 */
void send_path(Dealer* dealer, char* pathString);

/**
 * Receives the dealer msg and updates the game accordingly. Exits if a
 *         a communication error occurs.
 * Param:
 * game - the game containing the current game information.
 * msg - the message received
 * Returns true if the dealer requests a move, else false.
 */
bool recv_dealer_msg(Game* game, char* msg);

/**
 * Gets the move of the player whose turn it is. Exits if a communication 
 *         error occurs.
 * Param:
 * dealer - the dealer of the game.
 * Returns the site number of the site that the player wants to move to.
 */
int recv_player_move(Dealer* dealer);

/**
 * Sends the update that occurred to due to a player move to all players. Also
 *         it prints out the info of the player that moved and the path.
 * Param:
 * dealer - the dealer of the game.
 * pid - the player ID of the player that moved.
 * destSite - the site the player moved to.
 */
void send_player_update(Dealer* dealer, int pid, int destSite);

/**
 * Prints the path information to the file stream chosen.
 * Param:
 * file - the file stream to be printed to.
 * path - contains the path information to be printed
 */
void print_path(FILE* file, Game* game);

/**
 * Prints the scores of all players to the file chosen.
 * Param:
 * file - the file stream to be printed to.
 * game - contains the current game info.
 */
void print_scores(FILE* file, Game* game);

#endif