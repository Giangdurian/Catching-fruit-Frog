#include "Game.h"

Game* game = nullptr;

int main(int argc, char* args[])
{
    srand(time(NULL));
    const int FPS = 60;
    const int FrameDelay = 1000 / FPS;

    Uint32 frameStart;
    int frameTime;

    game = new Game();
    // Initialise the window screen
    game->init("Fruit-Catching Frog", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_HEIGHT, WINDOW_WIDTH, false); 
    while (game->running()) {
        frameStart = SDL_GetTicks();
        game->handleEvent();//Handle the input from player's action: mouse, keyboard,...
        if (game->playing()) {
            game->update();
        }
        game->render();
        frameTime = SDL_GetTicks() - frameStart;
        if (FrameDelay > frameTime) {
            SDL_Delay(FrameDelay - frameTime);
        }
    }
    Mix_CloseAudio();
    game->clean();
    return 0;
}