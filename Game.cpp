#include "Game.h"

Game::Game()
  : window(nullptr)
  , renderer(nullptr)
  , isRunning(false)
  , state(START_SCREEN)
  , ballLaunched(false)
  , lives(3)
  , score(0)
  , totalBricks(0)
  , fontLogo(nullptr)
  , fontMenu(nullptr)
{
    SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
    TTF_Init();
    std::srand((unsigned)std::time(nullptr));

    window   = SDL_CreateWindow("Arkanoid", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    fontLogo = TTF_OpenFont("assets/fonts/arcade.ttf", 72);
    fontMenu = TTF_OpenFont("assets/fonts/arcade.ttf", 28);

    instructions = {
        "Use A / D to move paddle",
        "Press W to launch the ball",
        "Break all the bricks to win!",
        "Press ESC to return"
    };

    audioManager.LoadAudio();
    audioManager.PlayMenuMusic();

    isRunning = true;
}

Game::~Game() {
    TTF_CloseFont(fontLogo);
    TTF_CloseFont(fontMenu);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::Run() {
    Uint32 fs, ft;
    while (isRunning) {
        fs = SDL_GetTicks();
        HandleEvents();
        if (state == PLAYING) Update();
        Render();
        ft = SDL_GetTicks() - fs;
        if (ft < FRAME_DELAY) SDL_Delay(FRAME_DELAY - ft);
    }
}

void Game::HandleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            isRunning = false;
        }
        else if (e.type == SDL_KEYDOWN) {
            switch (state) {
                case START_SCREEN:
                    HandleMenuInput(e.key.keysym.sym);
                    break;
                case INSTRUCTIONS:
                    HandleInstructionsInput(e.key.keysym.sym);
                    break;
                case PAUSED:
                    HandlePauseInput(e.key.keysym.sym);
                    break;
                case GAME_OVER_SCREEN:
                    HandleGameOverInput(e.key.keysym.sym);
                    break;
                case WIN_SCREEN:
                    HandleWinInput(e.key.keysym.sym);
                    break;
                case PLAYING:
                    if (e.key.keysym.sym == SDLK_ESCAPE) {
                        state = PAUSED;
                        audioManager.PauseMusic();
                    }
                    else if (!ballLaunched && e.key.keysym.sym == SDLK_w) {
                        LaunchBall();
                    }
                    break;
            }
        }
    }
    if (state == PLAYING) {
        paddle.HandleInput();
    }
}

void Game::HandleMenuInput(SDL_Keycode key) {
    if (key == SDLK_RETURN) {
        lives        = 3;
        score        = 0;
        bricks       = BrickManager();
        totalBricks  = bricks.GetTotalBricks() * 10;
        ballLaunched = false;
        audioManager.PlayGameMusic();
        state = PLAYING;
    }
    else if (key == SDLK_ESCAPE) {
        isRunning = false;
    }
    else if (key == SDLK_i) {
        state = INSTRUCTIONS;
    }
}

void Game::HandleInstructionsInput(SDL_Keycode key) {
    if (key == SDLK_ESCAPE) {
        state = START_SCREEN;
    }
}

void Game::HandlePauseInput(SDL_Keycode key) {
    if (key == SDLK_ESCAPE) {
        state = PLAYING;
        audioManager.ResumeMusic();
    }
    else if (key == SDLK_RETURN) {
        audioManager.StopMusic();
        audioManager.PlayMenuMusic();
        state = START_SCREEN;
    }
}

void Game::HandleGameOverInput(SDL_Keycode key) {
    if (key == SDLK_RETURN) {
        lives        = 3;
        score        = 0;
        bricks       = BrickManager();
        totalBricks  = bricks.GetTotalBricks() * 10;
        ballLaunched = false;
        audioManager.PlayGameMusic();
        state = PLAYING;
    }
    else if (key == SDLK_ESCAPE) {
        audioManager.StopMusic();
        audioManager.PlayMenuMusic();
        state = START_SCREEN;
    }
}

void Game::HandleWinInput(SDL_Keycode key) {
    if (key == SDLK_RETURN) {
        lives        = 3;
        score        = 0;
        bricks       = BrickManager();
        totalBricks  = bricks.GetTotalBricks() * 10;
        ballLaunched = false;
        audioManager.PlayGameMusic();
        state = PLAYING;
    }
    else if (key == SDLK_ESCAPE) {
        audioManager.StopMusic();
        audioManager.PlayMenuMusic();
        state = START_SCREEN;
    }
}

void Game::LaunchBall() {
    int vx = ((std::rand() % 2) ? 1 : -1) * 4;
    int vy = -4;
    ball.SetVelocity(vx, vy);
    ballLaunched = true;
}

