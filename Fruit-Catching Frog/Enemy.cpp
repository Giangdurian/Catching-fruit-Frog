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
		speedX = rand() % MAX_SPEED - 3;
		speedY = rand() % MAX_SPEED - 3;
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
		do {
			speedX = -(speedX + rand() % 7 - 3);
		} while (speedX < -6 || speedX > 6 || (speedX < 2 && speedX > -2));

		if (xPos <= 0) {
			xPos = 0;
		}
		else {
			xPos = WINDOW_WIDTH - destRect.w;
		}
	}

	if (yPos <= 0 || yPos >= WINDOW_HEIGHT - destRect.h)
	{
		do {
			speedY = -(speedY + rand() % 7 - 3);
		} while (speedY < -6 || speedY > 6 || (speedY < 2 && speedY > -2));


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