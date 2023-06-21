#include "Sound.h"
#include "Audio.h"

Sound::Sound(const char *path, int volume)
{
    Sound::volume = volume;
    Sound::chunk = Mix_LoadWAV(path);
    Mix_VolumeChunk(Sound::chunk, volume);
}

Sound::~Sound()
{
    Mix_FreeChunk(Sound::chunk);
}

void Sound::play()
{
    Audio::playSound(this);
}