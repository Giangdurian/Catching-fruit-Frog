#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture)
{

	SDL_Surface* tmpSurface = IMG_Load(texture);
	//Delete background of player, fruits, and ênmies;
	SDL_SetColorKey(tmpSurface, SDL_TRUE, SDL_MapRGB(tmpSurface->format, 255, 255, 255));
	SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tmpSurface);
	SDL_FreeSurface(tmpSurface);
	return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest)
{
	//Copy tex into Game::renderer
	SDL_RenderCopy(Game::renderer, tex, &src, &dest);
}