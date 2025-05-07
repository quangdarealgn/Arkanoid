#pragma once

#include "SDL_mixer.h"
#include <iostream>

class AudioManager {
public:
    AudioManager();
    ~AudioManager();

    bool LoadAudio();

    void PlayMenuMusic();
    void PlayGameMusic();
    void StopMusic();

    void PauseMusic();
    void ResumeMusic();

    void PlayWin();
    void PlayDie();

    void PlayBrickBreak();
    void PlayGameOver();

private:
    Mix_Music* menuMusic;
    Mix_Music* gameMusic;
    Mix_Chunk* brickBreak;
    Mix_Chunk* gameOver;
    Mix_Chunk* die;
    Mix_Chunk* win;

    bool LoadMusic(const char* path, Mix_Music** music);
    bool LoadSound(const char* path, Mix_Chunk** sound);
};
