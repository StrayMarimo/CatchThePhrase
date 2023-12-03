#include "raylib.h"
#include "int_values.h"
#include "string_values.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#ifndef COMMON_UTILS_H
#define COMMON_UTILS_H

bool ValidateArgs(char *file, int params_expected, int params_received);
void DieWithError(char *error_message);
void DrawGameLayout();
void DrawTopic(char *topic);

// char* EncryptPhrase(char* phrase);
// char* CapitalizePhrase(char* phrase);
// void PrintLine(const char *format, ...);
// void PrintSysMessage(int lineIndex, const char *format, ...);
// void PrintFile(const char* filename);

void DrawTextCenter(const char *text, int y, int font, Color color);
#endif