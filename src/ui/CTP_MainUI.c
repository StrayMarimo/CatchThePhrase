#include <ncurses.h>

int main() {
    WINDOW *mainWindow, *player1, *player2, *chat_relay, *powerups, *turnIndicator, *topic;
    int ch, i = 0;

    initscr(); // initialize Ncurses
    mainWindow = newwin(49, 209, 1, 1); // create a new window
    box(mainWindow, 0, 0); // sets default borders for the window

    int start_x = (getmaxx(mainWindow) - 23) / 2; // Center the text
    mvwprintw(mainWindow, 0, start_x, "    Catch the Phrase    ");

    wrefresh(mainWindow); // update the terminal screen
    i = 0;
    noecho(); // disable echoing of characters on the screen
    keypad(mainWindow, TRUE); // enable keyboard input for the window.
    curs_set(0); // hide the default screen cursor.

    int subwin_height = 20;
    int subwin_width = 95;
    int start_y = 4;
    int start_x1 = (getmaxx(mainWindow) - subwin_width * 2 - 5) / 2;
    int start_x2 = start_x1 + subwin_width + 5;

    player1 = subwin(mainWindow, subwin_height, subwin_width, start_y, start_x1);
    box(player1, 0, 0);
    int player1_title = (getmaxx(player1) - 16) / 2;
    mvwprintw(player1, 0, player1_title, "    Player 1    ");

    int alphabet_center1A = (getmaxx(player1) - 61) / 2;
    mvwprintw(player1, 12, alphabet_center1A, "    A   B   C   D   E   F   G   H   I   J   K   L   M   N   ");

    int alphabet_center2A = (getmaxx(player1) - 53) / 2;
    mvwprintw(player1, 15, alphabet_center2A, "    O   P   Q   R   S   T   U   V   W   X   Y   Z   ");
    wrefresh(player1);

    player2 = subwin(mainWindow, subwin_height, subwin_width, start_y, start_x2);
    box(player2, 0, 0);
    int player2_title = (getmaxx(player2) - 16) / 2;
    mvwprintw(player2, 0, player2_title, "    Player 2    ");

    int alphabet_center1B = (getmaxx(player2) - 61) / 2;
    mvwprintw(player2, 12, alphabet_center1B, "    A   B   C   D   E   F   G   H   I   J   K   L   M   N   ");

    int alphabet_center2B = (getmaxx(player1) - 53) / 2;
    mvwprintw(player2, 15, alphabet_center2B, "    O   P   Q   R   S   T   U   V   W   X   Y   Z   ");
    wrefresh(player2);

    int start_y_below = start_y + subwin_height + 2;

    chat_relay = subwin(mainWindow, 17, 160, start_y_below, start_x1);
    box(chat_relay, 0, 0);
    wrefresh(chat_relay);

    int start_x4 = start_x2 + 65;

    powerups = subwin(mainWindow, 10, 30, start_y_below, start_x4);
    box(powerups, 0, 0);
    int start_sub4x = (getmaxx(powerups) - 15) / 2;
    mvwprintw(powerups, 0, start_sub4x, "    Powerups   ");
    wrefresh(powerups);

    int start_y_below_powerups = start_y_below + 12;
    turnIndicator = subwin(mainWindow, 5, 30, start_y_below_powerups, start_x4);
    box(turnIndicator, 0, 0);
    int word_position = (getmaxx(turnIndicator) - 15) / 2;
    mvwprintw(turnIndicator, 2, word_position, "   YOUR TURN   ");
    wrefresh(turnIndicator);

    int start_y_below_chat_relay = start_y_below + 18; 
    topic = subwin(mainWindow, 3, 195, start_y_below_chat_relay, start_x1);
    box(topic, 0, 0);
    mvwprintw(topic, 1, 1, "   Topic: Art");
    wrefresh(topic);

    // get the input
    while ((ch = wgetch(mainWindow)) != 'q') {
    }

    delwin(mainWindow);
    endwin();
    return 0;
}