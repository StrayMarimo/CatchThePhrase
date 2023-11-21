#include "sizes.h"
#include "player.h"
#include "socket_communication.h"
#include "common_utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>


struct Player CreatePlayer() {
    struct Player player;
    player.score = 100;
    return player;
}

void SetPhrase(struct Player *player, int client_sock) {
    PrintLine("Enter a phrase: ");
    SendMessage(client_sock, player->player_phrase, true);
    strcpy(player->player_phrase, CapitalizePhrase(player->player_phrase));

    PrintLine("Your opponent will guess the phrase: %s\n", player->player_phrase);
    strcpy(player->opponent_progress, EncryptPhrase(player->player_phrase));
}

void SetGuessPhrase(struct Player *player, int client_sock) {
    PrintLine("Waiting for opponent to set a phrase...\n");
    char buffer[MAX_STRING_SIZE];
    ReceiveMessage(client_sock, buffer, true);

    strcpy(player->opponent_phrase, CapitalizePhrase(buffer));
    strcpy(player->progress, EncryptPhrase(player->opponent_phrase));
    PrintLine("Encrypted Phrase: %s\n", player->progress);
}