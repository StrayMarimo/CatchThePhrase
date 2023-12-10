#ifndef POWER_UP_H
#define POWER_UP_H

#include "player.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "int_values.h"
#include "common_utils.h"

bool IsMarkedSpot(struct Player *player, char letter, bool isPlayer);
void RevealNotPresentLetter(struct Player *player, int client_sock);
void ReceiveRevealNotPresentLetter(struct Player *player, int client_sock);
void RevealALetter(struct Player *player, int client_sock, bool *isGuessing, bool *isWaitingForGuess);
bool CheckThreeInARow(struct Player *player);
bool CheckandResetThreeInARow(struct Player *player);
void ReceiveRevealLetter(struct Player *player, int client_sock);
// void PowerUpTrigger(struct Player *player, int client_sock);
#endif