void Game::Update() {
    paddle.Update();
    SDL_Rect pr = paddle.GetRect();

    if (!ballLaunched) {
        ball.SetPosition(pr.x + pr.w/2 - 10, pr.y - 20);
        return;
    }

    ball.Update(pr);

    if (bricks.CheckCollision(ball.GetRect())) {
        ball.BounceY();
        audioManager.PlayBrickBreak();
        score += 10;
    }

    if (ball.GetRect().y > SCREEN_HEIGHT) {
        audioManager.PlayDie();
        lives--;
        if (lives > 0) {
            ballLaunched = false;
            ball.SetVelocity(0,0);
        } else {
            audioManager.StopMusic();
            audioManager.PlayGameOver();
            state = GAME_OVER_SCREEN;
        }
        return;
    }

    if (bricks.AllDestroyed()) {
        audioManager.StopMusic();
        audioManager.PlayWin();
        state = WIN_SCREEN;
    }
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 0,0,0,255);
    SDL_RenderClear(renderer);

    switch (state) {
        case START_SCREEN:
            RenderStartScreen();
            break;
        case INSTRUCTIONS:
            RenderInstructions();
            break;
        case PAUSED:
            RenderPauseScreen();
            break;
        case GAME_OVER_SCREEN:
            RenderGameOverScreen();
            break;
        case WIN_SCREEN:
            RenderWinScreen();
            break;
        case PLAYING:
        {
            SDL_Color white{255,255,255,255};
            const int margin = 5;

            std::string scoreText = "Score: " + std::to_string(score) + "/" + std::to_string(totalBricks);
            SDL_Surface* scoreSurf = TTF_RenderText_Solid(fontMenu, scoreText.c_str(), white);
            SDL_Texture* scoreTex  = SDL_CreateTextureFromSurface(renderer, scoreSurf);
            int sw, sh;
            SDL_QueryTexture(scoreTex, nullptr, nullptr, &sw, &sh);
            SDL_Rect scoreRect{ margin, SCREEN_HEIGHT - sh - margin, sw, sh };
            SDL_RenderCopy(renderer, scoreTex, nullptr, &scoreRect);

            std::string livesText = "Lives: " + std::to_string(lives) + "/3";
            SDL_Surface* lifeSurf = TTF_RenderText_Solid(fontMenu, livesText.c_str(), white);
            SDL_Texture* lifeTex  = SDL_CreateTextureFromSurface(renderer, lifeSurf);
            int lw, lh;
            SDL_QueryTexture(lifeTex, nullptr, nullptr, &lw, &lh);
            SDL_Rect livesRect{ margin, scoreRect.y - lh - margin, lw, lh };
            SDL_RenderCopy(renderer, lifeTex, nullptr, &livesRect);

            SDL_FreeSurface(scoreSurf);
            SDL_DestroyTexture(scoreTex);
            SDL_FreeSurface(lifeSurf);
            SDL_DestroyTexture(lifeTex);

            paddle.Render(renderer);
            ball.Render(renderer);
            bricks.Render(renderer);
        }
        break;
    }

    SDL_RenderPresent(renderer);
}

void Game::RenderStartScreen() {
    SDL_Color white{255,255,255,255};
    SDL_Surface* s1 = TTF_RenderText_Solid(fontLogo, "ARKANOID", white);
    SDL_Texture* t1 = SDL_CreateTextureFromSurface(renderer, s1);
    int w,h; SDL_QueryTexture(t1, nullptr, nullptr, &w, &h);
    SDL_Rect r1{SCREEN_WIDTH/2 - w/2,100,w,h};
    SDL_RenderCopy(renderer, t1, nullptr, &r1);
    SDL_FreeSurface(s1); SDL_DestroyTexture(t1);

    auto draw = [&](const char* txt, int y){
        SDL_Surface* s = TTF_RenderText_Solid(fontMenu, txt, white);
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
        int tw,th; SDL_QueryTexture(t, nullptr, nullptr, &tw, &th);
        SDL_Rect r{SCREEN_WIDTH/2 - tw/2, y, tw, th};
        SDL_RenderCopy(renderer, t, nullptr, &r);
        SDL_FreeSurface(s); SDL_DestroyTexture(t);
    };
    draw("Press ENTER to Start", 300);
    draw("Press ESC to Quit",     350);
    draw("Press I for Instructions",400);
}

