#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"

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
	player = new GameObject("img/frog1.png", WINDOW_WIDTH / 2 - 90, WINDOW_HEIGHT - 90);
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
	player->Update();
	//enemy->Update();
}

void Game::render()
{
	SDL_RenderClear(renderer);
	drawBackground();
	player->Render();
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
	std::cout << "Game Clean\n";
}