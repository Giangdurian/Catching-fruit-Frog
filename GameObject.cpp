#include "GameObject.h"
#include "TextureManager.h"

GameObject::GameObject(const char* texturesheet, int x, int y)
{
	objTexture = TextureManager::LoadTexture(texturesheet);

	xPos = x;
	yPos = y;
	speed = 5;
}

GameObject::~GameObject()
{}

void GameObject::Update()
{
	const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
	if (currentKeyStates[SDL_SCANCODE_LEFT]) {
		xPos -= speed;
		if (xPos < 0) {
			xPos = 0;
		}
	}
	else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
		xPos += speed;
		if (xPos + destRect.w > WINDOW_WIDTH) {
			xPos = WINDOW_WIDTH - destRect.w;
		}
	}
	//height and width(size) of the pictures
	srcRect.h = 90;
	srcRect.w = 90;
	//the position of the top-left conner you want to cut, generally (0,0);
	srcRect.x = 0;
	srcRect.y = 0;

	destRect.x = xPos;
	destRect.y = yPos;
	destRect.w = srcRect.w;
	destRect.h = srcRect.h;

}

void GameObject::Render()
{
	//Copy objTexture into renderer;
	//srcRect: if this pointer points to other address other than NULL, it
	//use a specific part of the texture, not the entire texture.
	//destRect: determine the position and size of the object on the screen.
	SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}

