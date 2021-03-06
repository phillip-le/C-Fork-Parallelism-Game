#include "errors.h"
#include "utility.h"
#include "path.h"
#include "game.h"
#include "coms.h"
#include "move.h"
#include <stdlib.h>
#include <string.h>

/**
 * The main program of the game.
 * Param:
 * argc - number of arguments
 * argv - the arguments given
 * Return 0 if the game runs successfully, otherwise returns an error code.
 */
int main(int argc, char** argv) {
    if (argc != 3) {
        exit(player_error_msg(PLAYER_ARGS));
    }
    char* invalidPart1;
    char* invalidPart2;
    int playerCount = strtol(argv[1], &invalidPart1, 10);
    if (strlen(argv[1]) == 0 || *invalidPart1 != '\0' || playerCount < 1) {
        exit(player_error_msg(PLAYER_COUNT));
    }
    int pid = strtol(argv[2], &invalidPart2, 10);
    if (strlen(argv[2]) == 0 || *invalidPart2 != '\0' || pid < 0 || 
            pid >= playerCount) {
        exit(player_error_msg(PLAYER_BAD_ID));
    }

    printf("^");
    fflush(stdout);
    
    char* pathString = read_line(stdin);
    if (pathString == NULL) {
        exit(player_error_msg(PLAYER_BAD_PATH));
    }
    Game* game = init_game(pid, playerCount, pathString);
    if (game == NULL) {
        free(pathString);
        exit(player_error_msg(PLAYER_BAD_PATH));
    }
    free(pathString);
    print_path(stderr, game);
    while (1) {
        char* msg = read_line(stdin);
        if (msg != NULL) {
            if (recv_dealer_msg(game, msg)) {
                printf("DO%d\n", get_move(game, pid));
                fflush(stdout);
            }
        } else {
            exit(player_error_msg(PLAYER_COMS));
        }
    }
}