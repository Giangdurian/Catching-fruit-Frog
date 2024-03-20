#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include <random>

GameObject* player;
Uint32 eatingStartTime = 0;


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
	loadBackground("img/background1.png");
	//create a new object, in this case it's a player;
	player = new GameObject("img/frog1_resize.png", WINDOW_WIDTH / 2 - PLAYER_WIDTH, WINDOW_HEIGHT - PLAYER_HEIGHT);
}

void Game::spawnFruits()
	{	// Initialize path to fruits images
		std::vector<std::string> fruitPaths = {
			"img/melon.png",
			"img/nho.png",
			"img/star.png",
			"img/cherry.png",
		};
		// Shuffle fruits
		std::random_device rd;
		std::mt19937 g(rd());
		std::shuffle(fruitPaths.begin(), fruitPaths.end(), g);

		int num = 1 + rand() % 5;

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

	enemies.push_back(new Enemy("img/bom.png", randomX, 0));
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
	case SDL_MOUSEBUTTONDOWN:
		if (isPlaying)	break;
		int x, y;
		SDL_GetMouseState(&x, &y);
		if (x > 240 && x < 480 && y > 160 && y < 320) {
			isPlaying = true;
		}
		if (x > 240 && x < 480 && y > 360 && y < 420) {
			isRunning = false;
		}
		break;
	default:
		break;
	}
}

void Game::update()
{
	//Spawn a new enemy every 6 seconds until number of enemies are max = 5
	if (number_of_enemy < MAX_ENEMY_NUMBERS && std::chrono::steady_clock::now() - lastEnemySpawnTime >= std::chrono::seconds(6))
	{
		spawnEnemies();
		lastEnemySpawnTime = std::chrono::steady_clock::now();
	}
	//spawn fruits every 3 second
	if (SDL_GetTicks() - startTime >= 3000) {
		spawnFruits();
		startTime = SDL_GetTicks();
	}
	
	for (auto it = fruits.begin(); it != fruits.end();) {
		if (player->checkCollision(*it)) {
			// Delete fruit if collide
			eatingStartTime = SDL_GetTicks();
			isEating = true;
			delete* it;
			it = fruits.erase(it);
			cnt++;
			std::cout << "Score: " << cnt << std::endl;
			player->changeTexture("img/frog2_resize.png");
			break;
		}
		else {
			// Move if not collide
			(*it)->Update();
			++it;
		}
	}
	player->Update();

	if (isEating) {
		Uint32 currentTime = SDL_GetTicks();
		Uint32 elapsedTime = currentTime - eatingStartTime;
		if (elapsedTime >= 50 && elapsedTime <= 200) { // Open mouth in 0.1 - 0.3s
			// Update open_mouth status of player
			player->changeTexture("img/frog2.png");
		}
		else if (elapsedTime > 200 && elapsedTime <= 500) { // 0.3s - 0.5s
			// Reset to player's initial status close_mouth;
			isEating = false;
			player->changeTexture("img/frog1_resize.png");
		}
	}
	for (auto it = enemies.begin(); it != enemies.end();) {
		if ((*it)->checkCollision(player->getXpos(), player->getYpos(), player->getWidth(), player->getHeight())) {
			isPlaying = false;
			break;
		}
		else {
			(*it)->Update();
			it++;
		}
	}
}

void Game::render()
{
	SDL_RenderClear(renderer);
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
}

void Game::cleanUp()
{
	if (player != nullptr)
	{
		delete player;
		player = nullptr;
	}
	for (auto& enemy : enemies) {
		if (enemy != nullptr) {
			delete enemy;
			enemy = nullptr;
		}
	}
}


void Game::clean() {
	cleanUp();
	SDL_DestroyWindow(Game::window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Clean!\n";
}