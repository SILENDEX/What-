#include "Kunti.h"
#include "Player.h"
#include <cmath>

Kunti::Kunti(int startX, int startY) 
    : Enemy(startX, startY, 8, 13) {
    chaseRange = 100;
    wanderTimer = 0;
    wanderDir = 0;
    
    for (int i = 0; i < 4; i++) {
        textures[i] = nullptr;
    }
}

Kunti::~Kunti() {}

void Kunti::setTexture(Direction dir, SDL_Texture* tex) {
    textures[dir] = tex;
}

void Kunti::update(const Player& player, const std::vector<SDL_Rect>& walls) {
    if (!active) return;
    
    int playerX = player.getX();
    int playerY = player.getY();
    
    int dx = playerX - x;
    int dy = playerY - y;
    int distance = std::sqrt(dx*dx + dy*dy);
    
    int newX = x, newY = y;
    
    if (distance < chaseRange) {
        if (std::abs(dx) > std::abs(dy)) {
            if (dx > 0) { newX += speed; facing = DIR_RIGHT; }
            else { newX -= speed; facing = DIR_LEFT; }
        } else {
            if (dy > 0) { newY += speed; facing = DIR_DOWN; }
            else { newY -= speed; facing = DIR_UP; }
        }
    } else {
        wanderTimer++;
        if (wanderTimer > 30) {
            wanderDir = randomInt(0, 3);
            wanderTimer = 0;
        }
        
        switch (wanderDir) {
            case 0: newY -= speed; facing = DIR_UP; break;
            case 1: newY += speed; facing = DIR_DOWN; break;
            case 2: newX -= speed; facing = DIR_LEFT; break;
            case 3: newX += speed; facing = DIR_RIGHT; break;
        }
    }
    
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
}

void Kunti::render(SDL_Renderer* renderer, int cameraX, int cameraY) {
    if (!active || !textures[facing]) return;
    
    SDL_Rect dest = {x - cameraX, y - cameraY, width, height};
    SDL_RenderCopy(renderer, textures[facing], NULL, &dest);
}

void Kunti::respawn(int newX, int newY) {
    x = newX;
    y = newY;
    active = true;
}
