#include "Enemy.h"
#include "Player.h"

Enemy::Enemy(int startX, int startY, int w, int h) {
    x = startX;
    y = startY;
    width = w;
    height = h;
    facing = DIR_DOWN;
    speed = 1;
    active = true;
}

Enemy::~Enemy() {}

bool Enemy::checkCollision(const Player& player) {
    if (!active || !player.isAlive() || player.isInBarrel()) return false;
    
    SDL_Rect playerRect = player.getRect();
    SDL_Rect enemyRect = {x, y, width, height};
    
    return SDL_HasIntersection(&playerRect, &enemyRect);
}
