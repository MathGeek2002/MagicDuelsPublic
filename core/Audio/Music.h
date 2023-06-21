#ifndef MUSIC_H
#define MUSIC_H

#include <SDL2/SDL_mixer.h>

class Music
{
public:
    Mix_Music *data;
    int volume;

    Music(const char *path, int volume);
    void play(bool loop = false);
    ~Music();
};

#endif