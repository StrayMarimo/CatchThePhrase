#include "sizes.h"
#include "player.h"
#include "socket_communication.h"
#include "common_utils.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ncurses.h>
#include <ctype.h>
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
    PrintLine(player->opponent_phrase);
    PrintHashedCharacter(player->progress);
}

char InputLetter(struct Player *player) {
    PrintLine("Guess a Letter: ");
    char letter = getch();
    PrintLine("\n");
    if (isLetterPressed(player, letter))
        PrintLine("\nYou have already pressed that letter.\n");
    return letter;
}

bool isLetterPressed(struct Player *player, char letter){
    for (int i = 0; i < 26; i++){
        if(toupper(letter) == player->letters_pressed[i]){
            player->letters_pressed[i] = '*';
            return false;
        } 
        
    }
    return true;
}

bool UpdatePhrase(struct Player *player, char letter, bool isOpponent){
    bool isLetterInPhrase = false;
    char phrase[MAX_STRING_SIZE];

    PrintLine("Updating Phrase...");
    PrintLine(player->player_phrase);
    if (isOpponent)
        strcpy(phrase, player->player_phrase);
    else
        strcpy(phrase, player->opponent_phrase);
    PrintLine(phrase); 
    
    for (int i = 0; i < strlen(phrase); i++){
        if(letter == phrase[i]){
            isLetterInPhrase = true;
            if (isOpponent)
                player->opponent_progress[i] = letter;
            else
                player->progress[i] = letter;
        } 
    }
    return isLetterInPhrase;
}

bool isPhraseGuessed(char *phrase, char *progress){
    for (int i = 0; i < strlen(phrase); i++){
        if(phrase[i] != progress[i]){
            return false;
        } 
    }
    return true;
}

bool SetOpponentProgress(struct Player *player, int client_sock) {
    char letter;
    ReceiveMessage(client_sock, &letter, false);
    PrintLine("\nLetter: %c\n", letter);

    if (UpdatePhrase(player, letter, true)) {
        PrintLine("Your opponent guessed correctly\n");
    } else {
        PrintLine("Wrong guess\n");
    }

    PrintHashedCharacter(player->opponent_progress);
    if (isPhraseGuessed(player->opponent_progress, player->player_phrase)) {
        PrintLine("Your opponent guessed the phrase!\n");
        return true;
    }

    return false;
}

bool SetProgress(struct Player *player, char letter, int client_sock) {
    PrintLine("Letter: %c\n", letter);
    SendMessage(client_sock, &letter, false);

    if (UpdatePhrase(player, letter, false)) {
        PrintLine("Correct!\n");
    } else {
        PrintLine("Incorrect!\n");
        player->score -= 10;
    }

    PrintHashedCharacter(player->progress);

    if (isPhraseGuessed(player->progress, player->opponent_phrase)) {
        PrintLine("You guessed the phrase!\n");
        return true;
    }
    return false;
}

