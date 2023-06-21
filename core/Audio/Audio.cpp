#include "Audio.h"
#include <SDL2/SDL_mixer.h>

int Audio::masterVolume = 128;
Music *Audio::currentMusic = nullptr;

void Audio::Init()
{
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
};

void Audio::playSound(Sound *sound)
{
    Mix_PlayChannel(-1, sound->chunk, 0);
}

void Audio::playMusic(Music *music, bool loop)
{
    if(currentMusic != nullptr)
        delete currentMusic;
        
    currentMusic = music;
    int loops = loop ? (-1) : 0;
    Mix_PlayMusic(music->data, loops);
}

// Set master volume. Value range is 0-128.
void Audio::setMasterVolume(int volume)
{
    masterVolume = volume;
    Mix_Volume(-1, volume);

    int musicVolume = volume;
    if (currentMusic != nullptr)
    {
        musicVolume = (currentMusic->volume * volume) / 128;
    }
    Mix_VolumeMusic(musicVolume);
}

void Audio::stopMusic()
{
    Mix_HaltMusic();
}

void Audio::Close()
{
    Mix_CloseAudio();

    if(currentMusic != nullptr)
        delete currentMusic;
}