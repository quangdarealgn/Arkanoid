#pragma once
#include "SDL.h"
#include "Constants.h"

class Paddle {
public:
    Paddle();
    void HandleInput();
    void Update();
    void Render(SDL_Renderer* renderer);
    SDL_Rect GetRect() const { return rect; }

private:
    SDL_Rect rect;
    int speed;
    const Uint8* keystate;
};
