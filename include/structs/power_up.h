#ifndef POWER_UP_H
#define POWER_UP_H

#include "player.h"
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "int_values.h"
#include "common_utils.h"

bool IsMarkedSpot(struct Player *player, char letter, bool isPlayer);

#endif