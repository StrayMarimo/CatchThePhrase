#include "sizes.h"
#include "player.h"
#include "socket_communication.h"
#include "common_utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include "special_chars.h"


struct Player CreatePlayer() {
    struct Player player;
    player.score = 100;
    for(int i = 0; i < 26; i++){
        player.letters_pressed[i] = 'A' + i;
    }
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

    PrintHashedCharacter(player->progress);
}

bool isLetterPressed(struct Player *player, char letter){
    for (int i = 0; i < 26; i++){
        if(letter == player->letters_pressed[i]){
            player->letters_pressed[i] = '*';
            return false;
        } 
        
    }
    return true;
}