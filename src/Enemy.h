#ifndef ENEMY_H
#define ENEMY_H

#include <SDL2/SDL.h>
#include "utils.h"

class Player;

class Enemy {
protected:
    int x, y;
    int width, height;
    Direction facing;
    int speed;
    bool active;
    
public:
    Enemy(int startX, int startY, int w, int h);
    virtual ~Enemy();
    
    virtual void update(const Player& player, const std::vector<SDL_Rect>& walls) = 0;
    virtual void render(SDL_Renderer* renderer, int cameraX, int cameraY) = 0;
    
    int getX() { return x; }
    int getY() { return y; }
    SDL_Rect getRect() { return {x, y, width, height}; }
    bool isActive() { return active; }
    
    bool checkCollision(const Player& player);
};

#endif
