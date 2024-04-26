#include "Game.h"
#include "TextureManager.h"
#include "GameObject.h"
#include <random>
#include <cstring>

GameObject* player;
GameObject* player2;
Uint32 eatingStartTime = 0;
Uint32 eatingStartTime2 = 0;
std::vector<Explosion*> explosions;
int winner;


SDL_Renderer* Game::renderer;

TTF_Font* gFont24;
TTF_Font* gFont48;

Game::Game() {}

Game::~Game() {}

void renderText(const std::string& text, int x, int y, TTF_Font* type) {
	SDL_Color textColor = { 0, 0, 0 }; //Black


	SDL_Surface* textSurface = TTF_RenderText_Solid(type, text.c_str(), textColor);
	if (textSurface == NULL) {
		printf("Unable to render text surface! Error: %s\n", TTF_GetError());
	}
	else {
		// Create texture from surface
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(Game::renderer, textSurface);
		if (textTexture == NULL) {
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else {
			// render texture
			SDL_Rect renderQuad = { x, y, textSurface->w, textSurface->h };
			SDL_RenderCopy(Game::renderer, textTexture, NULL, &renderQuad);
			SDL_DestroyTexture(textTexture);
		}
		SDL_FreeSurface(textSurface);
	}
}


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
	//initialize player;
	player = new GameObject("img/frog1.png", WINDOW_WIDTH / 2 - PLAYER_WIDTH - 30, WINDOW_HEIGHT - PLAYER_HEIGHT);
	player2 = new GameObject("img/frog2.png", WINDOW_WIDTH / 2 - PLAYER_WIDTH + 30, WINDOW_HEIGHT - PLAYER_HEIGHT);
	loadBackground("img/background_menu.jpg");
	if (TTF_Init() == -1) {
		printf("SDL_ttf could not be opened! Error: %s\n", TTF_GetError());
		return;
	}
	//load font
	gFont24 = TTF_OpenFont("OpenSans-Bold.ttf", 24);
	gFont48 = TTF_OpenFont("OpenSans-Bold.ttf", 48);
	//load sound
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	clickingSound = Mix_LoadWAV("sound/clicking.wav");
	eatingSound = Mix_LoadWAV("sound/eating.wav");
	background_music = Mix_LoadWAV("sound/background_music.wav");
	game_play_music = Mix_LoadWAV("sound/game_play_music.wav");
	eatingSound = Mix_LoadWAV("sound/Eating_sound.wav");
	explosionSound = Mix_LoadWAV("sound/explosion.wav");
	healSound = Mix_LoadWAV("sound/healing.wav");
	shieldSound = Mix_LoadWAV("sound/shield.wav");
	Mix_VolumeChunk(background_music, MIX_MAX_VOLUME / 3);
	Mix_VolumeChunk(game_play_music, MIX_MAX_VOLUME / 3);
	Mix_PlayChannel(2, background_music, -1);
	//
	MAX_ENEMY_NUMBERS = 5;
	MAX_ENEMY_SPEED = 3;
	MAX_ENEMY_SPEED = 6;
}

void Game::first_spawn() {
	spawnFruits();
	spawnEnemies();
	spawnPoisions();
}

void Game::spawnFruits()
{	// Path to fruits images
	std::vector<std::string> fruitPaths = {
		"img/melon.png",
		"img/grape.png",
		"img/strawberry.png",
		"img/cherry.png",
		"img/banana.png",
		"img/apple.png"
	};

	int num = 1 + rand() % 6;

	for (int i = 0; i < num; ++i)
	{
		int randomIndex = rand() % fruitPaths.size();
		std::string fruit_type = fruitPaths[randomIndex];
		// Add a new fruits and push_back into the fruits_list;
		int randomX = 40 + rand() % (WINDOW_WIDTH - 80);
		fruits.push_back(new Fruit(fruit_type.c_str(), randomX, 0));
	}
}

void Game::spawnHP() {
	int randomX = 40 + rand() % (WINDOW_WIDTH - 80);
	fruits.push_back(new Fruit("img/hp.png", randomX, 0));
	fruits.back()->is_HP = true;
}

