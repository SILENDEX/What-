#ifndef BARREL_H
#define BARREL_H

#include <SDL2/SDL.h>

class Barrel {
private:
    int x, y;
    int width, height;
    bool isSpecial;
    SDL_Texture* texture;
    
public:
    Barrel(int startX, int startY, SDL_Texture* tex);
    
    void render(SDL_Renderer* renderer, int cameraX, int cameraY);
    
    int getX() { return x; }
    int getY() { return y; }
    SDL_Rect getRect() { return {x, y, width, height}; }
    int getWidth() { return width; }
    int getHeight() { return height; }
    bool getSpecial() { return isSpecial; }
};

#endif
