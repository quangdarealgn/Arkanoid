#include "Paddle.h"

Paddle::Paddle() {
    rect = { SCREEN_WIDTH / 2 - 50, SCREEN_HEIGHT - 30, 100, 15 };
    speed = 8;
    keystate = SDL_GetKeyboardState(NULL);  // Initialize keyboard state
}

void Paddle::HandleInput() {
    if (keystate[SDL_SCANCODE_A]) rect.x -= speed;  // A key = left
    if (keystate[SDL_SCANCODE_D]) rect.x += speed;  // D key = right
}

void Paddle::Update() {
    if (rect.x < 0) rect.x = 0;
    if (rect.x > SCREEN_WIDTH - rect.w) rect.x = SCREEN_WIDTH - rect.w;
}

void Paddle::Render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, WHITE.r, WHITE.g, WHITE.b, WHITE.a);
    SDL_RenderFillRect(renderer, &rect);
}
