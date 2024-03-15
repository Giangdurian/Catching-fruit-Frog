#pragma once
#include "Game.h"
#include "Fruit.h"

int const fallSpeed = 20;

class GameObject {
public:
	GameObject(const char* texturesheet, int x, int y);
	~GameObject();
	bool checkCollision(const Fruit* fruit) const;
	void Update();
	void Render();
	void openMouth();
	void closeMouth();
private:

	int xPos; //x and y coordinates of the top - left corner of the rectangle.
	int yPos;
	int speed;

	int initialJumpYPos, yVelocity, maxJumpHeight;
	int const jumpForce = 20;
	SDL_Texture* objTexture;
	SDL_Rect destRect;


};