void Game::spawnShield() {
	int randomX = 40 + rand() % (WINDOW_WIDTH - 80);
	fruits.push_back(new Fruit("img/shield.png", randomX, 0));
	fruits.back()->is_Shield = true;
}

void Game::spawnPoisions()
{
	int num = 1 + rand() % 2;
	for (int i = 0; i < num; ++i)
	{
		int randomX = 40 + rand() % (WINDOW_WIDTH - 80);
		poisions.push_back(new Poision("img/poision.png", randomX, 0));
	}
}


void Game::spawnEnemies()
{
	int randomX = rand() % (WINDOW_WIDTH - 40);

	enemies.push_back(new Enemy("img/bom.png", randomX, 0, MAX_ENEMY_SPEED, MIN_ENEMY_SPEED));
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
		std::cout << x << "  " << y << std::endl;
		if (x > 250 && x < 540 && y > 90 && y < 200 && !start_clicked && !difficulty_clicked && !paused && !tutorial_clicked) {
			Mix_PlayChannel(1, clickingSound, 0);
			loadBackground("img/background_play_mode.jpg");
			start_clicked = true;
			if (mode == 1) {
				MAX_ENEMY_NUMBERS = 5;
				MAX_ENEMY_SPEED = 6;
				MIN_ENEMY_SPEED = 3;
				std::cout << "speed: " << MAX_ENEMY_SPEED << std::endl;
			}
			else if (mode == 2) {
				MAX_ENEMY_NUMBERS = 7;
				MAX_ENEMY_SPEED = 8;
				MIN_ENEMY_SPEED = 5;
				std::cout << "speed: " << MAX_ENEMY_SPEED << std::endl;
			}
		}
		else if (x > 250 && x < 540 && y > 240 && y < 340 && !difficulty_clicked && !paused && !start_clicked && !tutorial_clicked) {
			Mix_PlayChannel(1, clickingSound, 0);
			loadBackground("img/tutorial.jpg");
			tutorial_clicked = true;
		}
		else if (x > 250 && x < 540 && y > 400 && y < 525 && !difficulty_clicked && !paused && !start_clicked && !tutorial_clicked) {
			Mix_PlayChannel(1, clickingSound, 0);
			loadBackground("img/difficulty.jpg");
			difficulty_clicked = true;
		}
		else if (x > 250 && x < 540 && y > 550 && y < 650 && !difficulty_clicked && !paused && !start_clicked) {
			Mix_PlayChannel(1, clickingSound, 0);
			isRunning = false;
		}
		else if (difficulty_clicked) {
			if (x > 220 && x < 560 && y > 190 && y < 300) {
				Mix_PlayChannel(1, clickingSound, 0);
				mode = 1;
				loadBackground("img/background_menu.jpg");
				difficulty_clicked = false;
			}
			else if (x > 220 && x < 560 && y > 420 && y < 530) {
				Mix_PlayChannel(1, clickingSound, 0);
				mode = 2;
				loadBackground("img/background_menu.jpg");
				difficulty_clicked = false;
			}
			else if (x > 10 && x < 85 && y > 10 && y < 75) {
				Mix_PlayChannel(1, clickingSound, 0);
				difficulty_clicked = false;
				loadBackground("img/background_menu.jpg");
			}
		}
		else if (paused) {
			if (x > 250 && x < 520 && y > 150 && y < 250) {
				Mix_PlayChannel(1, clickingSound, 0);
				Mix_PlayChannel(2, game_play_music, -1);
				isPlaying = true;
				loadBackground("img/background1.png");
				paused = false;
			}
			else if (x > 250 && x < 520 && y > 330 && y < 430) {
				Mix_PlayChannel(1, clickingSound, 0);
				player = new GameObject("img/frog1.png", WINDOW_WIDTH / 2 - PLAYER_WIDTH - 30, WINDOW_HEIGHT - PLAYER_HEIGHT);
				player2 = new GameObject("img/frog2.png", WINDOW_WIDTH / 2 - PLAYER_WIDTH + 30, WINDOW_HEIGHT - PLAYER_HEIGHT);
				if (number_of_player == 1)	player2->isDead = true;
				end_game();
				Mix_PlayChannel(2, game_play_music, -1);
				reset_time();
				paused = false;
				loadBackground("img/background1.png");
				isPlaying = true;
			}
			else if (x > 270 && x < 500 && y > 530 && y < 620) {
				Mix_PlayChannel(1, clickingSound, 0);
				//lose = true;
				end_game();
				isPlaying = false;
				loadBackground("img/background_menu.jpg");
				paused = false;
			}
		}
		else if (start_clicked) {
			if (x > 230 && x < 570 && y > 180 && y < 300) {
				Mix_PlayChannel(1, clickingSound, 0);
				Mix_PlayChannel(2, game_play_music, -1);
				player = new GameObject("img/frog1.png", WINDOW_WIDTH / 2 - PLAYER_WIDTH - 30, WINDOW_HEIGHT - PLAYER_HEIGHT);
				player2 = new GameObject("img/frog2.png", WINDOW_WIDTH / 2 - PLAYER_WIDTH + 30, WINDOW_HEIGHT - PLAYER_HEIGHT);
				player->isDead = false;
				player2->isDead = true;
				isPlaying = true;
				loadBackground("img/background1.png");
				number_of_player = 1;
				start_clicked = false;
				reset_time();
			}
			else if (x > 230 && x < 560 && y > 400 && y < 520) {
				Mix_PlayChannel(1, clickingSound, 0);
				Mix_PlayChannel(2, game_play_music, -1);
				player = new GameObject("img/frog1.png", WINDOW_WIDTH / 2 - PLAYER_WIDTH - 30, WINDOW_HEIGHT - PLAYER_HEIGHT);
				player2 = new GameObject("img/frog2.png", WINDOW_WIDTH / 2 - PLAYER_WIDTH + 30, WINDOW_HEIGHT - PLAYER_HEIGHT);
				player->isDead = false;
				player2->isDead = false;
				isPlaying = true;
				loadBackground("img/background1.png");
				number_of_player = 2;
				start_clicked = false;
				reset_time();
			}
			else if (x > 10 && x < 60 && y > 10 && y < 70) {
				Mix_PlayChannel(1, clickingSound, 0);
				loadBackground("img/background_menu.jpg");
				start_clicked = false;
			}
		}
		else if (tutorial_clicked) {
			if (x > 15 && x < 85 && y > 10 && y < 85) {
				Mix_PlayChannel(1, clickingSound, 0);
				loadBackground("img/background_menu.jpg");
				tutorial_clicked = false;
			}
		}
		break;
	case SDL_KEYDOWN:
		if (isPlaying && event.key.keysym.sym == SDLK_ESCAPE) {
			Mix_PlayChannel(2, background_music, -1);
			loadBackground("img/resume.jpg");
			paused = true;
			isPlaying = false;
		}
		break;
	default:
		break;
	}
	//SDL_Delay(5);
}

