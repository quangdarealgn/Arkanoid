#pragma once

#include "SDL.h"
#include "Constants.h"

class Ball {
public:
    Ball();
    // flips the Y velocity
    void BounceY() {
        velY = -velY;
    }
    //launch logic
    void SetVelocity(int vx, int vy) {
         velX = vx; velY = vy;
    }
    void SetPosition(int x, int y)   {
         rect.x   = x;   rect.y   = y;
    }

    void Update(const SDL_Rect& paddleRect);
    void Render(SDL_Renderer* renderer);
    bool CheckCollision(const SDL_Rect& other);
    SDL_Rect GetRect() const {
         return rect;
    }
    bool IsDie() const  {
         return die;
    }

private:
    SDL_Rect rect;
    int velX, velY;
    bool die;
};
