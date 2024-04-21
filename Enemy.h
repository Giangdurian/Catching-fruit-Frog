#pragma once
#include "Game.h"
int const ENEMY_SIZE = 40;

class Enemy {
public:
    Enemy(const char* path, int x, int y, int MAX_SPEED);
    ~Enemy();

    void Update();
    void Render();
    bool checkCollision(int playerX, int playerY, int playerW, int playerH) const;

    int angle = 0;

private:
    int xPos, yPos;
    int speedX, speedY; //Speed 
    SDL_Texture* enemyTexture;
    SDL_Rect destRect;
};