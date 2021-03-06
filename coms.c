#include "coms.h"
#include "player.h"
#include "errors.h"
#include "deck.h"
#include <stdlib.h>
#include "utility.h"
#include <string.h>
#include <unistd.h>

/**
 * Sends the msg to all players.
 * Param:
 * dealer - the dealer keeping track of the players.
 * msg - the message to be sent
 */
void send_msg_to_all_player(Dealer* dealer, char* msg) {
    for (int i = 0; i < dealer->game->playerCount; i++) {
        fprintf(dealer->comStreams[i][STDIN_FILENO], "%s", msg);
        fflush(dealer->comStreams[i][STDIN_FILENO]);
    }
}

/**
 * Sends EARLY message to all players and exits.
 * Param:
 * dealer - the dealer of the game.
 */
void end_early(Dealer* dealer) {
    send_msg_to_all_player(dealer, "EARLY\n");
    exit(dealer_error_msg(BAD_PLAYER));
}

/**
 * Checks that the player is ready and sends the path information. Exits if
 *         a communication error occurs.
 * Param:
 * dealer - the dealer of the game.
 * pathString - string containing the path information.
 */
void send_path(Dealer* dealer, char* pathString) {
    for (int i = 0; i < dealer->game->playerCount; i++) {
        char msg = fgetc(dealer->comStreams[i][STDOUT_FILENO]);
        if (msg != '^') {
            end_early(dealer);
        } else {
            fprintf(dealer->comStreams[i][STDIN_FILENO], "%s\n", pathString);
            fflush(dealer->comStreams[i][STDIN_FILENO]);
        }
    }
}

/**
 * Gets the updated player info from the update string. Exits if the update
 *         is not formatted correctly.
 * Param:
 * game - contains the current game info.
 * update - the string containing the updated player info.
 * Returns an int array containing the info extracted from the update.
 */
int* read_player_update(Game* game, char* update) {
    // 12 is the min number of char in the update HAPp,n,s,m,c
    if (strlen(update) < 12) {
        exit(player_error_msg(PLAYER_COMS));
    }

    // Check if the start of the update is HAP
    int msgStartLength = 3;
    char msgStart[msgStartLength + 1];
    str_splice(msgStart, update, 0, msgStartLength);
    if (strcmp(msgStart, "HAP")) {
        exit(player_error_msg(PLAYER_COMS));
    }

    // Get values from the update string.
    char infoString[strlen(update) - msgStartLength + 1];
    str_splice(infoString, update, msgStartLength, strlen(update));
    int* info = (int*) calloc(5, sizeof(int));
    char* invalidPart;
    char* intString;
    for (int i = 0; i < 5; i++) {
        if (i == 0) {
            intString = strtok(infoString, ",");
        } else {
            intString = strtok(NULL, ",");
        }
        info[i] = strtol(intString, &invalidPart, 10);
        if (invalidPart[0] != '\0') {
            free(info);
            exit(player_error_msg(PLAYER_COMS));
        }
    }

    // Check that the values are valid
    int pid = info[PLAYER_ID_INDEX];
    int newSite = info[NEW_SITE_INDEX];
    if (pid < 0 || pid >= game->playerCount || newSite < 0 || 
            newSite > game->path->numOfSites - 1 ||
            info[CARD_INDEX] < 0 || info[CARD_INDEX] > 5) {
        free(info);
        exit(player_error_msg(PLAYER_COMS));
    }
    return info;
}

/**
 * Prints the information of the player to the file chosen.
 * Param:
 * file - the file stream to be printed to
 * player - the player whose information is printed
 */
void print_player_info(FILE* file, Player* player) {
    fprintf(file, "Player %d Money=%d V1=%d V2=%d ",
            player->pid, player->money, player->v1Counter, 
            player->v2Counter);
    int* cardCounters = get_card_counters(player->deck);
    fprintf(file, "Points=%d A=%d B=%d C=%d D=%d E=%d\n", 
            player->points, cardCounters[0], cardCounters[1], cardCounters[2],
            cardCounters[3], cardCounters[4]);
    fflush(file);
    free(cardCounters);
}

/**
 * Updates the player based on the updateInfo.
 * Param:
 * path - contains the path info.
 * player - the player to be updated.
 * updateInfo - contains the info to update the player.
 */
void update_player(Path* path, Player* player, int* updateInfo, 
        int currentTurn) {
    player->currentSite = updateInfo[NEW_SITE_INDEX];
    player->lastTurn = currentTurn;
    if (path->sites[updateInfo[NEW_SITE_INDEX]] == V1) {
        player->v1Counter++;
    }
    if (path->sites[updateInfo[NEW_SITE_INDEX]] == V2) {
        player->v2Counter++;
    }
    player->points += updateInfo[POINTS_INDEX];
    player->money += updateInfo[MONEY_INDEX];
    if (updateInfo[CARD_INDEX] != 0) {
        add_card(player->deck, updateInfo[CARD_INDEX]);
    } 
}

