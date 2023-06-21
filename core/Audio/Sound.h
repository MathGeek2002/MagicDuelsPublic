#ifndef SOUND_H
#define SOUND_H

#include <SDL2/SDL_mixer.h>

class Sound
{
public:
    Mix_Chunk *chunk;
    int volume;

    Sound(const char *path, int volume);
    void play();
    ~Sound();
};

#endif