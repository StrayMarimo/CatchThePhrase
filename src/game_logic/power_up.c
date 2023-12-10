#include "power_up.h"

bool IsMarkedSpot(struct Player *player, char letter, bool isPlayer) {

    char phrase[MAX_STRING_SIZE];
    char progress[MAX_STRING_SIZE];

    if (isPlayer) {
        strcpy(phrase, player->opponent_phrase);
        strcpy(progress, player->progress);
    }
    else {
        strcpy(phrase, player->player_phrase);
        strcpy(progress, player->opponent_progress);
    }

    for (int i = 0; i < strlen(phrase); i++) {
        if (toupper(letter) == phrase[i] && progress[i] == '^')
            return true;
    }

    return false;
}

void RevealNotPresentLetter(struct Player *player, int client_sock){
    bool isRevealed = false;
    bool isPresent = true;
    char *letter;
    char randomletter;
    time_t t;
    srand((unsigned) time (&t));
    while(isPresent){
        int randIndex = rand() % 26;
        randomletter = player->letters_pressed[randIndex];
        if (randomletter == '*')
            continue;
        isPresent = false;
        // find if letter is not in phrase
        for(int i = 0; i < strlen(player->opponent_phrase); i++){
            if(randomletter == player->opponent_phrase[i]){
                isPresent = true;
                break;
            }
        }
    }

    for(int i = 0; i < 26; i++){
        if(player->letters_pressed[i] == randomletter){
            player->letters_pressed[i] = '*';
            break;
        }
    }
    char new_message[MAX_STRING_SIZE] = REVEAL_NOT_PRESENT_LETTER;
    sprintf(new_message, "%s%c", new_message, randomletter);
    AddSystemMessage(new_message);

    char letterMsg[MAX_STRING_SIZE];
    sprintf(letterMsg, "%c", randomletter);
    SendMessage(client_sock, letterMsg);
}

void ReceiveRevealNotPresentLetter(struct Player *player, int client_sock){
    char buffer[MAX_STRING_SIZE];
    ReceiveMessage(client_sock, buffer);
    char new_message[MAX_STRING_SIZE] = REVEAL_NOT_PRESENT_LETTER_OPPONENT;
    sprintf(new_message, "%s%c", new_message, buffer[0]);
    AddSystemMessage(new_message);
    for(int i = 0; i < 26; i++){
        if(toupper(buffer[0]) == player->opponent_letters_pressed[i]){
            player->opponent_letters_pressed[i] = '*';
            break;
        }
    }
}

void RevealALetter(struct Player *player, int client_sock, bool *isGuessing, bool *isWaitingForGuess){
    char lettterToReveal;
    time_t t;
    srand((unsigned) time (&t));
    bool isDone = false;
    while(!isDone){
        int i = rand() % strlen(player->opponent_phrase);
        if(player->progress[i] != player->opponent_phrase[i] && 
            !isLetterPressed(player, player->opponent_phrase[i], true) && 
            player->progress[i] != '^'){
            lettterToReveal = player->opponent_phrase[i];
            isDone = true;
        }
    }
    for(int i = 0; i < 26; i++){
        if(player->letters_pressed[i] == lettterToReveal){
            player->letters_pressed[i] = lettterToReveal;
            break;
        }
    }
    char new_message[MAX_STRING_SIZE] = REVEAL_A_LETTER;
    sprintf(new_message, "%s%c!", new_message, lettterToReveal);
    AddSystemMessage(new_message);

    UpdateProgress(player, lettterToReveal, true);
    char letterMsg[MAX_STRING_SIZE];
    sprintf(letterMsg, "%c", lettterToReveal);
    SendMessage(client_sock,letterMsg);
}

void ReceiveRevealLetter(struct Player *player, int client_sock){
    char buffer[MAX_STRING_SIZE];
    ReceiveMessage(client_sock, buffer);
    char new_message[MAX_STRING_SIZE];
    sprintf(new_message, "%s%c", REVEAL_A_LETTER_OPPONENT, buffer[0]);
    AddSystemMessage(new_message);

    UpdateProgress(player, buffer[0], false);
}

bool CheckThreeInARow(struct Player *player) {
    if (IsPhraseGuessed(player->progress, player->opponent_phrase)) {
        player->consecutiveCorrectGuesses++;
        if (player->consecutiveCorrectGuesses == 3) {
            return true;
        }
    } else {
        player->consecutiveCorrectGuesses = 0;
    }

    return false;
}

bool CheckandResetThreeInARow(struct Player *player) {
    if (CheckThreeInARow(player)) {
        player->consecutiveCorrectGuesses = 0;
        return true;
    }
    return false;
}

// void PowerUpTrigger(struct Player *player, int client_sock) {

//     int randomInt = rand() % 2;

//         if (randomInt == 0) {
//             RevealALetter(player, );
//         } else {
//             RevealNotPresentLetter(player,client_sock);
//         }
//         RevealALetter(player);
// }