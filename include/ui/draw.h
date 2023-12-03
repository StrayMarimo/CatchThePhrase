#include "raylib.h"
#include "int_values.h"

#ifndef DRAW_H
#define DRAW_H

void DrawGameLayout();
void DrawLettersPressed(char *letters_pressed);
void DrawHealthBar(int currentHealth);
void DrawTopic(char *topic);

#endif