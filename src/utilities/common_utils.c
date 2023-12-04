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

void ClearInputBox(char *phraseBuffer) {
    for (int i = 0; i < MAX_STRING_SIZE; i++) {
        phraseBuffer[i] = '\0';
    }
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

// void PrintLine(const char *format, ...) {
//     va_list args;
//     va_start(args, format);
//     vw_printw(stdscr, format, args);
//     va_end(args);
//     refresh();
// }

// void PrintSysMessage(int line, const char *format, ...) {
//     int row = 42, col = 4;
//     row = row - line;
    
//     move(row, col);
//     va_list args;
//     va_start(args, format);

//     for (int i = 0; i < 70; i++){
//         printw(" ");
//         refresh();
//     }

//     move(row, col);
//     printw("sys msg > ");
//     refresh();
//     vw_printw(stdscr, format, args);
//     va_end(args);
//     refresh();
// }


// void PrintFile(const char* filename) {
//     FILE* file = fopen(filename, "r");
//     if (file == NULL) {
//         PrintLine("Failed to open file: %s\n", filename);
//         return;
//     }
//     int col = 2, row = 2;
//     char line[256];
//     while (fgets(line, sizeof(line), file)) {
//         line[strcspn(line, "\n")] = '\0';
//         move(row, col);
//         PrintLine("%s", line);
//         row++;
//     }

//     fclose(file);

//     DrawBox(90, 12);
//     getyx(stdscr, row, col);
//     row -= 11;
//     col++;
//     move(row, col);
//     DrawBox(30, 12); 

// }