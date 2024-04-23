#include "Game.h"

Game* game = nullptr;
//reference: Lets make game(youtube)
bool stop = false;
bool stop2 = false;

Mix_Chunk* playMusic(const char* path) {
    Mix_Chunk* music = Mix_LoadWAV(path);
    if (music == nullptr) {
        std::cerr << "Failed to load music: " << Mix_GetError() << std::endl;
        SDL_Quit();
    }
    return music;
}

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
    /*Mix_Chunk* background_music = playMusic("sound/background_music.mp3");
    Mix_Chunk* game_play_music = playMusic("sound/game_play_music.mp3");*/
    while (game->running()) {
        frameStart = SDL_GetTicks();
        game->handleEvent();//Handle the input from player's action: mouse, keyboard,...
        if (game->playing()) {
            /*if (!stop) {
                Mix_HaltChannel(2);
                stop = true;
                stop2 = false;
            }
            Mix_PlayChannel(2, game_play_music, 1);*/
            game->update();
        }
        /*else {
            if (!stop2) {
                Mix_HaltChannel(2);
                stop2 = true;
                stop = false;
            }
            Mix_PlayChannel(2, background_music, -1);
        }*/
        game->render();
        frameTime = SDL_GetTicks() - frameStart;
        if (FrameDelay > frameTime) {
            SDL_Delay(FrameDelay - frameTime);
        }
    }
    Mix_FreeChunk(background_music);
    Mix_FreeChunk(game_play_music);
    Mix_CloseAudio();
    game->clean();
    return 0;
}