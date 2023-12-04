#include "player.h"
#include "common_utils.h"


struct Player CreatePlayer() {
    struct Player player;
    player.score = 100;
    player.opponent_score = 100;
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

// char InputLetter(struct Player *player) {
//     PrintLine("Guess a Letter: ");
//     char letter = getch();
//     PrintLine("\n");
//     while (isLetterPressed(player, letter)) {
//         PrintLine("\nYou have already pressed that letter. Guess again: ");
//         letter = getch();
//         PrintLine("\n"); 
//     }
//     return letter;
// }

// bool isLetterPressed(struct Player *player, char letter){
//     for (int i = 0; i < 26; i++){
//         if(toupper(letter) == player->letters_pressed[i]){
//             player->letters_pressed[i] = '*';
//             return false;
//         } 
        
//     }
//     return true;
// }

// bool IsPhraseGuessed(char *phrase, char *progress){
//     for (int i = 0; i < strlen(phrase); i++){
//         if(toupper(phrase[i]) != toupper(progress[i])){
//             return false;
//         } 
//     }
//     return true;
// }

// bool SetOpponentProgress(struct Player *player, char letter, int client_sock) {
//     bool isLetterInPhrase = false;

//     for(int i = 0; i < strlen(player->player_phrase); i++){
//         if(toupper(letter) == player->player_phrase[i]){
//             player->opponent_progress[i] = letter;
//             isLetterInPhrase = true;
//         }
//     }
//     if (!isLetterInPhrase) {
//         PrintLine("Your opponent guessed a letter not in the phrase.\n");
//     }
    
//     PrintLine("Opponent Progress: ");
//     PrintHashedCharacter(player->opponent_progress);
    
//     if (IsPhraseGuessed(player->opponent_progress, player->player_phrase)) {
//         PrintLine("Your opponent guessed the phrase!\n");

//         return true;
//     }

//     return false;
// }

// bool SetProgress(struct Player *player, char letter, int client_sock) {
//     bool isLetterInPhrase = false;

//     for(int i = 0; i < 26; i++)
//         PrintLine("%c ", player->letters_pressed[i]);
//     PrintLine("\n");

//     for (int i = 0; i < strlen(player->opponent_phrase); i++) {
//         if (toupper(letter) == player->opponent_phrase[i]) {
//             isLetterInPhrase = true;
//             player->progress[i] = letter;
//         }
//     }

//     if (!isLetterInPhrase) {
//         player->score -= 10;
//         PrintLine("Letter not in phrase. Score: %d\n", player->score);
//     }

//     PrintLine("Your Progress: ");
//     PrintHashedCharacter(player->progress);

//     if (IsPhraseGuessed(player->progress, player->opponent_phrase)) {
//         PrintLine("You guessed the phrase!\n");
//         SendMessage(client_sock, &letter, false);
//         return true;
//     }

//     SendMessage(client_sock, &letter, false);
//     return false;
// }

// void RevealNotPresentLetter(struct Player *player){
//     bool isRevealed = false;
//     while(!isRevealed){
//         char randomletter = 'A' + (rand() % 26);
//         bool isPresent = false;
//         // find if letter is not in phrase
//         for(int i = 0; strlen(player->opponent_phrase); i++){
//             if(randomletter == player->opponent_phrase[i]){
//                 isPresent = true;
//                 break;
//             }
//         }
//         if(!isPresent){
//             for(int i = 0; i < 26; i++){
//                 if(player->letters_pressed[i] == randomletter){
//                     player->letters_pressed[i] = randomletter;
//                     isRevealed = true;
//                     break;
//                 }
//             }
//         }
//     }
// }

// char RevealALetter(struct Player *player){
//     char lettterToReveal;
//     time_t t;
//     srand((unsigned) time (&t));
//     bool isDone = false;
//     while(!isDone){
//         int i = rand() % strlen(player->opponent_phrase);
//         if(player->progress[i] != player->opponent_phrase[i])
//             lettterToReveal = player->opponent_phrase[i];
//             isDone = true;
//     }
//     return lettterToReveal;

// }
