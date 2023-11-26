#include "common_utils.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

void ValidateArgs(char *file, int params_expected, int params_received) {
    if (params_expected != params_received) {
        if (params_expected == 2) 
            PrintLine("Usage: %s port_no\n", file);
        else if (params_expected == 3) {
            PrintLine("Usage: %s hostname port_no\n", file);
        DieWithError("Invalid number of arguments.");
        }
    }
}

void DieWithError(char *errorMessage) {
    PrintLine(errorMessage);
    exit(EXIT_FAILURE);
}

char* EncryptPhrase(char* phrase) {
    // Turns alphabet to asterisks.

    char* encryptedPhrase;
    int phraseLength = strlen(phrase);
    encryptedPhrase = (char*)malloc(phraseLength);

    strcpy(encryptedPhrase, phrase);

    for (int i = 0; i < phraseLength; i++) {
        if (isalpha(phrase[i])) {
            encryptedPhrase[i] = '*';
        }
    }

    return encryptedPhrase;
} 

char* CapitalizePhrase(char* phrase) {
    // returns a capitalized char*

    char* tempPhrase;
    int phraseLength = strlen(phrase);
    tempPhrase = (char*)malloc(phraseLength);

    // Copy string to temp variable
    strcpy(tempPhrase, phrase);

    for (int i = 0; i < phraseLength; i++) {
        tempPhrase[i] = toupper(tempPhrase[i]);
    }

    return tempPhrase;
}


void PrintLine(const char *format, ...) {
    va_list args;
    va_start(args, format);
    vw_printw(stdscr, format, args);
    va_end(args);
    refresh();
}


void PrintFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        PrintLine("Failed to open file: %s\n", filename);
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        PrintLine("%s", line);
    }

    fclose(file);

}