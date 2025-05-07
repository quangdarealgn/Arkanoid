#include "Brick.h"
#include "Constants.h"
#include <cstdlib>
#include <algorithm>

BrickManager::BrickManager() {
    // compute how many cells fit
    cols = SCREEN_WIDTH  / BRICK_W;         // e.g: 800/75 = 10
    rows = (SCREEN_HEIGHT/2) / BRICK_H;     // top half only, e.g: 300/25 = 12
    CreateRandomLayout();
}

void BrickManager::CreateRandomLayout() {
    bricks.clear();

    // build list of all possible grid‐cells
    std::vector<int> cells(cols * rows);
    for (int i = 0; i < cols*rows; ++i) cells[i] = i;

    // shuffle in order to get random cell order
    std::random_shuffle(cells.begin(), cells.end());

    // pick first N cells for bricks
    int N = cols * 5;  // e.g. 10 cols × 5 rows = 50 bricks
    for (int i = 0; i < N && i < (int)cells.size(); ++i) {
        int idx = cells[i];
        int c = idx % cols;
        int r = idx / cols;
        Brick b;
        b.rect.w = BRICK_W - 2;
        b.rect.h = BRICK_H - 2;
        b.rect.x = c * BRICK_W + 1;
        b.rect.y = r * BRICK_H + 1;
        b.isDestroyed = false;
        bricks.push_back(b);
    }
}

bool BrickManager::CheckCollision(const SDL_Rect& ballRect) {
    for (auto& b : bricks) {
        if (!b.isDestroyed && SDL_HasIntersection(&ballRect, &b.rect)) {
            b.isDestroyed = true;
            return true;
        }
    }
    return false;
}

void BrickManager::Render(SDL_Renderer* renderer) {
    for (auto& b : bricks) {
        if (!b.isDestroyed) {
            SDL_SetRenderDrawColor(renderer, BLUE.r, BLUE.g, BLUE.b, BLUE.a);
            SDL_RenderFillRect(renderer, &b.rect);
        }
    }
}

bool BrickManager::AllDestroyed() const {
    for (auto& b : bricks)
        if (!b.isDestroyed) return false;
    return true;
}

int BrickManager::GetTotalBricks() const {
    return static_cast<int>(bricks.size());
}

int BrickManager::GetRemainingBricks() const {
    int cnt = 0;
    for (auto& b : bricks)
        if (!b.isDestroyed) ++cnt;
    return cnt;
}
