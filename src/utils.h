#ifndef UTILS_H
#define UTILS_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <random>

// Hanya deklarasi fungsi
SDL_Texture* loadTexture(SDL_Renderer* renderer, std::string path);
int randomInt(int min, int max);

enum Direction {
    DIR_DOWN,
    DIR_UP,
    DIR_LEFT,
    DIR_RIGHT
};

struct Position {
    int x, y;
    Position(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

#endif
