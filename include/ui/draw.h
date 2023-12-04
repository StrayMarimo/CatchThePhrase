#include "raylib.h"
#include "int_values.h"
#include "stdio.h"

#ifndef DRAW_H
#define DRAW_H

void DrawGameLayout();
void DrawLettersPressed(char *letters_pressed, int offset);
void DrawHealthBar(int currentHealth, int x, int y);
void DrawTopic(char *topic);
void DrawInputBox(char phraseBuffer[MAX_STRING_SIZE], Rectangle textBox, bool mouseOnText, int letterCount, int framesCounter);

#endif