void Game::reset_time() {
	lastFruits_SpawnTime = std::chrono::steady_clock::now();
	lastPosions_SpawnTime = std::chrono::steady_clock::now();
	lastEnemy_SpawnTime = std::chrono::steady_clock::now();
	lastHP_SpawnTime = std::chrono::steady_clock::now();
	lastShield_SpawnTime = std::chrono::steady_clock::now();
}

void Game::update()
{
	if (!first_spawned) {
		first_spawn();
		first_spawned = true;
	}
	//Spawn a new enemy every 6 seconds until number of enemies are max = MAX_ENEMY
	if (isPlaying && number_of_enemy < MAX_ENEMY_NUMBERS && std::chrono::steady_clock::now() - lastEnemy_SpawnTime >= std::chrono::seconds(6))
	{
		std::cout << MAX_ENEMY_NUMBERS << std::endl;
		spawnEnemies();
		lastEnemy_SpawnTime = std::chrono::steady_clock::now();
	}
	//spawn fruits every 6 second
	if (isPlaying && std::chrono::steady_clock::now() - lastFruits_SpawnTime >= std::chrono::seconds(6)) {
		spawnFruits();
		lastFruits_SpawnTime = std::chrono::steady_clock::now();
	}
	if (isPlaying && std::chrono::steady_clock::now() - lastPosions_SpawnTime >= std::chrono::seconds(8)) {
		spawnPoisions();
		lastPosions_SpawnTime = std::chrono::steady_clock::now();
	}
	if (isPlaying && std::chrono::steady_clock::now() - lastHP_SpawnTime >= std::chrono::seconds(20)) {
		spawnHP();

		lastHP_SpawnTime = std::chrono::steady_clock::now();
	}
	if (isPlaying && std::chrono::steady_clock::now() - lastShield_SpawnTime >= std::chrono::seconds(15)) {
		spawnShield();
		lastShield_SpawnTime = std::chrono::steady_clock::now();
	}
	if (!(player->isDead))	player->Update();
	if (!(player2->isDead)) player2->Update2();

	for (auto it = fruits.begin(); it != fruits.end();) {
		if (!player->isDead) {
			if ((*it)->checkCollision(player->getXpos(), player->getYpos(), player->getWidth(), player->getHeight())) {
				if ((*it)->is_HP) {
					player->hp++;
					Mix_PlayChannel(-1, healSound, 0);
				}
				else if ((*it)->is_Shield) {
					Mix_PlayChannel(-1, shieldSound, 0);
					player->Protected = true;
					player->bonusHP = 3;
					player->protecting_start_time = std::chrono::steady_clock::now();
				}
				else {
					Mix_PlayChannel(-1, eatingSound, 0);
					player->score++;
				}
				eatingStartTime = SDL_GetTicks();
				player->isEating = true;
				//Delete fruit if collide and keep update the following fruits;
				it = fruits.erase(it);
				break;
			}
		}
		if (!player2->isDead) {
			if ((*it)->checkCollision(player2->getXpos(), player2->getYpos(), player2->getWidth(), player2->getHeight())) {
				if ((*it)->is_HP) {
					player2->hp++;
					Mix_PlayChannel(-1, healSound, 0);
				}
				else if ((*it)->is_Shield) {
					Mix_PlayChannel(-1, shieldSound, 0);
					player2->Protected = true;
					player2->bonusHP = 3;
					player2->protecting_start_time = std::chrono::steady_clock::now();
				}
				else {
					Mix_PlayChannel(-1, eatingSound, 0);
					player2->score++;
				}
				eatingStartTime2 = SDL_GetTicks();
				player2->isEating = true;
				//Delete fruit if collide and keep update the following fruits;
				it = fruits.erase(it);
				break;
			}
		}
		if ((*it)->getXPos() >= WINDOW_HEIGHT - FRUIT_SIZE) {
			it = fruits.erase(it);
		}
		else {
			// Move if not colliding
			(*it)->Update();
			++it;
		}
	}

	for (auto it = poisions.begin(); it != poisions.end();) {
		if (!player->isDead) {
			if ((*it)->checkCollision(player->getXpos(), player->getYpos(), player->getWidth(), player->getHeight())) {
				explosions.push_back(new Explosion(player->getXpos() - 30, player->getYpos() - 30));
				Mix_PlayChannel(-1, explosionSound, 0);
				if (player->Protected) {
					player->bonusHP--;
				}
				else {
					player->hp--;
				}
				it = poisions.erase(it);
				if (player->hp == 0) {
					player->isDead = true;
				}
				break;
			}
		}

		if (!player2->isDead) {
			if ((*it)->checkCollision(player2->getXpos(), player2->getYpos(), player2->getWidth(), player2->getHeight())) {
				explosions.push_back(new Explosion(player2->getXpos() - 30, player2->getYpos() - 30));
				Mix_PlayChannel(-1, explosionSound, 0);
				if (player2->Protected) {
					player2->bonusHP--;
				}
				else {
					player2->hp--;
				}
				it = poisions.erase(it);
				if ((player2->hp) == 0) {
					player2->isDead = true;
				}
				break;
			}
		}
		if (player->isDead && player2->isDead) {
			lose = true;
			end_game();
			break;
		}
		else if ((*it) != NULL) {
			(*it)->Update();
			it++;
		}
	}

	if (!(player->isDead)) {
		if (player->isEating) {
			Uint32 currentTime = SDL_GetTicks();
			Uint32 elapsedTime = currentTime - eatingStartTime;
			if (elapsedTime >= 50 && elapsedTime <= 200) { // Open mouth in 0.05 - 0.2s
				// Update open_mouth status of player
				player->changeTexture("img/frog1_open.png");
			}
			else if (elapsedTime > 200) {
				// Reset to player's initial status close_mouth;
				player->isEating = false;
				player->changeTexture("img/frog1.png");
			}
		}
	}

	if (!(player2->isDead)) {
		if (player2->isEating) {
			Uint32 currentTime = SDL_GetTicks();
			Uint32 elapsedTime = currentTime - eatingStartTime2;
			if (elapsedTime >= 50 && elapsedTime <= 200) { // Open mouth in 0.05 - 0.2s
				// Update open_mouth status of player
				player2->changeTexture("img/frog2_open.png");
			}
			else if (elapsedTime > 200) {
				// Reset to player's initial status close_mouth;
				player2->isEating = false;
				player2->changeTexture("img/frog2.png");
			}
		}
	}
	for (auto it = enemies.begin(); it != enemies.end();) {
		if (!(player->isDead)) {
			if ((*it)->checkCollision(player->getXpos(), player->getYpos(), player->getWidth(), player->getHeight())) {
				explosions.push_back(new Explosion(player->getXpos() - 30, player->getYpos() - 30));
				Mix_PlayChannel(-1, explosionSound, 0);
				if (player->Protected) {
					player->bonusHP--;
				}
				else {
					player->hp--;
				}
				number_of_enemy--;
				it = enemies.erase(it);
				if (player->hp == 0) {
					player->isDead = true;
				}
				//continue;
				lastEnemy_SpawnTime = std::chrono::steady_clock::now();
				break;
			}
		}

		if (!player2->isDead) {
			if ((*it)->checkCollision(player2->getXpos(), player2->getYpos(), player2->getWidth(), player2->getHeight())) {
				explosions.push_back(new Explosion(player2->getXpos() - 30, player2->getYpos() - 30));
				Mix_PlayChannel(-1, explosionSound, 0);
				if (player2->Protected) {
					(player2->bonusHP)--;
				}
				else {
					(player2->hp)--;
				}
				number_of_enemy--;
				it = enemies.erase(it);
				if ((player2->hp) == 0) {
					player2->isDead = true;
				}
				//continue;
				lastEnemy_SpawnTime = std::chrono::steady_clock::now();
				break;
			}
		}

		if (std::chrono::steady_clock::now() - player->protecting_start_time >= std::chrono::seconds(5) || player->bonusHP == 0)
			player->Protected = false;

		if (!player2->isDead) {
			if (std::chrono::steady_clock::now() - player2->protecting_start_time >= std::chrono::seconds(5) || player2->bonusHP == 0)
				player2->Protected = false;
		}

		if (player->isDead && player2->isDead) {
			lose = true;
			end_game();
			break;
		}
		else if ((*it) != NULL) {
			(*it)->Update(MAX_ENEMY_SPEED, MIN_ENEMY_SPEED);
			it++;
		}
	}
}