/**
 * Updates the player information of the player that moved according to the
 *         update.
 * Param:
 * game - contains the current game info.
 * update - the update on the player that moved.
 */
void recv_player_update(Game* game, char* update) {
    int* updateInfo = read_player_update(game, update);
    int pid = updateInfo[PLAYER_ID_INDEX];
    game->turnCounter++;
    update_player(game->path, game->players[pid], updateInfo, 
            game->turnCounter);
    print_player_info(stderr, game->players[pid]);
    print_path(stderr, game);
}

/**
 * Receives the dealer msg and updates the game accordingly. Exits if a
 *         a communication error occurs.
 * Param:
 * game - the game containing the current game information.
 * msg - the message received
 * Returns true if the dealer requests a move, else false.
 */
bool recv_dealer_msg(Game* game, char* msg) {
    if (!strcmp(msg, "YT")) {
        return true;
    }
    if (!strcmp(msg, "EARLY")) {
        exit(player_error_msg(EARLY_GAME_END));
    }
    if (!strcmp(msg, "DONE")) {
        print_scores(stderr, game);
        exit(player_error_msg(PLAYER_OK));
    }
    // Program will exit if an error occurred
    recv_player_update(game, msg);
    return false;
}

/**
 * Gets the move of the player whose turn it is. Exits if a communication 
 *         error occurs.
 * Param:
 * dealer - the dealer of the game.
 * Returns the site number of the site that the player wants to move to.
 */
int recv_player_move(Dealer* dealer) {
    int nextPlayer = get_next_turn(dealer->game);
    fprintf(dealer->comStreams[nextPlayer][STDIN_FILENO], 
            "YT\n");
    fflush(dealer->comStreams[nextPlayer][STDIN_FILENO]);
    char* playerMove = read_line(
            dealer->comStreams[nextPlayer][STDOUT_FILENO]);
    if (playerMove == NULL || playerMove[0] != 'D' || playerMove[1] != 'O') {
        end_early(dealer);
    }
    char* invalidPart;
    int siteNum = strtol(playerMove + 2, &invalidPart, 10);
    if (*invalidPart != '\0') {
        end_early(dealer);
    }
    return siteNum;
}

/**
 * Gets player update info based on the site that the player moved to.
 * Param:
 * dealer - the dealer of the game.
 * pid - the player ID of the player that moved.
 * destSite - the site that the player moved to.
 * Returns an int array containing the update to the player.
 */
int* get_player_update(Dealer* dealer, int pid, int destSite) {
    int* updateInfo = (int*) calloc(5, sizeof(int));
    switch (dealer->game->path->sites[destSite]) {
        case MO:
            updateInfo[MONEY_INDEX] = 3;
            break;
        case DO:
            updateInfo[POINTS_INDEX] = 
                    (int) dealer->game->players[pid]->money / 2;
            updateInfo[MONEY_INDEX] = dealer->game->players[pid]->money * -1;
            break;
        case RI:
            updateInfo[CARD_INDEX] = dealer->deck->cards[dealer->nextCard];
            next_card(dealer);
            break;
        case SITE_ERROR_TYPE:
            free(updateInfo);
            return NULL;
        default:
            break;
    }
    updateInfo[PLAYER_ID_INDEX] = pid;
    updateInfo[NEW_SITE_INDEX] = destSite;
    return updateInfo;
}

/**
 * Gets a HAPp,n,s,m,c formatted string which contains updated player info.
 *         p = player ID
 *         n = new site for player
 *         s = additional points for player
 *         m = change in money for player.
 *         c = card drawn by player
 * Param:
 * updateInfo - contains the values for p,n,s,m,c
 * Returns a HAPp,n,s,m,c formatted string which contains updated player info.
 */
char* get_update_string(int* updateInfo) {
    char* update = (char*) calloc(30, sizeof(char));
    sprintf(update, "HAP%d,%d,%d,%d,%d\n", updateInfo[PLAYER_ID_INDEX],
            updateInfo[NEW_SITE_INDEX], updateInfo[POINTS_INDEX],
            updateInfo[MONEY_INDEX], updateInfo[CARD_INDEX]);
    return update;
}

/**
 * Sends the update that occurred to due to a player move to all players. Also
 *         it prints out the info of the player that moved and the path.
 * Param:
 * dealer - the dealer of the game.
 * pid - the player ID of the player that moved.
 * destSite - the site the player moved to.
 */
