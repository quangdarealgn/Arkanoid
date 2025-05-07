#include "Ball.h"

Ball::Ball() {
    rect = { SCREEN_WIDTH / 2 - 10, SCREEN_HEIGHT / 2 - 10, 20, 20 };
    velX = 4;
    velY = -4;
    die = false;
}

void Ball::Update(const SDL_Rect& paddleRect) {
    rect.x += velX;
    rect.y += velY;

    // Wall collision
    if (rect.x <= 0 || rect.x >= SCREEN_WIDTH - rect.w) velX = -velX;
    if (rect.y <= 0)    velY = -velY;

    // Paddle collision
    if (CheckCollision(paddleRect)) {
        rect.y = paddleRect.y - rect.h;
        velY  = -velY;
    }

    // Die
    if (rect.y > paddleRect.y) {
        die = true;
    }
}

bool Ball::CheckCollision(const SDL_Rect& other) {
    return SDL_HasIntersection(&rect, &other);
}

void Ball::Render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, RED.r, RED.g, RED.b, RED.a);
    SDL_RenderFillRect(renderer, &rect);
}