void Game::RenderInstructions() {
    SDL_Color white{255,255,255,255};
    SDL_Surface* s2 = TTF_RenderText_Solid(fontLogo, "INSTRUCTIONS", white);
    SDL_Texture* t2 = SDL_CreateTextureFromSurface(renderer, s2);
    int w,h; SDL_QueryTexture(t2, nullptr, nullptr, &w, &h);
    SDL_Rect r2{SCREEN_WIDTH/2 - w/2,80,w,h};
    SDL_RenderCopy(renderer, t2, nullptr, &r2);
    SDL_FreeSurface(s2); SDL_DestroyTexture(t2);

    int y = 200;
    for (auto& line : instructions) {
        SDL_Surface* s = TTF_RenderText_Solid(fontMenu, line.c_str(), white);
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
        int tw,th; SDL_QueryTexture(t, nullptr, nullptr, &tw, &th);
        SDL_Rect rr{SCREEN_WIDTH/2 - tw/2, y, tw, th};
        SDL_RenderCopy(renderer, t, nullptr, &rr);
        SDL_FreeSurface(s); SDL_DestroyTexture(t);
        y += th + 10;
    }
}

void Game::RenderPauseScreen() {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0,0,0,180);
    SDL_Rect ov{0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
    SDL_RenderFillRect(renderer, &ov);

    SDL_Color white{255,255,255,255};
    SDL_Surface* s3 = TTF_RenderText_Solid(fontLogo, "Paused", white);
    SDL_Texture* t3 = SDL_CreateTextureFromSurface(renderer, s3);
    int w,h; SDL_QueryTexture(t3, nullptr, nullptr, &w, &h);
    SDL_Rect r3{SCREEN_WIDTH/2 - w/2,150,w,h};
    SDL_RenderCopy(renderer, t3, nullptr, &r3);
    SDL_FreeSurface(s3); SDL_DestroyTexture(t3);

    auto draw2 = [&](const char* txt, int y){
        SDL_Surface* s = TTF_RenderText_Solid(fontMenu, txt, white);
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
        int tw,th; SDL_QueryTexture(t, nullptr, nullptr, &tw, &th);
        SDL_Rect rr{SCREEN_WIDTH/2 - tw/2, y, tw, th};
        SDL_RenderCopy(renderer, t, nullptr, &rr);
        SDL_FreeSurface(s); SDL_DestroyTexture(t);
    };
    draw2("Press ESC to resume", 300);
    draw2("Press ENTER to exit", 350);
}

void Game::RenderGameOverScreen() {
    SDL_Color white{255,255,255,255};
    SDL_Surface* s4 = TTF_RenderText_Solid(fontLogo, "Game Over!", white);
    SDL_Texture* t4 = SDL_CreateTextureFromSurface(renderer, s4);
    int w,h; SDL_QueryTexture(t4, nullptr, nullptr, &w, &h);
    SDL_Rect r4{SCREEN_WIDTH/2 - w/2,150,w,h};
    SDL_RenderCopy(renderer, t4, nullptr, &r4);
    SDL_FreeSurface(s4); SDL_DestroyTexture(t4);

    auto draw = [&](const char* txt, int y){
        SDL_Surface* s = TTF_RenderText_Solid(fontMenu, txt, white);
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
        int tw,th; SDL_QueryTexture(t, nullptr, nullptr, &tw, &th);
        SDL_Rect rr{SCREEN_WIDTH/2 - tw/2, y, tw, th};
        SDL_RenderCopy(renderer, t, nullptr, &rr);
        SDL_FreeSurface(s); SDL_DestroyTexture(t);
    };
    draw("Press ENTER to play again", SCREEN_HEIGHT/2 + 20);
    draw("Press ESC to return to menu", SCREEN_HEIGHT/2 + 60);
}

void Game::RenderWinScreen() {
    SDL_Color white{255,255,255,255};
    SDL_Surface* s1 = TTF_RenderText_Solid(fontLogo, "You win!", white);
    SDL_Texture* t1 = SDL_CreateTextureFromSurface(renderer, s1);
    int w,h; SDL_QueryTexture(t1, nullptr, nullptr, &w, &h);
    SDL_Rect r1{SCREEN_WIDTH/2 - w/2,150,w,h};
    SDL_RenderCopy(renderer, t1, nullptr, &r1);
    SDL_FreeSurface(s1); SDL_DestroyTexture(t1);

    auto draw = [&](const char* txt, int y){
        SDL_Surface* s = TTF_RenderText_Solid(fontMenu, txt, white);
        SDL_Texture* t = SDL_CreateTextureFromSurface(renderer, s);
        int tw,th; SDL_QueryTexture(t, nullptr, nullptr, &tw, &th);
        SDL_Rect rr{SCREEN_WIDTH/2 - tw/2, y, tw, th};
        SDL_RenderCopy(renderer, t, nullptr, &rr);
        SDL_FreeSurface(s); SDL_DestroyTexture(t);
    };
    draw("Press ENTER to play again", SCREEN_HEIGHT/2 + 20);
    draw("Press ESC to return to menu", SCREEN_HEIGHT/2 + 60);
}
