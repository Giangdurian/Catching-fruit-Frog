#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include "Fruit.h"
#include <random>

GameObject* player;
//GameObject* enemy;
//Map* map;

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
	player = new GameObject("img/frog1.png", WINDOW_WIDTH / 2 - 60, WINDOW_HEIGHT - 60);
}

	void Game::spawnFruits()
	{		// Initialize path to fruits images
			std::vector<std::string> fruitPaths = {
				"img/melon.png",
				"img/nho.png",
				"img/star.png",
				"img/cherry.png",
				"img/bom.png"
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



void Game::handleEvent()
{
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		isRunning = false;
		break;
	default:
		break;
	}
}

void Game::update()
{
	if (SDL_GetTicks() - startTime >= 3000) {
		spawnFruits();
		startTime = SDL_GetTicks();
	}
	
	for (auto it = fruits.begin(); it != fruits.end(); ) {
		if (player->checkCollision(*it)) {
			// Delete fruit if collide
			isEating = true;
			delete* it;
			it = fruits.erase(it);
			cnt++;
			std::cout << "Score: " << cnt << std::endl;
			player->openMouth();
		}
		else {
			// Move if not collide
			(*it)->Update();
			player->closeMouth();
			++it;
		}
	}
	player->Update();
}

void Game::render()
{
	SDL_RenderClear(renderer);
	drawBackground();
	player->Render();
	for (auto& fruit : fruits)
	{
		fruit->Render();
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

}


void Game::clean() {
	cleanUp();
	SDL_DestroyWindow(Game::window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Clean!\n";
}