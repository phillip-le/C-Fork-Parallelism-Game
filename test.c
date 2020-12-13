#include "stdio.h"
#include "string.h"
#include "path.h"
#include "deck.h"
#include "stdlib.h"
#include "player.h"
#include "errors.h"
#include "coms.h"
#include "game.h"
#include "utility.h"
#include "dealer.h"

void init_games(int playerCount, Game** game) {
    for (int i = 0; i < playerCount; i++) {
        char* pathString = (char*) malloc(sizeof(char) * 80);
        strcpy(pathString, "7;::-Mo1V11V22Mo1Mo1::-");
        game[i] = init_game(i, playerCount, pathString);
        free(pathString);
    }
}

void print_deck(Deck* deck) {
    printf("Cardcount: %d\nCards: ", deck->cardCount);
    for (int i = 0; i < deck->cardCount; i++) {
        printf("%c", get_card_char(deck->cards[i]));
    }
    printf("\n");
    fflush(stdout);
}

void print_player(Path* path, Player* player, int playerCount) {
    printf("Player ID: %d\n", player->pid);
    printf("Player Count: %d\n", playerCount);
    printf("Money: %d\n", player->money);
    printf("V1: %d\n", player->V1);
    printf("V2: %d\n", player->V2);
    printf("Points: %d\n", player->points);
    printf("Current Site: %d\n", player->currentSite);
    print_deck(player->deck);
    // print_path(path, stdout);
}

void test_highest_occupancy(void) {
    int playerCount = 3;
    Game* game[playerCount];
    init_games(playerCount, game);
    char* msgs[3] = {"HAP1,2,0,0,0", "HAP0,2,0,0,0", "HAP0,3,0,0,0"};
    printf("%d\n", get_highest_occupied(game[0]));
    for (int i = 0; i < 3; i++) {
        recv_player_update(game[0], msgs[i]);
        printf("%d\n", get_highest_occupied(game[0]));
    }
}

void test_game_over(void) {
    int playerCount = 3;
    Game* game[playerCount];
    init_games(playerCount, game);
    char* msgs[4] = {"HAP0,6,0,0,0", "HAP1,6,0,0,0", "HAP2,6,0,0,0", "HAP0,6,0,0,0"};
    int i = 0;
    while (!game_over(game[0])) {
        recv_player_update(game[0], msgs[i++]);
    }
}

void test_splice(void) {
    int playerCount = 3;
    Game* game[playerCount];
    init_games(playerCount, game);
    read_player_update(game[0], "HAP0,1,0,0,0");
}

void test_dealer(void) {
    char* pathString = (char*) malloc(sizeof(char) * 80);
    strcpy(pathString, "7;::-Mo1V11V22Mo1Mo1::-");
    char* deckString = (char*) malloc(sizeof(char) * 80);
    strcpy(deckString, "7ABACDEE");
    Dealer* dealer = init_dealer(2, deckString, pathString);
    for (int i = 0; i < 10; i++) {
        printf("%c\n", get_card_char(dealer->deck->cards[dealer->nextCard]));
        next_card(dealer);
    }
    free(dealer);
}

int main(void) {
    test_dealer();
    return 0;
}