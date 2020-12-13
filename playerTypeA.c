#include "game.h"
#include "path.h"

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
    int nextBarrier = get_next_barrier(game->path, currentSite);
    if (game->players[pid]->money > 0) {
        for (int i = nextSite; i < nextBarrier; i++) {
            if (valid_move(game, pid, i) && game->path->sites[i] == DO) {
                return i;
            }
        }
    }
    if (valid_move(game, pid, nextSite) && 
            game->path->sites[nextSite] == MO) {
        return nextSite;
    }
    for (int i = nextSite; i <= nextBarrier; i++) {
        if (valid_move(game, pid, i)) {
            Site site = game->path->sites[i];
            if (site == V1 || site == V2 || site == BARRIER) {
                return i;
            }
        }
    }
    return -1;
}