#include "TextureManager.h"
#include "Fruit.h"
#include <stdlib.h>
#include <ctime>

Fruit::Fruit(const char* texturePath, int x, int y) {

	fruitTexture = TextureManager::LoadTexture(texturePath);

	xPos = x;
	yPos = y;
	speed = 1 + rand() % 4;
}

Fruit::~Fruit() 
{}

void Fruit::Update()
{
    //Update position of fruit based on its speed
    yPos += speed;
	//height and width(size) of the pictures
	//srcRect.h = 40;
	//srcRect.w = 40;
	//the position of the top-left conner you want to cut, generally (0,0);
	//srcRect.x = 0;
	//srcRect.y = 0;

	destRect.x = xPos;
	destRect.y = yPos;
	destRect.w = FRUIT_SIZE;
	destRect.h = FRUIT_SIZE;
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
