#ifndef AUDIO_H
#define AUDIO_H

#include <SDL2/SDL_mixer.h>
#include "Sound.h"
#include "Music.h"

class Audio
{
public:
    static int masterVolume;

private:
    static Music *currentMusic;

public:
    static void Init();
    static void playSound(Sound *);
    static void playMusic(Music *, bool = false);
    static void setMasterVolume(int);
    static void stopMusic();
    static void Close();
};

#endif