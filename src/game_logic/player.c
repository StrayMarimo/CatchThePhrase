#include "sizes.h"
#include "player.h"
#include "socket_communication.h"
#include <string.h>
#include <ctype.h>
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
    printf("Your opponent will guess the phrase: %s", player->player_phrase);
}

char* encryptPhrase(char* phrase, int phraseLength) {
    char* encryptedPhrase;
    encryptedPhrase = (char*)malloc(phraseLength);

    strcpy(encryptedPhrase, phrase);

    // loop over phrase
    // check if character is " "(space);
    // if true change to asterisk

    for (int i = 0; i < phraseLength; i++) {
        if (isalpha(phrase[i])) {
            encryptedPhrase[i] = '*';
        }
    }

    return encryptedPhrase;
} 

void SetGuessPhrase(struct Player *player, int client_sock) {
    printf("Waiting for opponent to choose a topic...\n");
    char buffer[MAX_STRING_SIZE];
    ReceiveMessage(client_sock, buffer, true);

    strcpy(player->opponent_phrase, buffer);

    printf("Opponent Phrase: %s\n", player->opponent_phrase);

    strcpy(player->progress, encryptPhrase(player->opponent_phrase, strlen(player->opponent_phrase)));
    printf("Encrypted Phrase: %s\n", player->progress);
}