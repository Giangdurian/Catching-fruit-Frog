#include "Explosion.h"
#include <string>


Explosion::Explosion(int x, int y) {
    posX = x;
    posY = y;
    frame = 0;
    frameDelay = 2;
    // Load 9 explosion images
    for (int i = 1; i <= 9; ++i) {
        std::string filename = "img2/explosion" + std::to_string(i) + ".png";
        explosionFrames.push_back(IMG_LoadTexture(Game::renderer, filename.c_str()));
    }
}

Explosion::~Explosion() {}

void Explosion::update() {
    if (frameDelay == 0) {
        if (frame < explosionFrames.size() - 1) {
            frame++;
        }
        else {
            // Destroy explosion when explosion finished
            destroy = true;
        }
        frameDelay = 2;
    }
    else {
        frameDelay--;
    }
}

void Explosion::render() {
    SDL_Rect destRect = { posX, posY, 120, 120 }; 
    SDL_RenderCopy(Game::renderer, explosionFrames[frame], NULL, &destRect);
}

bool Explosion::isDestroyed() const {
    return destroy;
}

