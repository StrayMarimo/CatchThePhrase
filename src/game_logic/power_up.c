#include "power_up.h"

bool IsMarkedSpot(struct Player *player, char letter, bool isPlayer) {

    char phrase[MAX_STRING_SIZE];
    char progress[MAX_STRING_SIZE];

    if (isPlayer) {
        strcpy(phrase, player->opponent_phrase);
        strcpy(progress, player->progress);
    }
    else {
        strcpy(phrase, player->player_phrase);
        strcpy(progress, player->opponent_progress);
    }

    AddSystemMessage(phrase);
    for (int i = 0; i < strlen(phrase); i++) {
        if (toupper(letter) == phrase[i] && progress[i] == '^')
            return true;
    }

    return false;
}