#include "Music.h"
#include "Audio.h"

Music::Music(const char *path, int volume)
{
    Music::volume = volume;
    Music::data = Mix_LoadMUS(path);
    Mix_VolumeMusic(volume);
}

Music::~Music()
{
    Mix_FreeMusic(Music::data);
}

void Music::play(bool loop)
{
    Audio::playMusic(this, loop);
}