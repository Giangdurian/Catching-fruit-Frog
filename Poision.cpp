#include "TextureManager.h"
#include "Poision.h"
#include <stdlib.h>
#include <ctime>

Poision::Poision(const char* texturePath, int x, int y) {

	poisionTexture = TextureManager::LoadTexture(texturePath);

	xPos = x;
	yPos = y;
	destRect.w = POISION_WIDTH;
	destRect.h = POISION_LENGTH;
	speed = 1 + rand() % 4;
}

Poision::~Poision()
{}

void Poision::Update()
{
	yPos += speed;

	destRect.x = xPos;
	destRect.y = yPos;
	if (yPos > WINDOW_HEIGHT)
	{
		return;
	}
}

void Poision::Render()
{
	SDL_RenderCopy(Game::renderer, poisionTexture, NULL, &destRect);
}

bool Poision::checkCollision(int playerX, int playerY, int playerW, int playerH) const {
	return xPos < playerX + playerW &&
		xPos > playerX - destRect.w &&
		yPos < playerY + playerH &&
		yPos > playerY - destRect.h;
}