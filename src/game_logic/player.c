#include "player.h"
#include "common_utils.h"


struct Player CreatePlayer() {
    struct Player player;
    player.score = 100;
    player.opponent_score = 100;
    player.consecutiveCorrectGuesses = 0;
    player.consecutiveCorrectGuessesOpponent = 0;

    for(int i = 0; i < 26; i++){
        player.letters_pressed[i] = 'A' + i;
        player.opponent_letters_pressed[i] = 'A' + i;
    }

    strcpy(player.player_phrase, "");
    strcpy(player.opponent_phrase, "");
    strcpy(player.progress, "");
    strcpy(player.opponent_progress, "");
    strcpy(player.turn, "");

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
    strcpy(player->player_phrase, CapitalizePhrase(player->player_phrase));
    char *encryptedPhrase = EncryptPhrase(player->player_phrase);
    strcpy(player->opponent_progress, encryptedPhrase);
    SendMessage(client_sock, player->player_phrase);
    SendMessage(client_sock, encryptedPhrase);
}

void SetGuessPhrase(struct Player *player, int client_sock) {
    ReceiveMessage(client_sock, player->opponent_phrase);
    ReceiveMessage(client_sock, player->progress);
}

bool isLetterPressed(struct Player *player, char letter){
    for (int i = 0; i < 26; i++){
        if(toupper(letter) == toupper(player->letters_pressed[i])){
            return false;
        } 
    }
    AddSystemMessage(ALREADY_GUESSED);
    return true;
}

void UpdateLetterPressed(struct Player *player, char letter, bool isPlayer){
    char letters_pressed[MAX_STRING_SIZE];
    if (isPlayer)
        strcpy(letters_pressed, player->letters_pressed);
    else 
        strcpy(letters_pressed, player->opponent_letters_pressed);
    for (int i = 0; i < 26; i++){
        if(toupper(letter) == letters_pressed[i]){
            if (isPlayer) 
                player->letters_pressed[i] = '*';
            else 
                player->opponent_letters_pressed[i] = '*';
            break;
        } 
    }
}

bool IsPhraseGuessed(char *phrase, char *progress){
    for (int i = 0; i < strlen(phrase); i++){
        if(toupper(phrase[i]) != toupper(progress[i])){
            return false;
        } 
    }
    return true;
}


bool UpdateProgress(struct Player *player, char letter, bool isPlayer){
    bool isLetterInPhrase = false;
    char phrase[MAX_STRING_SIZE];
    UpdateLetterPressed(player, letter, isPlayer);
    if (isPlayer) {
        strcpy(phrase, player->opponent_phrase);
    }
    else {
        strcpy(phrase, player->player_phrase);
    }
    for(int i = 0; i < strlen(phrase); i++){
        if(toupper(letter) == phrase[i]){
            if(isPlayer)
                player->progress[i] = letter;
            else
                player->opponent_progress[i] = letter;
            isLetterInPhrase = true;
        }
    }

    return isLetterInPhrase;
}

bool SetOpponentProgress(struct Player *player, int client_sock) {
    char buffer[MAX_STRING_SIZE];
    bool isLetterInPhrase = false;
    bool isMarkedSpot = false;

    ReceiveMessage(client_sock, buffer);

    strcpy(player->turn, YOUR_TURN);
    char new_message[MAX_STRING_SIZE] = PRINT_LETTER_OPPONENT;
    sprintf(new_message, "%s%c", new_message, buffer[0]);
    AddSystemMessage(new_message);

    if (IsMarkedSpot(player, buffer[0], false)) {
        AddSystemMessage(GUESS_MARKED_SPOT_OPPONENT);
        isMarkedSpot = true;
    }

    isLetterInPhrase = UpdateProgress(player, buffer[0], false);

    if (isMarkedSpot) {
        ReceiveRevealLetter(player, client_sock);
    }

    if (!isLetterInPhrase) {
        AddSystemMessage(OPPONENT_NOT_IN_PHRASE);
        player->consecutiveCorrectGuessesOpponent = 0;
        player->opponent_score -=30;
    } else player->consecutiveCorrectGuessesOpponent++;


    if (player->consecutiveCorrectGuessesOpponent == 3) {
        AddSystemMessage(THREE_IN_A_ROW_OPPONENT);
        ReceiveRevealNotPresentLetter(player, client_sock);
        player->consecutiveCorrectGuessesOpponent = 0;
    }
    
    if (IsPhraseGuessed(player->opponent_progress, player->player_phrase)) {
        AddSystemMessage(OPPONENT_WON);
        SendAck(client_sock);
        return true;
    }
    
    AddSystemMessage(GUESS_PHRASE);

    return false;
}


bool SetProgress(struct Player *player, char letter, int client_sock, bool *isGuessing, bool *isWaitingForGuess) {
    bool isLetterInPhrase = false;
    bool isMarkedSpot = false;

    char phraseBuffer[MAX_STRING_SIZE] = "";
    sprintf(phraseBuffer, "%c", letter);
    SendMessage(client_sock, phraseBuffer);

    if (IsMarkedSpot(player, letter, true)) {
        AddSystemMessage(GUESS_MARKED_SPOT);
        isMarkedSpot = true;
    }
    isLetterInPhrase =  UpdateProgress(player, letter, true);

    if (isMarkedSpot)
        RevealALetter(player, client_sock, isGuessing, isWaitingForGuess);

    if (!isLetterInPhrase) {
        player->score -= 30;
        player->consecutiveCorrectGuesses = 0;
        AddSystemMessage(NOT_IN_PHRASE);
    } else player->consecutiveCorrectGuesses++;

    if (player->consecutiveCorrectGuesses == 3) {
        AddSystemMessage(THREE_IN_A_ROW);
        RevealNotPresentLetter(player, client_sock);
        player->consecutiveCorrectGuesses = 0;
    }

    *isGuessing = false;
    *isWaitingForGuess = true;


    if (IsPhraseGuessed(player->progress, player->opponent_phrase)) {
        AddSystemMessage(PLAYER_WON);
        return true;
    }

    return false;
}


