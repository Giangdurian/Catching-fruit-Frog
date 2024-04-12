#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include <random>

GameObject* player;
GameObject* Player2;
Uint32 eatingStartTime = 0;
Uint32 lose_Time = 0;
static bool loaded_menu = false;
static bool loaded_difficulty = false;
static bool paused = false;
static int mode = 1;


SDL_Renderer* Game::renderer = nullptr;

Game::Game() {}

Game::~Game() {}

void Game::loadBackground(const char* path) {
	SDL_Surface* surface = IMG_Load(path);
	if (surface == nullptr) {
		std::cerr << "Unable to load background image: " << SDL_GetError() << std::endl;
	}
	else {
		backgroundTexture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}
}

void Game::drawBackground() {
	SDL_RenderCopy(renderer, backgroundTexture, nullptr, nullptr);
}

void Game::init(const char* title, int xpos, int ypos, int width, int height, bool fullscreen)
{
	int flags = 0;
	if (fullscreen)
	{
		flags = SDL_WINDOW_FULLSCREEN;
	}
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
	{
		std::cout << "Subsystems Initialised..." << std::endl;

		window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
		if (window)
		{
			std::cout << "Window created!" << std::endl;
		}
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer)
		{
			SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			std::cout << "Renderer created!" << std::endl;
		}
		isRunning = true;
	}
	else
	{
		isRunning = false;
	}
	loadBackground("img/background_menu.png");
	//create a new object, in this case it's a player;
	player = new GameObject("img/frog1.png", WINDOW_WIDTH / 2 - PLAYER_WIDTH, WINDOW_HEIGHT - PLAYER_HEIGHT);
}

void Game::spawnFruits()
{	// Initialize path to fruits images
	std::vector<std::string> fruitPaths = {
		"img/melon.png",
		"img/grape.png",
		"img/strawberry.png",
		"img/cherry.png",
		"img/blueberry.png",
		"img/banana.png",
		"img/apple.png"
	};

	int num = 1 + rand() % 7;

	for (int i = 0; i < num; ++i)
	{
		int randomIndex = rand() % fruitPaths.size();
		std::string fruit_type = fruitPaths[randomIndex];
		// Add a new fruits and push_back into the fruits_list;
		int randomX = 40 + rand() % (WINDOW_WIDTH - 80);
		fruits.push_back(new Fruit(fruit_type.c_str(), randomX, 0));
	}
}

void Game::spawnEnemies()
{
	int randomX = rand() % (WINDOW_WIDTH - 40);

	enemies.push_back(new Enemy("img/bom.png", randomX, 0, MAX_SPEED));
	number_of_enemy++;
}

void Game::handleEvent()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	//Menu: start, exit, how to play update later;
	case SDL_MOUSEBUTTONDOWN:
		if (isPlaying || lose == true)	break;
		int x, y;
		SDL_GetMouseState(&x, &y);
		std::cout << x << "  " <<  y << std::endl;
		if (x > 270 && x < 500 && y > 160 && y < 240 && !loaded_difficulty && !paused) {
			isPlaying = true;
			loadBackground("img/background1.png");
			loaded_menu = true;
			if (mode == 1) {
				MAX_ENEMY_NUMBERS = 5;
				MAX_SPEED = 7;
			}
			else if (mode == 2) {
				MAX_ENEMY_NUMBERS = 7;
				MAX_SPEED = 9;
				std::cout << "MODE: " << 2 << std::endl;
			}
		}
		else if (x > 270 && x < 500 && y > 560 && y < 650 && !loaded_difficulty && !paused) {
			isRunning = false;
		}
		else if (x > 270 && x < 500 && y > 455 && y < 520 && !loaded_difficulty && !paused) {
			loadBackground("img/difficulty.png");
			loaded_difficulty = true;
		}else if (loaded_difficulty && !paused) {
			if (x > 230 && x < 520 && y > 195 && y < 300) {
				mode = 1;
				loadBackground("img/background_menu.png");
				loaded_difficulty = false;
			}
			else if (x > 230 && x < 520 && y > 400 && y < 520) {
				mode = 2;
				loadBackground("img/background_menu.png");
				loaded_difficulty = false;
			}
		}
		else if (paused && !isPlaying) {
			if (x > 270 && x < 500 && y > 170 && y < 250) {
				isPlaying = true;
				loadBackground("img/background1.png");
				paused = false;
			}
			else if (x > 270 && x < 500 && y > 330 && y < 420) {
				end_game();
				paused = false;
				loadBackground("img/background1.png");
				isPlaying = true;
			}
			else if (x > 270 && x < 500 && y > 500 && y < 600) {
				lose = true;
				end_game();
				isPlaying = false;
				loadBackground("img/background_menu.png");
				paused = false;
			}
		}
		break;
	case SDL_KEYDOWN:
		if (isPlaying && event.key.keysym.sym == SDLK_ESCAPE) {
			loadBackground("img/resume.png");
			paused = true;
			isPlaying = false;
		}
		break;
	default:
		break;
	}
}

