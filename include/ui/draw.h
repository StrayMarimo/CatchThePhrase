#include "raylib.h"
#include "int_values.h"
#include "stdio.h"
#include "player.h"

#ifndef DRAW_H
#define DRAW_H

extern char system_message[MAX_STRING_SIZE];
extern char system_message2[MAX_STRING_SIZE];
extern char system_message3[MAX_STRING_SIZE];

void DrawGameLayout(struct Player *player);
void DrawLettersPressed(char *letters_pressed, int offset);
void DrawHealthBar(int currentHealth, int x, int y);
void DrawTopic(char *topic);
void DrawInputBox(char phraseBuffer[MAX_STRING_SIZE], Rectangle textBox, bool mouseOnText, int letterCount, int framesCounter);

#endif