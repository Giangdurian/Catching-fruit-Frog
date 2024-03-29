#pragma once
#include "Game.h"

int const FRUIT_SIZE = 40;

class Fruit {
public:
    Fruit(const char* texturePath, int x, int y);
    ~Fruit();

    void Update();
    void Render();
    bool checkCollision(int playerX, int playerY, int playerW, int playerH) const;
    int getXPos() const { return xPos; }
    int getYPos() const { return yPos; }
private:
    int xPos, yPos;
    int speed;

    SDL_Texture* fruitTexture;
    SDL_Rect destRect;
};