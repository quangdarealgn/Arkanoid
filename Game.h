#pragma once
#include "SDL.h"
#include "SDL_ttf.h"
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include "AudioManager.h"
#include "Paddle.h"
#include "Ball.h"
#include "Brick.h"
#include "Constants.h"

enum GameState {
    START_SCREEN,
    INSTRUCTIONS,
    PLAYING,
    PAUSED,
    GAME_OVER_SCREEN,
    WIN_SCREEN
};

class Game {
public:
    Game();
    ~Game();
    void Run();

private:
    SDL_Window*   window;
    SDL_Renderer* renderer;
    bool          isRunning;

    AudioManager  audioManager;
    Paddle        paddle;
    Ball          ball;
    BrickManager  bricks;

    GameState                state;
    bool                     ballLaunched;
    int                      lives;
    int                      score;
    int                      totalBricks;

    TTF_Font*                fontLogo;
    TTF_Font*                fontMenu;
    std::vector<std::string> instructions;

    void HandleEvents();
    void Update();
    void Render();

    void HandleMenuInput(SDL_Keycode key);
    void HandleInstructionsInput(SDL_Keycode key);
    void HandlePauseInput(SDL_Keycode key);
    void HandleGameOverInput(SDL_Keycode key);
    void HandleWinInput(SDL_Keycode key);

    void LaunchBall();

    void RenderStartScreen();
    void RenderInstructions();
    void RenderPauseScreen();
    void RenderGameOverScreen();
    void RenderWinScreen();
};
