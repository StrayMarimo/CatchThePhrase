#include "player.h"
#include "common_utils.h"


struct Player CreatePlayer() {
    struct Player player;
    player.score = 100;
    player.opponent_score = 100;
    player.consecutiveCorrectGuesses = 0;
    for(int i = 0; i < 26; i++){
        player.letters_pressed[i] = 'A' + i;
        player.opponent_letters_pressed[i] = 'A' + i;
    }

    strcpy(player.player_phrase, "");
    strcpy(player.opponent_phrase, "");
    strcpy(player.progress, "");
    strcpy(player.opponent_progress, "");

    return player;
}

void PrintPlayer(struct Player player) {
    printf("Score: %d\n", player.score);
    printf("Phrase: %s\n", player.player_phrase);
    printf("Opponent Phrase: %s\n", player.opponent_phrase);
    printf("Progress: %s\n", player.progress);
    printf("Opponent Progress: %s\n\n", player.opponent_progress);
    printf("Letters Pressed: ");

    for(int i = 0; i < 26; i++)
        printf("%c ", player.letters_pressed[i]);
    

    printf("Letters Pressed Opponent: ");
    for(int i = 0; i < 26; i++)
        printf("%c ", player.opponent_letters_pressed[i]);
    
    printf("\n");
}

void SetPhrase(struct Player *player, int client_sock) { 
    SendMessage(client_sock, player->player_phrase);
    strcpy(player->player_phrase, CapitalizePhrase(player->player_phrase));
    strcpy(player->opponent_progress, EncryptPhrase(player->player_phrase));
}

void SetGuessPhrase(struct Player *player, int client_sock) {
    ReceiveMessage(client_sock, player->opponent_phrase);
    strcpy(player->opponent_phrase, CapitalizePhrase(player->opponent_phrase));
    strcpy(player->progress, EncryptPhrase(player->opponent_phrase));
}

bool isLetterPressed(struct Player *player, char letter, bool isPlayer){
    for (int i = 0; i < 26; i++){
        if(toupper(letter) == player->letters_pressed[i]){
            if (isPlayer)
                player->letters_pressed[i] = '*';
            else
                player->opponent_letters_pressed[i] = '*';
            return false;
        } 
        
    }
    return true;
}

bool IsPhraseGuessed(char *phrase, char *progress){
    for (int i = 0; i < strlen(phrase); i++){
        if(toupper(phrase[i]) != toupper(progress[i])){
            return false;
        } 
    }
    return true;
}

bool IsMarkedSpot(char* phrase, char* progress, char letter) {
    for (int i = 0; i < strlen(phrase); i++) {
        if (toupper(letter) == phrase[i] && progress[i] == '^') {
            return true;
        }
    }
    return false;
}

bool SetOpponentProgress(struct Player *player, int client_sock) {
    char buffer[MAX_STRING_SIZE];
    bool isLetterInPhrase = false;

    ReceiveMessage(client_sock, buffer);
    // SendAck(client_sock);
    
    if (isLetterPressed(player, buffer[0], false)) {
        AddSystemMessage(ALREADY_GUESSED);
        AddSystemMessage(GUESS_PHRASE);
        return false;
    }

    for(int i = 0; i < strlen(player->player_phrase); i++){
        if(toupper(buffer[0]) == player->player_phrase[i]){
            player->opponent_progress[i] = buffer[0];
            isLetterInPhrase = true;
        }
    }
    if (!isLetterInPhrase)
        AddSystemMessage(OPPONENT_NOT_IN_PHRASE);
    else {
        char new_message[MAX_STRING_SIZE] = PRTINT_LETTER_OPPONENT;
        sprintf(new_message, "%s%c", new_message, buffer[0]);
        AddSystemMessage(new_message);
    }
    
    // ReceiveMessage(client_sock, buffer);
    ReceiveRevealNotPresentLetter(player, client_sock);
    
    if (IsPhraseGuessed(player->opponent_progress, player->player_phrase)) {
        AddSystemMessage(OPPONENT_WON);
        player->opponent_score -=10;
        return true;
    }
    
    AddSystemMessage(GUESS_PHRASE);

    return false;
}

bool SetProgress(struct Player *player, char letter, int client_sock, bool *isGuessing, bool *isWaitingForGuess) {
    bool isLetterInPhrase = false;

    if (isLetterPressed(player, letter, true)) {
        AddSystemMessage(ALREADY_GUESSED);
        AddSystemMessage(GUESS_PHRASE);
        return false;
    }

    for (int i = 0; i < strlen(player->opponent_phrase); i++) {
        if (toupper(letter) == player->opponent_phrase[i]) {
            isLetterInPhrase = true;
            player->progress[i] = letter;
            if(IsMarkedSpot(player->opponent_phrase, player->progress,player->letters_pressed[i])){
                AddSystemMessage(GUESS_MARKED_SPOT);
                PowerUpTrigger(player, client_sock);
            }
        }
    }

    // if (IsMarkedSpot(player->opponent_phrase, player->progress, letter)) {
    //     
    // }

    if (!isLetterInPhrase) {
        player->score -= 10;
        AddSystemMessage(NOT_IN_PHRASE);
    }

    // ReceiveAck(client_sock);
    *isGuessing = false;
    *isWaitingForGuess = true;


    if (IsPhraseGuessed(player->progress, player->opponent_phrase)) {
        AddSystemMessage(PLAYER_WON);
        return true;
    }

    AddSystemMessage(OPPONENTS_TURN);
    return false;
}

void RevealNotPresentLetter(struct Player *player, int client_sock){
    bool isRevealed = false;
    char *letter;
    while(!isRevealed){
        char randomletter = 'A' + (rand() % 26);
        bool isPresent = false;
        // find if letter is not in phrase
        for(int i = 0; strlen(player->opponent_phrase); i++){
            if(randomletter == player->opponent_phrase[i]){
                isPresent = true;
                break;
            }
        }
        if(!isPresent){
            for(int i = 0; i < 26; i++){
                if(player->letters_pressed[i] == randomletter){
                    player->letters_pressed[i] = randomletter;
                    sprintf(letter, "%c", randomletter);
                    isRevealed = true;
                    break;
                }
            }
        }
    }
    AddSystemMessage(REVEAL_A_LETTER);
    // SendMessage(client_sock, letter);
}

void ReceiveRevealNotPresentLetter(struct Player *player, int client_sock){
    char *buffer;
    // ReceiveMessage(client_sock, buffer);
    for(int i = 0; i < 26; i++){
        if(toupper(buffer[0]) == player->opponent_letters_pressed[i]){
            player->opponent_letters_pressed[i] = '*';
        }
    }
}

void RevealALetter(struct Player *player){
    char lettterToReveal;
    time_t t;
    srand((unsigned) time (&t));
    bool isDone = false;
    while(!isDone){
        int i = rand() % strlen(player->opponent_phrase);
        if(player->progress[i] != player->opponent_phrase[i]){
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

void PowerUpTrigger(struct Player *player, int client_sock) {

    // char markedLetter = '\0';

    // for (int i =0; i < 26; i++) {
    //     if (player->letters_pressed[i] != '*' &&
    //         IsMarkedSpot(player->opponent_phrase, player->progress,player->letters_pressed[i])) {
    //         markedLetter = player->letters_pressed[i];
    //         break;
    //     }
    // }
    // if (markedLetter != '\0' || CheckThreeInARow(player)) {
        
    // }

    int randomInt = rand() % 2;

        if (randomInt == 0) {
            RevealALetter(player);
        } else {
            RevealNotPresentLetter(player,client_sock);
        }
}
