#include "Player.h"
#include "Barrel.h"

Player::Player(int startX, int startY) {
    x = startX;
    y = startY;
    width = 8;
    height = 8;
    facing = DIR_DOWN;
    currentFrame = 0;
    frameTimer = 0;
    isMoving = false;
    alive = true;
    inBarrel = false;
    breathTimer = 100;
    headVariation = 0;
    bodyVariation = 0;
    
    for (int i = 0; i < 4; i++) {
        headTextures[i] = nullptr;
        for (int j = 0; j < 3; j++) {
            bodyTextures[i][j] = nullptr;
        }
    }
}

Player::~Player() {}

void Player::setHeadTexture(Direction dir, SDL_Texture* tex) {
    headTextures[dir] = tex;
}

void Player::setBodyTexture(Direction dir, int frame, SDL_Texture* tex) {
    if (frame >= 0 && frame < 3) {
        bodyTextures[dir][frame] = tex;
    }
}

void Player::update(bool up, bool down, bool left, bool right,
                    const std::vector<SDL_Rect>& walls,
                    const std::vector<Barrel*>& barrels) {
    
    if (!alive || inBarrel) return;
    
    int newX = x, newY = y;
    isMoving = false;
    
    if (up) { newY -= 2; facing = DIR_UP; isMoving = true; }
    if (down) { newY += 2; facing = DIR_DOWN; isMoving = true; }
    if (left) { newX -= 2; facing = DIR_LEFT; isMoving = true; }
    if (right) { newX += 2; facing = DIR_RIGHT; isMoving = true; }
    
    SDL_Rect newRect = {newX, newY, width, height};
    bool canMove = true;
    
    for (const auto& wall : walls) {
        if (SDL_HasIntersection(&newRect, &wall)) {
            canMove = false;
            break;
        }
    }
    
    if (canMove) {
        x = newX;
        y = newY;
    }
    
    if (isMoving) {
        frameTimer++;
        if (frameTimer > 5) {
            if (currentFrame == 1) currentFrame = 2;
            else currentFrame = 1;
            frameTimer = 0;
        }
    } else {
        currentFrame = 0;
        frameTimer = 0;
    }
}

void Player::render(SDL_Renderer* renderer, int cameraX, int cameraY) {
    if (!alive) return;
    
    int screenX = x - cameraX;
    int screenY = y - cameraY;
    
    if (bodyTextures[facing][currentFrame]) {
        SDL_Rect dest = {screenX, screenY + 6, width, height};
        SDL_RenderCopy(renderer, bodyTextures[facing][currentFrame], NULL, &dest);
    }
    
    if (headTextures[facing]) {
        SDL_Rect dest = {screenX, screenY, 8, 6};
        SDL_RenderCopy(renderer, headTextures[facing], NULL, &dest);
    }
}

void Player::enterBarrel(Barrel* barrel) {
    inBarrel = true;
    breathTimer = 100;
    x = barrel->getX() + 1;
    y = barrel->getY() + 1;
}

void Player::exitBarrel() {
    inBarrel = false;
}

void Player::die() {
    alive = false;
}

void Player::respawn(int startX, int startY) {
    x = startX;
    y = startY;
    alive = true;
    inBarrel = false;
    breathTimer = 100;
    currentFrame = 0;
}
