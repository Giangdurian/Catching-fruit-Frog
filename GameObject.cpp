#include "GameObject.h"
#include "TextureManager.h"

static bool isJumping = false;
static bool isFalling = false;
const int gravity = 1;


GameObject::GameObject(const char* texturesheet, int x, int y)
{
    objTexture = TextureManager::LoadTexture(texturesheet);

	xPos = x;
	yPos = y;
    destRect.w = PLAYER_WIDTH;
    destRect.h = PLAYER_HEIGHT;
	speed = 5;

	initialJumpYPos = y;
	yVelocity = 0;
	maxJumpHeight = 200;
}

GameObject::~GameObject()
{}

void GameObject::changeTexture(const char* path)
{
    SDL_DestroyTexture(objTexture);
    objTexture = TextureManager::LoadTexture(path);
}

bool GameObject::checkCollision(const Fruit* fruit) const {
	SDL_Rect playerRect = { xPos, yPos, destRect.w, destRect.h };
	SDL_Rect fruitRect = { fruit->getXPos(), fruit->getYPos(), FRUIT_SIZE, FRUIT_SIZE };

	//Check if rentangles are intersected
	return SDL_HasIntersection(&playerRect, &fruitRect);
}

void GameObject::Update()
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_LEFT]) {
        xPos -= speed;
        if (xPos < 0) {
            xPos = 0;
        }
    }
    if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
        xPos += speed;
        if (xPos + destRect.w > WINDOW_WIDTH) {
            xPos = WINDOW_WIDTH - destRect.w;
        }
    }
    if (currentKeyStates[SDL_SCANCODE_UP] && !isJumping) {
        yVelocity = -jumpForce;
        isJumping = true;
        initialJumpYPos = yPos;
    }
    if (isJumping) {
        yVelocity += gravity;
        yPos += yVelocity;

        if (yPos < WINDOW_HEIGHT - maxJumpHeight || yVelocity > 0) {
            isJumping = false;
            // Set yPos as the highest pos that player can jump
            yPos = WINDOW_HEIGHT - maxJumpHeight;
            // Set vY = 0
            yVelocity = 0;
            // Update Falling status
            isFalling = true;
        }
    }
    if (isFalling) {
        // Update yVelocity(Move down velocity)
        yVelocity += gravity;
        yPos += yVelocity;

        if (yPos > WINDOW_HEIGHT - PLAYER_HEIGHT) {
            // Set yPos of player on the ground
            yPos = WINDOW_HEIGHT - PLAYER_HEIGHT;
            isFalling = false;
        }
    }

    destRect.x = xPos;
    destRect.y = yPos;
    destRect.w = PLAYER_WIDTH;
    destRect.h = PLAYER_HEIGHT;
}


void GameObject::Render()
{
	//Copy objTexture into renderer;
	//destRect: determine the position and size of the object on the screen.
	SDL_RenderCopy(Game::renderer, objTexture, NULL, &destRect);
}

