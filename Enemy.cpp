#include "Enemy.h"
#include "TextureManager.h"
#include <stdlib.h>
#include <ctime>

Enemy::Enemy(const char* path, int x, int y) 
{
	enemyTexture = TextureManager::LoadTexture(path);
	xPos = x;
	yPos = y;
	do {
		speedX = rand() % 7 - 3;
		speedY = rand() % 7 - 3;
	} while ((speedX <= 2 && speedX >= -2) || (speedY <= 2 && speedY >= -2));
	
	destRect.w = ENEMY_SIZE;
	destRect.h = ENEMY_SIZE;
}

Enemy::~Enemy()
{}

void Enemy::Update()
{
	xPos += speedX;
	yPos += speedY;

	if (xPos <= 0 || xPos >= WINDOW_WIDTH - destRect.w)
	{
		speedX = -speedX;
		if (xPos <= 0) {
			xPos = 0;
		}
		else {
			xPos = WINDOW_WIDTH - destRect.w;
		}
	}
	if (yPos <= 0 || yPos >= WINDOW_HEIGHT - destRect.h)
	{
		speedY = -speedY;
		if (yPos <= 0) {
			yPos = 0;
		}
		else {
			yPos = WINDOW_HEIGHT - destRect.h;
		}
	}
	destRect.x = xPos;
	destRect.y = yPos;
	destRect.w = ENEMY_SIZE;
	destRect.h = ENEMY_SIZE;
}

void Enemy::Render()
{
	SDL_RenderCopy(Game::renderer, enemyTexture, NULL, &destRect);
}

bool Enemy::checkCollision(int playerX, int playerY, int playerW, int playerH) const {
	return xPos < playerX + playerW &&
		xPos + destRect.w > playerX &&
		yPos < playerY + playerH &&
		yPos + destRect.h > playerY;
}

