#include "TextureManager.h"
#include "Fruit.h"
#include <stdlib.h>
#include <ctime>

Fruit::Fruit(const char* texturePath, int x, int y) {

	fruitTexture = TextureManager::LoadTexture(texturePath);
	is_HP = false;
	is_Shield = false;
	

	xPos = x;
	yPos = y;
	destRect.w = FRUIT_SIZE;
	destRect.h = FRUIT_SIZE;
	speed = 1 + rand() % 4;
}

Fruit::~Fruit()
{}

void Fruit::Update()
{
	//Update position of fruit based on its speed
	yPos += speed;

	destRect.x = xPos;
	destRect.y = yPos;
	//Check if fruits are out of window, destroy it;
	if (yPos > WINDOW_HEIGHT)
	{
		//SDL_DestroyTexture(fruitTexture);
		return;
	}

}

void Fruit::Render()
{
	SDL_RenderCopy(Game::renderer, fruitTexture, NULL, &destRect);
}

bool Fruit::checkCollision(int playerX, int playerY, int playerW, int playerH) const {
	return xPos < playerX + playerW - 10 &&
		xPos > playerX - destRect.w + 10 &&
		yPos < playerY + playerH - 2 * playerH / 5 &&
		yPos > playerY - destRect.h + 10;
}