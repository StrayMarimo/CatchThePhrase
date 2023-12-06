#include "common_utils.h"


void DrawTextCenter(const char *text, int y, int font, Color color) {
    int textWidth = MeasureText(text, font);
    int x = (GetScreenWidth() - textWidth) / 2;
    DrawText(text, x, y, font, color);
}


bool ValidateArgs(char *file, int params_expected, int params_received) {
    char error_message[MAX_STRING_SIZE];
    if (params_expected != params_received) {
        if (params_expected == 2) 
            printf("Failed Setting up: %s port_no", file);
        else if (params_expected == 3)
            printf("Failed Setting up: %s hostname port_no", file);
        return false;
    }

    return true;
}

void DieWithError(char *error_message) {
    printf("%s\n", error_message);
    exit(EXIT_FAILURE);
}

char* EncryptPhrase(char* phrase) {
    // Turns alphabet to asterisks.

    char* encryptedPhrase;
    int phraseLength = strlen(phrase);
    int markedSpots = phraseLength/4;
    encryptedPhrase = (char*)malloc(phraseLength);

    strcpy(encryptedPhrase, phrase);
    
    int ctr = 0;
    time_t t;
    srand((unsigned) time(&t));
    while(ctr < markedSpots){
        int i = rand() % phraseLength;
        if(encryptedPhrase[i] != '^'){
            encryptedPhrase[i] = '^';
            ctr++;
        } 
    }
    for (int i = 0; i < phraseLength; i++) {
        if (isalpha(phrase[i]) && encryptedPhrase[i] != '^') {
            encryptedPhrase[i] = '*';
        }
    }

    return encryptedPhrase;
} 

char* CapitalizePhrase(char* phrase) {

    char* tempPhrase;
    int phraseLength = strlen(phrase);
    tempPhrase = (char*)malloc(phraseLength);
    strcpy(tempPhrase, phrase);

    for (int i = 0; i < phraseLength; i++) {
        tempPhrase[i] = toupper(tempPhrase[i]);
    }

    return tempPhrase;
}

void AddSystemMessage(char message[MAX_STRING_SIZE]) {
    strcpy(system_message3, system_message2);
    strcpy(system_message2, system_message);
    strcpy(system_message, message);
}

void GetInput(int *letterCount, char *phraseBuffer) {
    int key = GetCharPressed();
    while (key > 0) {
        if ((key >= 32) && (key <= 125) && (*letterCount < MAX_STRING_SIZE - 1)) {
            phraseBuffer[*letterCount] = (char)key;
            phraseBuffer[(*letterCount) +1] = '\0';
            (*letterCount)++;
        }
        key = GetCharPressed();  // Check next character in the queue
    }
}


void ProcessInputForPhrase(char phraseBuffer[MAX_STRING_SIZE], int *letterCount, bool *is_setting_phrase, bool *is_receiving_phrase, int *framesCounter, bool *mouseOnText, int client_sock, struct Player *player, bool isPlayer1) {
    SetMouseCursor(MOUSE_CURSOR_IBEAM);
    GetInput(letterCount, phraseBuffer);

    if (IsKeyPressed(KEY_BACKSPACE)) {
        (*letterCount)--;
        if (*letterCount < 0) *letterCount = 0;
        phraseBuffer[*letterCount] = '\0';
    }

    if (IsKeyPressed(KEY_ENTER)) {
        strcpy(player->player_phrase, phraseBuffer);
        char new_message[MAX_STRING_SIZE] = DISPLAY_PHRASE;
        strcat(new_message, CapitalizePhrase(player->player_phrase));
        AddSystemMessage(new_message);
        while((*letterCount) > 0) {
            (*letterCount)--;
            if (*letterCount < 0) *letterCount = 0;
            phraseBuffer[*letterCount] = '\0';
        }
        *is_setting_phrase = false;
        *is_receiving_phrase = true;
        *framesCounter = 0;
        *mouseOnText = false;
        if (isPlayer1) 
            AddSystemMessage(WAITING_FOR_PHRASE);
        else
            AddSystemMessage(OPPONENTS_TURN);
        SetPhrase(player, client_sock);
    }
}

void ProcessInputForLetter(char phraseBuffer[MAX_STRING_SIZE], int *letterCount, int *framesCounter, bool *mouseOnText, bool *isGuessing, int client_sock, struct Player *player) {
    SetMouseCursor(MOUSE_CURSOR_IBEAM);
    if(*letterCount != 1)
        GetInput(letterCount, phraseBuffer);

    
    if (IsKeyPressed(KEY_BACKSPACE)) {
        (*letterCount)--;
        if (*letterCount < 0) *letterCount = 0;
        phraseBuffer[*letterCount] = '\0';
    }

    if (IsKeyPressed(KEY_ENTER)) {
        char new_message[MAX_STRING_SIZE] = PRINT_LETTER;
        strcat(new_message, phraseBuffer);
        AddSystemMessage(new_message);
        *framesCounter = 0;
        *mouseOnText = false;

        if (isLetterPressed(player, phraseBuffer[0])) {
            AddSystemMessage(ALREADY_GUESSED);
            AddSystemMessage(GUESS_PHRASE);
            return;
        }

        SetProgress(player, phraseBuffer[0], client_sock, isGuessing);
    }

}


void ToggleFlags(bool* flag1, bool* flag2) {
    *flag1 = false;
    *flag2 = true;
}