#include <ncurses.h>
#include "draw_box.h"

void DrawBox(int width, int height) {
    // Get the current cursor position
    int row, col;
    getyx(stdscr, row, col);

    // Draw the top and bottom edges
    for (int i = 0; i < width; i++) {
        mvprintw(row, col + i, "-");
        mvprintw(row + height - 1, col + i, "-");
    }

    // Draw the left and right edges
    for (int i = 0; i < height; i++) {
        mvprintw(row + i, col, "|");
        mvprintw(row + i, col + width - 1, "|");
    }
}