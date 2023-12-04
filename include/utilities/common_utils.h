#include "raylib.h"
#include "int_values.h"
#include "string_values.h"
#include "player.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

extern char system_message[MAX_STRING_SIZE];
extern char system_message2[MAX_STRING_SIZE];
extern char system_message3[MAX_STRING_SIZE];

bool ValidateArgs(char *file, int params_expected, int params_received);
void DieWithError(char *error_message);
char* EncryptPhrase(char* phrase);
char* CapitalizePhrase(char* phrase);
void AddSystemMessage(char message[MAX_STRING_SIZE]);
void ClearInputBox(char *phraseBuffer);
void GetInput(int *letterCount, char *phraseBuffer);
void ProcessInputForPhrase(char phraseBuffer[MAX_STRING_SIZE], int *letterCount, bool *is_setting_phrase, bool *is_receiving_phrase, int *framesCounter, bool *mouseOnText, int client_sock, struct Player *player);
void ToggleFlags(bool* flag1, bool* flag2);
void DrawTextCenter(const char *text, int y, int font, Color color);
#endif