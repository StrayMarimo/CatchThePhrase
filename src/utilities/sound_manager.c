#include "sound_manager.h"



struct Audios LoadAudios() {
    struct Audios audio;
    audio.loading = LoadSound("assets/audio/target.ogg");
    audio.type = LoadSound("assets/audio/type.wav");
    audio.powerup = LoadSound("assets/audio/power_up.wav");
    audio.wrong = LoadSound("assets/audio/wrong.wav");
    audio.game_over = LoadSound("assets/audio/game_over.wav");

    return audio;
}

void UnloadAudios(struct Audios *audio) {
    UnloadSound(audio->loading);
    UnloadSound(audio->type);
    UnloadSound(audio->powerup);
    UnloadSound(audio->wrong);
    UnloadSound(audio->game_over);
}