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
	bool checkCollision(const Fruit* fruit) const;
	void Update();
	void Render();
	void changeTexture(const char* path);

	int getXpos() const { return xPos; }
	int getYpos() const { return yPos; }
	int getWidth() const { return destRect.w; }
	int getHeight() const { return destRect.h; }

private:

	int xPos; //x and y coordinates of the top - left corner of the rectangle.
	int yPos;
	int speed;

	int yVelocity, maxJumpHeight;
	int const jumpForce = 25;
	SDL_Texture* objTexture;
	SDL_Rect destRect;
};