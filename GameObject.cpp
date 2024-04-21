#include "GameObject.h"
#include "TextureManager.h"

const int gravity = 1;


GameObject::GameObject(const char* texturesheet, int x, int y)
{
    objTexture = TextureManager::LoadTexture(texturesheet);

    xPos = x;
    yPos = y;
    destRect.w = PLAYER_WIDTH;
    destRect.h = PLAYER_HEIGHT;
    speed = 5;
    hp = 3;

    yVelocity = 0;
    maxJumpHeight = 200;
    isJumping = false;
    isFalling = false;
}

GameObject::~GameObject()
{}

void GameObject::changeTexture(const char* path)
{
    SDL_DestroyTexture(objTexture);
    objTexture = TextureManager::LoadTexture(path);
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
        if (xPos + PLAYER_WIDTH > WINDOW_WIDTH) {
            xPos = WINDOW_WIDTH - PLAYER_WIDTH;
        }
    }
    if (isFalling == false && (currentKeyStates[SDL_SCANCODE_UP]) && isJumping == false) {
        yVelocity = -jumpForce;
        isJumping = true;
    }
    if (isJumping) {
        yVelocity += gravity;
        yPos += yVelocity;

        if (yPos < WINDOW_HEIGHT - maxJumpHeight || yVelocity > 0) {
            isJumping = false;
            // Set yPos as the highest pos that player can jump
            yPos = WINDOW_HEIGHT - maxJumpHeight;
            yVelocity = 0;
            isFalling = true;
        }
    }
    if (isFalling) {
        // Update yVelocity(Increase velocity)
        yVelocity += gravity;
        yPos += yVelocity;

        if (yPos > WINDOW_HEIGHT - PLAYER_HEIGHT) {
            // Set yPos of player on the ground
            yPos = WINDOW_HEIGHT - PLAYER_HEIGHT;
            isFalling = false;
        }
    }
    //SDL_Delay(10);

    destRect.x = xPos;
    destRect.y = yPos;
}

void GameObject::Update2()
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_A] != NULL) {
        xPos -= speed;
        if (xPos < 0) {
            xPos = 0;
        }
    }
    if (currentKeyStates[SDL_SCANCODE_D] != NULL) {
        xPos += speed;
        if (xPos + destRect.w > WINDOW_WIDTH) {
            xPos = WINDOW_WIDTH - PLAYER_WIDTH;
        }
    }
    if (isFalling == false && (currentKeyStates[SDL_SCANCODE_W]) && isJumping == false) {
        yVelocity = -jumpForce;
        isJumping = true;
    }
    if (isJumping) {
        yVelocity += gravity;
        yPos += yVelocity;

        if (yPos < WINDOW_HEIGHT - maxJumpHeight || yVelocity > 0) {
            isJumping = false;
            // Set yPos as the highest pos that player can jump
            yPos = WINDOW_HEIGHT - maxJumpHeight;
            yVelocity = 0;
            isFalling = true;
        }
    }
    if (isFalling) {
        // Update yVelocity(Increase velocity)
        yVelocity += gravity;
        yPos += yVelocity;

        if (yPos > WINDOW_HEIGHT - PLAYER_HEIGHT) {
            // Set yPos of player on the ground
            yPos = WINDOW_HEIGHT - PLAYER_HEIGHT;
            isFalling = false;
        }
    }
    //SDL_Delay(10);

    destRect.x = xPos;
    destRect.y = yPos;
}


void GameObject::Render()
{
    //Copy objTexture into renderer;
    //destRect: determine the position and size of the object on the screen.
    SDL_RenderCopy(Game::renderer, objTexture, NULL, &destRect);//NULL: whole picture
}
