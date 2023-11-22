#include <ncurses.h>
#include "special_chars.h"
#include <string.h>

void PrintHashedCharacter(char* str) {
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] == '*') {
            addch(ACS_DIAMOND);
        } else {
            printw("%c", str[i]);
        }
    }
    printw("\n");
    refresh();
}