void Game::update()
{
	//Spawn a new enemy every 6 seconds until number of enemies are max = 5
	if (isPlaying && number_of_enemy < MAX_ENEMY_NUMBERS && std::chrono::steady_clock::now() - lastEnemySpawnTime >= std::chrono::seconds(6))
	{
		spawnEnemies();
		lastEnemySpawnTime = std::chrono::steady_clock::now();
	}
	//spawn fruits every 3 second
	if (isPlaying && SDL_GetTicks() - startTime >= 3000) {
		spawnFruits();
		startTime = SDL_GetTicks();
	}


	for (auto it = fruits.begin(); it != fruits.end();) {
		if ((*it)->checkCollision(player->getXpos(), player->getYpos(), player->getWidth(), player->getHeight())) {
			eatingStartTime = SDL_GetTicks();
			player->isEating = true;
			//Delete fruit if collide and keep update the following fruits;
			it = fruits.erase(it);
			cnt++;
			std::cout << "Score: " << cnt << std::endl;
			break;
		}
		else if ((*it)->getXPos() >= WINDOW_HEIGHT - FRUIT_SIZE) {
			it = fruits.erase(it);
		}else {
			// Move if not colliding
			(*it)->Update();
			++it;
		}
	}
	player->Update();

	if (player->isEating) {
		Uint32 currentTime = SDL_GetTicks();
		Uint32 elapsedTime = currentTime - eatingStartTime;
		if (elapsedTime >= 50 && elapsedTime <= 200) { // Open mouth in 0.05 - 0.2s
			// Update open_mouth status of player
			player->changeTexture("img/frog2.png");
		}
		else if (elapsedTime > 200) { 
			// Reset to player's initial status close_mouth;
			player->isEating = false;
			player->changeTexture("img/frog1.png");
		}
	}
	for (auto it = enemies.begin(); it != enemies.end();) {
		if ((*it)->checkCollision(player->getXpos(), player->getYpos(), player->getWidth(), player->getHeight())) {
			end_game();
			Uint32 deadtime = SDL_GetTicks();
			Uint32 currentTime = SDL_GetTicks();
			Uint32 elapsedTime = currentTime - deadtime;
			lose = true;
			lose_Time = SDL_GetTicks();
			break;
		}
		else {
			(*it)->Update();
			it++;
		}
	}
}

void Game::end_game() {
	fruits.clear();
	enemies.clear();
	number_of_enemy = 0;
	player->changeTexture("img/frog1.png");
	isPlaying = false;
	player =  new GameObject("img/frog1.png", WINDOW_WIDTH / 2 - PLAYER_WIDTH, WINDOW_HEIGHT - PLAYER_HEIGHT);
}

void Game::render()
{
	SDL_RenderClear(renderer);
	if (lose) {
		Uint32 currentTime = SDL_GetTicks();
		Uint32 elapsedTime = currentTime - lose_Time;
		if (elapsedTime >= 0 && elapsedTime <= 1000) { 
			loadBackground("img/background_exit_3.png");
		}else if (elapsedTime > 1000 && elapsedTime <= 2000) { 
			loadBackground("img/background_exit_2.png");
		}else if (elapsedTime > 2000 && elapsedTime <= 3000) { 
			loadBackground("img/background_exit_1.png");
		}else if (elapsedTime > 3000) { 
			lose = false;
			loadBackground("img/background_menu.png");
		}
	}
	drawBackground();
	if (isPlaying) {
		player->Render();
		for (auto& enemy : enemies) {
			enemy->Render();
		}
		for (auto& fruit : fruits) {
			fruit->Render();
		}
	}
	SDL_RenderPresent(renderer);
	SDL_Delay(10);
}

void Game::cleanUp()
{
	delete player;
	for (auto& enemy : enemies) {
		if (enemy != nullptr) {
			delete enemy;
		}
	}
	for (auto& fruit : fruits) {
		delete fruit;
	}
}


void Game::clean() {
	cleanUp();
	SDL_DestroyWindow(Game::window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Clean!\n";
}