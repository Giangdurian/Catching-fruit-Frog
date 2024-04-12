#pragma once
#include "Game.h"
#include "Fruit.h"
#include "Enemy.h"

int const fallSpeed = 20;
int const PLAYER_WIDTH = 84 * 3 / 4;
int const PLAYER_HEIGHT = 80 * 3 / 4;

class GameObject {
public:
	GameObject(const char* texturesheet, int x, int y);
	~GameObject();
	void Update();
	void Update2();
	void Render();
	void changeTexture(const char* path);
	bool isEating = false;

	int getXpos() const { return xPos; }
	int getYpos() const { return yPos; }
	int getWidth() const { return destRect.w; }
	int getHeight() const { return destRect.h; }

private:

	int xPos; //x and y coordinates of the top - left corner of the rectangle.
	int yPos;
	int speed;
	bool is_eating = false;

	int yVelocity, maxJumpHeight;
	int const jumpForce = 25;
	SDL_Texture* objTexture;
	SDL_Rect destRect;
};