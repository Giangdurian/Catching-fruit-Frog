#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <string>
#include "Fruit.h"
#include "Enemy.h"
#include "Poision.h"
#include "Explosion.h"
#include <chrono>
#include <ctime>

const int WINDOW_HEIGHT = 780;
const int WINDOW_WIDTH = 780;
static bool lose = false;
static int MAX_ENEMY_NUMBERS = 5;
static int MAX_SPEED = 7;
static Mix_Chunk* clickingSound;
static Mix_Chunk* eatingSound;
static Mix_Chunk* background_music;
static Mix_Chunk* game_play_music;
static Mix_Chunk* explosionSound;
static Mix_Chunk* healSound;
static Mix_Chunk* shieldSound;


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

	bool running() const { return isRunning; }
	bool playing() const { return isPlaying; }

	void clean();
	void cleanUp();//Giai phong bo nho cua player

	//Handle fruits
	void spawnFruits();
	void spawnEnemies();
	void spawnPoisions();
	void spawnHP();
	void spawnShield();
	void first_spawn();
	void reset_time();

	void end_game();
	static	SDL_Renderer* renderer;

private:
	bool isRunning;
	bool isPlaying;
	int number_of_player = 1;
	bool first_spawned = false;
	int mode = 1;
	bool difficulty_clicked = false;
	bool paused = false;
	bool start_clicked = false;
	bool tutorial_clicked = false;
	SDL_Window* window;
	SDL_Texture* backgroundTexture;


	std::vector<Enemy*> enemies;
	std::vector<Fruit*> fruits;
	std::vector<Poision*> poisions;



	Uint32 startTime = 0;
	Uint32 startTime2 = 0;

	int number_of_enemy = 0;
	std::chrono::steady_clock::time_point lastEnemy_SpawnTime;
	std::chrono::steady_clock::time_point lastFruits_SpawnTime;
	std::chrono::steady_clock::time_point lastPosions_SpawnTime;
	std::chrono::steady_clock::time_point lastHP_SpawnTime;
	std::chrono::steady_clock::time_point lastShield_SpawnTime;
	int protecting_time;
};