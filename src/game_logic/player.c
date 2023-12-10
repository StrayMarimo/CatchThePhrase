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


bool updateProgress(struct Player *player, char letter, bool isPlayer){
    bool isLetterInPhrase = false;
    char phrase[MAX_STRING_SIZE];
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

void ReceiveRevealLetter(struct Player *player, int client_sock){
    char buffer[MAX_STRING_SIZE];
    ReceiveMessage(client_sock, buffer);
    char new_message[MAX_STRING_SIZE];
    sprintf(new_message, "%s%c", REVEAL_A_LETTER_OPPONENT, buffer[0]);
    AddSystemMessage(new_message);

    updateProgress(player, buffer[0], false);
}



bool SetOpponentProgress(struct Player *player, int client_sock) {
    char buffer[MAX_STRING_SIZE];
    bool isLetterInPhrase = false;
    bool isMarkedSpot = false;

    ReceiveMessage(client_sock, buffer);

    if (isLetterPressed(player, buffer[0], false)) {
        AddSystemMessage(ALREADY_GUESSED);
        AddSystemMessage(GUESS_PHRASE);
        return false;
    }

    char new_message[MAX_STRING_SIZE] = PRINT_LETTER_OPPONENT;
    sprintf(new_message, "%s%c", new_message, buffer[0]);
    AddSystemMessage(new_message);

    if (IsMarkedSpot(player, buffer[0], false)) {
        AddSystemMessage(GUESS_MARKED_SPOT_OPPONENT);
        isMarkedSpot = true;
    }

    isLetterInPhrase = updateProgress(player, buffer[0], false);

    if (isMarkedSpot) {
        ReceiveRevealLetter(player, client_sock);
    }


    if (!isLetterInPhrase)
        AddSystemMessage(OPPONENT_NOT_IN_PHRASE);
    
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
    bool isMarkedSpot = false;

    char phraseBuffer[MAX_STRING_SIZE] = "";
    sprintf(phraseBuffer, "%c", letter);
    SendMessage(client_sock, phraseBuffer);

    if (isLetterPressed(player, letter, true)) {
        AddSystemMessage(ALREADY_GUESSED);
        AddSystemMessage(GUESS_PHRASE);
        return false;
    }

    if (IsMarkedSpot(player, letter, true)) {
        AddSystemMessage(GUESS_MARKED_SPOT);
        isMarkedSpot = true;
    }
    isLetterInPhrase =  updateProgress(player, letter, true);

    if (isMarkedSpot) {
        RevealALetter(player, client_sock, isGuessing, isWaitingForGuess);
        // PowerUpTrigger(player, client_sock, isGuessing, isWaitingForGuess);
    }

    if (!isLetterInPhrase) {
        player->score -= 10;
        AddSystemMessage(NOT_IN_PHRASE);
    }

    *isGuessing = false;
    *isWaitingForGuess = true;


    if (IsPhraseGuessed(player->progress, player->opponent_phrase)) {
        AddSystemMessage(PLAYER_WON);
        return true;
    }

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
}

void ReceiveRevealNotPresentLetter(struct Player *player, int client_sock){
    char *buffer;
    for(int i = 0; i < 26; i++){
        if(toupper(buffer[0]) == player->opponent_letters_pressed[i]){
            player->opponent_letters_pressed[i] = '*';
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
    char new_message[MAX_STRING_SIZE] = "The letter ";
    sprintf(new_message, "%s%c is in the phrase.", new_message, lettterToReveal);
    AddSystemMessage(new_message);
    
    for (int i = 0; i < strlen(player->opponent_phrase); i++) {
        if (toupper(lettterToReveal) == player->opponent_phrase[i]) {
            if (player->progress[i] == '^') {
                player->progress[i] = lettterToReveal;
                AddSystemMessage(GUESS_MARKED_SPOT);
                RevealALetter(player, client_sock, isGuessing, isWaitingForGuess);
                // PowerUpTrigger(player, client_sock, isGuessing, isWaitingForGuess);
            } 
            player->progress[i] = lettterToReveal;
            char letterMsg[MAX_STRING_SIZE];
            sprintf(letterMsg, "%c", lettterToReveal);
            SendMessage(client_sock,letterMsg);
            // ReceiveAck(client_sock);
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

// void PowerUpTrigger(struct Player *player, int client_sock) {

//     int randomInt = rand() % 2;

//         if (randomInt == 0) {
//             AddSystemMessage(REVEAL_A_LETTER); 
//             // RevealALetter(player);
//         } else {
//             AddSystemMessage(REVEAL_NOT_PRESENT_LETTER);
//             // RevealNotPresentLetter(player,client_sock);
//         }
//         RevealALetter(player);
// }
