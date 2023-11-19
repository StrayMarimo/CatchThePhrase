#include "common_utils.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

void ValidateArgs(char *file, int params_expected, int params_received) {
    if (params_expected != params_received) {
        if (params_expected == 2) 
            printf("Usage: %s port_no\n", file);
        else if (params_expected == 3) {
            printf("Usage: %s hostname port_no\n", file);
        DieWithError("Invalid number of arguments.");
        }
    }
}

void DieWithError(char *errorMessage) {
    perror(errorMessage);
    exit(EXIT_FAILURE);
}

char* EncryptPhrase(char* phrase, int phraseLength) {
    // Turns alphabet to asterisks.

    char* encryptedPhrase;
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