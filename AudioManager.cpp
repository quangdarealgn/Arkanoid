#include "AudioManager.h"

AudioManager::AudioManager()
  : menuMusic(nullptr)
  , gameMusic(nullptr)
  , brickBreak(nullptr)
  , die(nullptr)
  , gameOver(nullptr)
  , win(nullptr)
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer init error: " << Mix_GetError() << "\n";
    }
}

AudioManager::~AudioManager() {
    StopMusic();
    Mix_FreeMusic(menuMusic);
    Mix_FreeMusic(gameMusic);
    Mix_FreeChunk(brickBreak);
    Mix_FreeChunk(gameOver);
    Mix_FreeChunk(die);
    Mix_FreeChunk(win);
    Mix_CloseAudio();
}

bool AudioManager::LoadAudio() {
    bool s = true;
    if (!LoadMusic("assets/music/menu.wav", &menuMusic))
    s = false;
    if (!LoadMusic("assets/music/background.wav", &gameMusic))
    s = false;
    if (!LoadSound("assets/sounds/brick.wav", &brickBreak))
    s = false;
    if (!LoadSound("assets/sounds/die.wav", &die))
    s = false;
    if (!LoadSound("assets/sounds/gameover.wav", &gameOver))
    s = false;
    if (!LoadSound("assets/sounds/win.wav", &win))
    s = false;
    return s;
}

bool AudioManager::LoadMusic(const char* path, Mix_Music** music) {
    *music = Mix_LoadMUS(path);
    if (!*music) {
        std::cerr << "Failed to load music " << path << ": " << Mix_GetError() << "\n";
        return false;
    }
    return true;
}

bool AudioManager::LoadSound(const char* path, Mix_Chunk** sound) {
    *sound = Mix_LoadWAV(path);
    if (!*sound) {
        std::cerr << "Failed to load SFX " << path << ": " << Mix_GetError() << "\n";
        return false;
    }
    return true;
}

void AudioManager::PlayMenuMusic() {
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(menuMusic, -1);
    }
}

void AudioManager::PlayGameMusic() {
    if (Mix_PlayingMusic()) Mix_HaltMusic();
    Mix_PlayMusic(gameMusic, -1);
}

void AudioManager::StopMusic() {
    Mix_HaltMusic();
}

void AudioManager::PauseMusic() {
    if (Mix_PlayingMusic() && !Mix_PausedMusic()) Mix_PauseMusic();
}

void AudioManager::ResumeMusic() {
    if (Mix_PausedMusic()) Mix_ResumeMusic();
}

void AudioManager::PlayWin() {
    Mix_PlayChannel(-1, win, 0);
}

void AudioManager::PlayBrickBreak() {
    Mix_PlayChannel(-1, brickBreak, 0);
}

void AudioManager::PlayDie() {
    Mix_PlayChannel(-1, die, 0);
}

void AudioManager::PlayGameOver(){
    Mix_PlayChannel(-1, gameOver, 0);
}