void Game::end_game() {
	Mix_PlayChannel(2, background_music, -1);
	fruits.clear();
	enemies.clear();
	poisions.clear();
	explosions.clear();
	number_of_enemy = 0;
	winner = 0;
	isPlaying = false;
	isPlaying = false;
	first_spawned = false;
	if (player->score > player2->score)	winner = 1;
	else if (player->score < player2->score)	winner = 2;
	else winner = 0;
}

void Game::render()
{
	SDL_RenderClear(renderer);
	drawBackground();
	if (lose) {
		loadBackground("img/background_exit.jpg");
		if (number_of_player == 1) {
			renderText("Your score: ", WINDOW_WIDTH / 2 - 250, 280, gFont48);
			renderText(std::to_string(player->score), WINDOW_WIDTH / 2 + 20, 280, gFont48);
		}
		else {
			if (winner == 1) {
				renderText("Winner: ", WINDOW_WIDTH / 2 - 250, 280, gFont48);
				SDL_Rect dest = { WINDOW_WIDTH / 2, 280, PLAYER_WIDTH, PLAYER_HEIGHT };
				SDL_RenderCopy(Game::renderer, TextureManager::LoadTexture("img/frog1.png"), NULL, &dest);
			}
			else if (winner == 2) {
				renderText("Winner: ", WINDOW_WIDTH / 2 - 250, 280, gFont48);
				SDL_Rect dest = { WINDOW_WIDTH / 2, 280, PLAYER_WIDTH, PLAYER_HEIGHT };
				SDL_RenderCopy(Game::renderer, TextureManager::LoadTexture("img/frog2.png"), NULL, &dest);
			}
			else {
				renderText("Draw", WINDOW_WIDTH / 2 - 250, 280, gFont48);
			}

			renderText("Player 1: ", WINDOW_WIDTH / 2 - 250, 350, gFont48);
			renderText(std::to_string(player->score), WINDOW_WIDTH / 2 + 20, 350, gFont48);
			renderText("Player 2: ", WINDOW_WIDTH / 2 - 250, 450, gFont48);
			renderText(std::to_string(player2->score), WINDOW_WIDTH / 2 + 20, 450, gFont48);
		}

		SDL_Event event;
		SDL_PollEvent(&event);
		switch (event.type) {
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_SPACE) {
				loadBackground("img/background_menu.jpg");
				lose = false;
			}
			break;
		default:
			break;
		}
	}
	if (isPlaying) {
		if (!player->isDead)	player->Render();
		if (!player2->isDead)	player2->Render();
		for (auto& enemy : enemies) {
			enemy->Render();
		}
		for (auto& fruit : fruits) {
			fruit->Render();
		}
		for (auto& poision : poisions) {
			poision->Render();
		}
		for (auto it = explosions.begin(); it != explosions.end();) {
			(*it)->update();
			(*it)->render();
			if ((*it)->isDestroyed()) {
				it = explosions.erase(it);
			}
			else {
				++it;
			}
			std::cout << "BOOOM\n";
		}
		renderText("Player 1: ", 10, 10, gFont24);
		renderText(std::to_string(player->score), 120, 10, gFont24);
		renderText("HP 1: ", 10, 40, gFont24);
		renderText(std::to_string(player->hp), 80, 40, gFont24);
		if (player->Protected && player->bonusHP != 0) {
			SDL_Rect dest = { 10, 70, 42, 46 };
			SDL_RenderCopy(Game::renderer, TextureManager::LoadTexture("img/shield.png"), NULL, &dest);
		}

		if (number_of_player == 2) {
			renderText("Player 2: ", WINDOW_WIDTH - 160, 10, gFont24);
			renderText(std::to_string(player2->score), WINDOW_WIDTH - 50, 10, gFont24);
			renderText("HP 2: ", WINDOW_WIDTH - 160, 40, gFont24);
			renderText(std::to_string(player2->hp), WINDOW_WIDTH - 90, 40, gFont24);
			if (player2->Protected && player2->bonusHP != 0) {
				SDL_Rect dest = { WINDOW_WIDTH - 160, 70, 42, 46 };
				SDL_RenderCopy(Game::renderer, TextureManager::LoadTexture("img/shield.png"), NULL, &dest);
			}
		}
	}
	SDL_RenderPresent(renderer);
}

void Game::cleanUp()
{
	player = NULL;
	player2 = NULL;
	delete player;
	delete player2;
	for (auto& enemy : enemies) {
		if (enemy != nullptr) {
			delete enemy;
		}
	}
	for (auto& fruit : fruits) {
		delete fruit;
	}
	for (auto& poision : poisions) {
		delete poision;
	}

	for (auto& explosion : explosions) {
		delete explosion;
	}
}


void Game::clean() {
	cleanUp();
	Mix_FreeChunk(clickingSound);
	Mix_FreeChunk(eatingSound);
	Mix_FreeChunk(background_music);
	Mix_FreeChunk(game_play_music);
	Mix_FreeChunk(explosionSound);
	Mix_FreeChunk(healSound);
	Mix_FreeChunk(shieldSound);
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	std::cout << "Game Clean!\n";
}