void send_player_update(Dealer* dealer, int pid, int destSite) {
    if (!valid_move(dealer->game, pid, destSite)) {
        end_early(dealer);
    }
    int* updateInfo = get_player_update(dealer, pid, destSite);
    dealer->game->turnCounter++;
    update_player(dealer->game->path, dealer->game->players[pid], updateInfo, 
            dealer->game->turnCounter);
    char* update = get_update_string(updateInfo);
    send_msg_to_all_player(dealer, update);
    free(update);
    print_player_info(stdout, dealer->game->players[pid]);
    print_path(stdout, dealer->game);
    free(updateInfo);
}

/**
 * Gets the highest number of players at any given site on the path.
 * Param:
 * game - contains the current game information.
 * Returns the highest number of players at any given site on the path.
 */
int get_highest_occupied(Game* game) {
    int playerSites[game->playerCount];
    for (int i = 0; i < game->playerCount; i++) {
        playerSites[i] = game->players[i]->currentSite;
    }
    int numOfSites = game->path->numOfSites;
    int siteCounters[numOfSites];
    for (int i = 0; i < numOfSites; i++) {
        siteCounters[i] = 0;
    }
    for (int i = 0; i < game->playerCount; i++) {
        siteCounters[playerSites[i]]++;
    }
    return max(siteCounters, numOfSites);
}

/**
 * Creates space filled strings based on the highestSiteOccupancy and
 *         the pathStringSize.
 * Param:
 * highestSiteOccupancy - the number of strings to be created
 * pathStringSize - the width of the strings to be created.
 * Returns an array of space filled strings.
 */ 
char** create_position_strings(int highestSiteOccupancy, int pathStringSize) {
    char** positionStrings = (char**) malloc(
            sizeof(char*) * highestSiteOccupancy);
    for (int i = 0; i < highestSiteOccupancy; i++) {
        positionStrings[i] = (char*) malloc(sizeof(char) * pathStringSize);
        for (int j = 0; j < pathStringSize - 1; j++) {
            positionStrings[i][j] = ' ';
        }
        positionStrings[i][pathStringSize - 1] = '\0';
    }
    return positionStrings;
}

/**
 * Fills the path strings with the players in turn order.
 * Param:
 * game - contains the current game information
 * positionStrings - the strings of the path containing where players are.
 * highestSiteOccupancy - the highest number of players at any given site.
 * siteOffset - the offset for each site in the position string.
 */
void fill_position_strings(Game* game, char** positionStrings, 
        int highestSiteOccupancy, int* siteOffset) {
    int turnOrder[game->playerCount];
    get_turn_order(game, turnOrder);
    for (int i = game->playerCount - 1; i >= 0; i--) {
        int pid = turnOrder[i];
        int siteNum = game->players[pid]->currentSite;
        for (int j = 0; j < highestSiteOccupancy; j++) {
            if (positionStrings[j][siteOffset[siteNum]] == ' ') {
                positionStrings[j][siteOffset[siteNum]] = pid + '0';
                break;
            }
        }
    }
}

/**
 * Prints the path information to the file stream chosen.
 * Param:
 * file - the file stream to be printed to.
 * path - contains the path information to be printed
 */
void print_path(FILE* file, Game* game) {
    int pathStringSize = game->path->numOfSites * 3 + 1;
    // Stores the highest number of players at any site
    int highestSiteOccupancy = get_highest_occupied(game);
    char* pathString = (char*) calloc(pathStringSize, sizeof(char));
    char** positionStrings = create_position_strings(highestSiteOccupancy,
            pathStringSize);
    int siteOffset[game->path->numOfSites];
    // Get site labels
    int pathStringIndex = 0;
    for (int i = 0; i < game->path->numOfSites; i++) {
        siteOffset[i] = pathStringIndex;
        Site site = game->path->sites[i];
        strncat(pathString, get_site_string(site), 2);
        pathStringIndex += 2;
        pathString[pathStringIndex++] = ' ';
    }
    fill_position_strings(game, positionStrings, highestSiteOccupancy, 
            siteOffset);
    fprintf(file, "%s\n", pathString);
    for (int i = 0; i < highestSiteOccupancy; i++) {
        fprintf(file, "%s\n", positionStrings[i]);
    }
    fflush(file);
    free(pathString);
    for (int i = 0; i < highestSiteOccupancy; i++) {
        free(positionStrings[i]);
    }
    free(positionStrings);
}

/**
 * Prints the scores of all players to the file chosen.
 * Param:
 * file - the file stream to be printed to.
 * game - contains the current game info.
 */
void print_scores(FILE* file, Game* game) {
    fprintf(file, "Scores: ");
    for (int i = 0; i < game->playerCount - 1; i++) {
        fprintf(file, "%d,", calculate_player_score(game->players[i]));
    }
    fprintf(file, "%d\n", calculate_player_score(game->players[
            game->playerCount - 1]));
    fflush(file);
}