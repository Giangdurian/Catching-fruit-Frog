#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture)
{

	SDL_Surface* tmpSurface = IMG_Load(texture);
	//Delete background of player, fruits, and ênmies;
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	return tex;
}
