#include "Enemy.h"
#include "TextureManager.h"
#include <stdlib.h>
#include <ctime>

Enemy::Enemy(const char* path, int x, int y, int MaxV, int MinV)
{
	enemyTexture = TextureManager::LoadTexture(path);
	xPos = x;
	yPos = y;
	
	speedX = rand() % (MaxV - MinV + 1) + MinV;
	speedY = rand() % (MaxV - MinV + 1) + MinV;
	if (rand() % 2 == 0) {
		speedX *= -1;
	}
	if (rand() % 2 == 0) {
		speedY *= -1;
	}
	destRect.w = ENEMY_SIZE;
	destRect.h = ENEMY_SIZE;
}

Enemy::~Enemy()
{}

void Enemy::Update(int MaxV, int MinV)
{
	xPos += speedX;
	yPos += speedY;

	if (xPos <= 0 || xPos >= WINDOW_WIDTH - destRect.w)
	{
		int tmp = rand() % (MaxV - MinV + 1) + MinV;
		if (rand() % 2)	speedX = -tmp;
		else speedX = tmp;
		
		

		if (xPos <= 0) {
			xPos = 0;
		}
		else {
			xPos = WINDOW_WIDTH - destRect.w;
		}
	}

	if (yPos <= 0 || yPos >= WINDOW_HEIGHT - destRect.h)
	{
		//speedY = -speedY;
		int tmp = rand() % (MaxV - MinV + 1) + MinV;
		if (rand() % 2)	speedY = -tmp;
		else speedY = tmp;
		

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
	return xPos < playerX + playerW - 10 &&
		xPos > playerX - destRect.w + 10 &&
		yPos < playerY + playerH - 2 * playerH / 5 &&
		yPos > playerY - destRect.h + 10;
}