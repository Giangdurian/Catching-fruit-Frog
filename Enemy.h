#pragma once
#include "Game.h"
int const ENEMY_SIZE = 40;
int const MAX_ENEMY_NUMBERS = 5;

class Enemy {
public:
    Enemy(const char* path, int x, int y);
    ~Enemy();

    void Update();
    void Render();
    bool checkCollision (int playerX, int playerY, int playerW, int playerH) const;

private:
    int xPos, yPos;
    int speedX, speedY; //Speed 
    SDL_Texture* enemyTexture;
    SDL_Rect destRect;
};