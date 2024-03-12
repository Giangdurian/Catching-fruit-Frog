#pragma once
#include "Game.h"

class GameObject {
public:
	GameObject(const char* texturesheet, int x, int y);
	~GameObject();

	void Update();
	void Render();

private:

	int xPos; //x and y coordinates of the top - left corner of the rectangle.
	int yPos;
	int speed;
	SDL_Texture* objTexture;
	SDL_Rect srcRect, destRect;
};