#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "Fruit.h"
#include "Enemy.h"
#include <chrono>
#include <ctime>

const int WINDOW_HEIGHT = 720;
const int WINDOW_WIDTH = 720;
static bool isEating = false;
static bool isPlaying = false;

class Enemy;

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

	bool running() { return isRunning; }

	void clean();
	void cleanUp();//Giai phong bo nho cua player

	//Handle fruits
	void spawnFruits();
	void spawnEnemies();

	static	SDL_Renderer* renderer;

private:
	int cnt = 0;
	bool isRunning;
	SDL_Window* window;
	SDL_Texture* backgroundTexture;

	std::vector<Enemy*> enemies;
	std::vector<Fruit*> fruits;
	

	Uint32 startTime = 0;

	int number_of_enemy = 0;
	std::chrono::steady_clock::time_point lastEnemySpawnTime;
};