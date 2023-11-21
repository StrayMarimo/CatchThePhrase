#include "sizes.h"
#include "player.h"
#include "socket_communication.h"
#include "common_utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


struct Player CreatePlayer() {
    struct Player player;
    player.score = 100;
    return player;
}

void SetPhrase(struct Player *player, int client_sock) {
    printf("Enter a phrase: ");
    SendMessage(client_sock, player->player_phrase, true);
    strcpy(player->player_phrase, CapitalizePhrase(player->player_phrase));
    printf("Your opponent will guess the phrase: %s", player->player_phrase);
    strcpy(player->opponent_progress, EncryptPhrase(player->player_phrase));
}

void SetGuessPhrase(struct Player *player, int client_sock) {
    printf("Waiting for opponent to choose a topic...\n");
    char buffer[MAX_STRING_SIZE];
    ReceiveMessage(client_sock, buffer, true);

    strcpy(player->opponent_phrase, CapitalizePhrase(buffer));

    printf("Opponent Phrase: %s\n", player->opponent_phrase);

    strcpy(player->progress, EncryptPhrase(player->opponent_phrase));
    printf("Encrypted Phrase: %s\n", player->progress);
}