#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include "errors.h"
#include "utility.h"
#include "dealer.h"
#include "coms.h"

#define WRITE_END 1
#define READ_END 0
#define DECK_INDEX 1
#define PATH_INDEX 2

// Global variable for signal handler storing player process IDs
pid_t* playerProcessIDs;
// Signal handler number of player processes
int processCount = 0;

/**
 * Frees the pipefds arrays.
 * Param:
 * pipefds - the array to be freed.
 */
void free_pipefds(int** pipefds) {
    for (int i = 0; i < STREAM_COUNT; i++) {
        free(pipefds[i]);
    }
    free(pipefds);
}

/**
 * Sets up the player side of the pipes.
 * Param:
 * pipefds - the file descriptors for the pipes between the dealer and
 *         player processes.
 * Returns true if the setup was successful, else false.
 */
bool setup_player_side_pipes(int** pipefds) {
    for (int i = 0; i < STREAM_COUNT; i++) {
        if (i == STDIN_FILENO) {
            // Handle stdin
            close(pipefds[STDIN_FILENO][WRITE_END]); // Child closes write end
            // Remap pipe to stdin
            if (dup2(pipefds[STDIN_FILENO][READ_END], STDIN_FILENO) == -1) {
                free_pipefds(pipefds);
                return false;
            }
            close(pipefds[STDIN_FILENO][READ_END]);
        } else { // Handle stdout and stderr
            close(pipefds[i][READ_END]);
            if (dup2(pipefds[i][WRITE_END], i) == -1) {
                free_pipefds(pipefds);
                return false;
            }
            close(pipefds[i][WRITE_END]);
        }
    }
    free_pipefds(pipefds);
    return true;
}

/**
 * Sets up the dealer side of the pipes.
 * Param:
 * pipefds - the file descriptors for the pipes between the dealer and
 *         player processes.
 * files - the file streams for communication between the dealer and
 *         player processes.
 * Returns true if the setup was successful, else false.
 */
bool setup_dealer_side_pipes(int** pipefds, FILE** files) {
    // Handle stdin
    close(pipefds[STDIN_FILENO][READ_END]);
    files[STDIN_FILENO] = fdopen(pipefds[STDIN_FILENO][WRITE_END], "w");
    // Handle stdout and stderr
    close(pipefds[STDOUT_FILENO][WRITE_END]);
    files[STDOUT_FILENO] = fdopen(pipefds[STDOUT_FILENO][READ_END], "r");
    if (files[STDIN_FILENO] == NULL || files[STDOUT_FILENO] == NULL) {
        free_pipefds(pipefds);
        return false;
    }
    // Suppress stderr output
    close(pipefds[STDERR_FILENO][WRITE_END]);
    if (dup2(pipefds[STDERR_FILENO][READ_END], fileno(
            fopen("/dev/null", "w"))) == -1) {
        free_pipefds(pipefds);
        return false;
    }
    close(pipefds[STDERR_FILENO][READ_END]);
    free_pipefds(pipefds);
    return true;
}

/**
 * Kills all players and exits based on the given error.
 * Param:
 * error - the error which caused the program to exit.
 */
void kill_players(int error) {
    for (int i = 0; i < processCount; i++) {
        kill(playerProcessIDs[i], SIGKILL);
    }
    while (wait(0) > -1) {
    }
    free(playerProcessIDs);
    if (error == BAD_PLAYER) {
        exit(dealer_error_msg(BAD_PLAYER));
    } else if (error == DEALER_COMS) {
        exit(dealer_error_msg(DEALER_COMS));
    }
}

/**
 * Sets up the signal handler.
 */
void setup_signal_handler(void) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(struct sigaction));
    sa.sa_handler = kill_players;
    sa.sa_flags = SA_RESTART;
    sigaction(SIGHUP, &sa, 0);
}

/**
 * Starts up the players processes.
 * Param:
 * playerCount - the number of players.
 * paths - the paths to the player programs.
 * comStreams - the file streams for communication to players.
 */
void start_up_players(int playerCount, char** paths, FILE*** comStreams) {
    for (int i = 0; i < playerCount; i++) {
        int** pipefds = (int**) malloc(sizeof(int*) * STREAM_COUNT);
        for (int j = 0; j < STREAM_COUNT; j++) {
            pipefds[j] = (int*) malloc(sizeof(int) * 2);
            if (pipe(pipefds[j]) == -1) {
                kill_players(BAD_PLAYER);
            }
        }
        pid_t processID = fork();
        if (processID == -1) {
            kill_players(BAD_PLAYER);
        }
        playerProcessIDs[i] = processID;
        processCount++;
        // Setup player side
        if (!processID) {
            if (!setup_player_side_pipes(pipefds)) {
                exit(dealer_error_msg(BAD_PLAYER));
            }
            int bufferSize = 10;
            char countString[bufferSize], idString[bufferSize];
            sprintf(countString, "%d", playerCount);
            sprintf(idString, "%d", i);
            execlp(paths[i], paths[i], countString, idString, NULL);
            exit(dealer_error_msg(BAD_PLAYER));
        }
        if (!setup_dealer_side_pipes(pipefds, comStreams[i])) {
            kill_players(BAD_PLAYER);
        }
    }
}

/**
 * The main program of the game.
 * Param:
 * argc - number of arguments
 * argv - the arguments given
 * Return 0 if the game runs successfully, otherwise returns an error code.
 */
int main(int argc, char** argv) {
    if (argc < 4) {
        return dealer_error_msg(DEALER_ARGS);
    }

    int playerCount = argc - 3;

    Dealer* dealer = init_dealer(playerCount, argv[DECK_INDEX], 
            argv[PATH_INDEX]);

    // Setup signal handler so that players can be killed if SIGHUP occurs
    setup_signal_handler();
    playerProcessIDs = (pid_t*) malloc(sizeof(pid_t) * playerCount);

    char** playerPaths = argv + 3;
    start_up_players(playerCount, playerPaths, dealer->comStreams);

    FILE* pathFile = fopen(argv[PATH_INDEX], "r");
    char* pathString = read_line(pathFile);
    send_path(dealer, pathString);
    fclose(pathFile);
    free(pathString);
    print_path(stdout, dealer->game);

    // Run the game
    while (!game_over(dealer->game)) {
        send_player_update(dealer, get_next_turn(dealer->game), 
                recv_player_move(dealer));
    }
    send_msg_to_all_player(dealer, "DONE\n");
    print_scores(stdout, dealer->game);
    free_dealer(dealer);
    exit(DEALER_OK);
}
