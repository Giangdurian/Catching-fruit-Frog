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
	speed = 5;

	initialJumpYPos = y;
	yVelocity = 0;
	maxJumpHeight = 200;
}

GameObject::~GameObject()
{}

bool GameObject::checkCollision(const Fruit* fruit) const {
	// Tạo các hình chữ nhật bao quanh player và quả
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
            // Dừng trạng thái nhảy
            isJumping = false;
            // Set yPos as the highest pos that player can jump
            yPos = WINDOW_HEIGHT - maxJumpHeight;
            // Set vY = 0
            yVelocity = 0;
            // Đặt trạng thái rơi xuống
            isFalling = true;
        }
    }
    if (isFalling) {
        // Cập nhật vận tốc y (tốc độ di chuyển xuống)
        yVelocity += gravity;
        // Cập nhật vị trí y của nhân vật dựa trên vận tốc y
        yPos += yVelocity;

        // Kiểm tra nếu nhân vật đã rơi xuống đáy
        if (yPos > WINDOW_HEIGHT - 60) {
            // Set yPos của nhân vật thành mặt đất
            yPos = WINDOW_HEIGHT - 60;
            isFalling = false;
        }
    }

    destRect.x = xPos;
    destRect.y = yPos;
    destRect.w = 93 * 2 / 3;
    destRect.h = 90 * 2 / 3;
}

void GameObject::openMouth() {
    objTexture = TextureManager::LoadTexture("img/frog2.png");
}

void GameObject::closeMouth() {
    objTexture = TextureManager::LoadTexture("img/frog1.png");
}


void GameObject::Render()
{
	//Copy objTexture into renderer;
	//srcRect: if this pointer points to other address other than NULL, it
	//use a specific part of the texture, not the entire texture.
	//destRect: determine the position and size of the object on the screen.
	SDL_RenderCopy(Game::renderer, objTexture, NULL, &destRect);
	//SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}

