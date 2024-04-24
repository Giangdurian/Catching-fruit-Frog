#pragma once
#include "Game.h"

int const POISION_WIDTH = 40;
int const POISION_LENGTH = 55;

class Poision {
public:
    Poision(const char* texturePath, int x, int y);
    ~Poision();

    void Update();
    void Render();
    bool checkCollision(int playerX, int playerY, int playerW, int playerH) const;
    int getXPos() const { return xPos; }
    int getYPos() const { return yPos; }
private:
    int xPos, yPos;
    int speed;

    SDL_Texture* poisionTexture;
    SDL_Rect destRect;
};