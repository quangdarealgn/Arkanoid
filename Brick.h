#pragma once

#include "SDL.h"
#include <vector>

struct Brick {
    SDL_Rect rect;
    bool      isDestroyed;
};

class BrickManager {
public:
    BrickManager();

    // Generates a fresh random layout each time calls
    void CreateRandomLayout();

    void Render(SDL_Renderer* renderer);
    bool CheckCollision(const SDL_Rect& ballRect);
    bool AllDestroyed() const;

    int  GetTotalBricks() const;
    int  GetRemainingBricks() const;

private:
    std::vector<Brick> bricks;

    // grid cell size = brick size
    static constexpr int BRICK_W = 75;
    static constexpr int BRICK_H = 25;

    // how many columns/rows fit
    int cols;
    int rows;
};
