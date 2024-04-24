#pragma once
#include "Game.h"

class Explosion{
public:
    Explosion(int x, int y);
    ~Explosion();
    void update();
    void render();
    bool isDestroyed() const;

private:
    std::vector<SDL_Texture*> explosionFrames;
    int posX, posY;
    int frame;
    int frameDelay;
    bool destroy = false;
};
