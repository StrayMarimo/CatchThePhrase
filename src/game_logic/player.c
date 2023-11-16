#include "sizes.h"
#include "player.h"
#include "socket_communication.h"
#include <string.h>
#include <stdio.h>
#include <stdbool.h>


struct Player CreatePlayer() {
    struct Player player;
    player.score = 100;
    return player;
}

void SetPhrase(struct Player *player, int client_sock) {
    printf("Enter a phrase: ");
    SendMessage(client_sock, player->phrase, true);
    printf("Your opponent will guess the phrase: %s", player->phrase);
}

void SetGuessPhrase(struct Player *player, int client_sock) {
    char buffer[MAX_STRING_SIZE];
    ReceiveMessage(client_sock, buffer, true);

    strcpy(player->guessphrase, buffer);
    memset(player->progress, '*', strlen(player->guessphrase));
    printf("Catch the Phrase: %s\n", player->progress);
}