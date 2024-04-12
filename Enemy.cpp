#include "Enemy.h"
#include "TextureManager.h"
#include <stdlib.h>
#include <ctime>

Enemy::Enemy(const char* path, int x, int y, int MAX_SPEED)
{
	enemyTexture = TextureManager::LoadTexture(path);
	xPos = x;
	yPos = y;
	do {
		speedX = rand() % MAX_SPEED - 4;
		speedY = rand() % MAX_SPEED - 4;
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
	//Rotate the enemies
	angle -= 3;
	if (angle == -360)	angle += 360;

	destRect.x = xPos;
	destRect.y = yPos;
	destRect.w = ENEMY_SIZE;
	destRect.h = ENEMY_SIZE;
}

void Enemy::Render()
{
	SDL_RenderCopyEx(Game::renderer, enemyTexture, NULL, &destRect, angle, NULL, SDL_FLIP_NONE);
}

bool Enemy::checkCollision(int playerX, int playerY, int playerW, int playerH) const {
	return destRect.x + destRect.w > playerX + playerW / 5 && destRect.x < playerX + 3 * playerW / 4
			&& destRect.y + destRect.h > playerY && destRect.y < playerY + 3 * playerW / 4;
}
//return xPos < playerX + playerW / 2 &&
//	xPos > playerX - destRect.w &&
//	yPos < playerY + playerH / 2 &&
//	yPos > playerY - destRect.h;