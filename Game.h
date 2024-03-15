#pragma once

#include<SDL.h>
#include <SDL_image.h>
#include<iostream>
#include <vector>
#include "Fruit.h"

const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 600;
static bool isEating = false;

class Game {
public:
	Game();
	~Game();
	void init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen);

	void handleEvent();
	void update();
	void render();
	void loadBackground(const char* path);
	void drawBackground();
	void clean();
	void cleanUp();//Giai phong bo nho cua player

	//Handle fruits
	void spawnFruits();

	bool running() { return isRunning; }

	static	SDL_Renderer* renderer;

private:
	int cnt = 0;
	bool isRunning;
	SDL_Window* window;
	SDL_Texture* backgroundTexture;

	std::vector<Fruit*> fruits;

	Uint32 startTime = 0;
};