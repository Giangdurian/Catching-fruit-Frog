#pragma once
#include "Game.h"
#include "Fruit.h"
#include "Enemy.h"

int const fallSpeed = 20;
int const PLAYER_WIDTH = 84  * 4 / 5;
int const PLAYER_HEIGHT = 80 * 4 / 5;

class GameObject {
public:
	GameObject(const char* texturesheet, int x, int y);
	~GameObject();
	void Update();
	void Update2();
	void Render();
	void changeTexture(const char* path);
	bool isEating = false;
	int hp;
	bool isDead = false;
	bool Protected;
	int bonusHP;
	std::chrono::steady_clock::time_point protecting_start_time;
	int getXpos() const { return xPos; }
	int getYpos() const { return yPos; }
	int getWidth() const { return destRect.w; }
	int getHeight() const { return destRect.h; }
	int score;

private:

	int xPos; //x and y coordinates of the top - left corner of the rectangle.
	int yPos;
	int speed;
	bool isJumping, isFalling;

	

	int yVelocity, maxJumpHeight;
	int const jumpForce = 23;
	SDL_Texture* objTexture;
	SDL_Rect destRect;
};