#ifndef SOUND_MANAGER_H
#define SOUND_MANAGER_H
#include "raylib.h"


struct Audios
{
    Sound loading;
    Sound type;
    Sound powerup;
    Sound wrong;
    Sound game_over;
};

struct Audios LoadAudios();
void UnloadAudios(struct Audios *audios);

#endif