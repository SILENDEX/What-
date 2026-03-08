#include "Barrel.h"

Barrel::Barrel(int startX, int startY, SDL_Texture* tex) {
    x = startX;
    y = startY;
    width = 10;
    height = 10;
    texture = tex;
    isSpecial = (rand() % 10 == 0);
}

void Barrel::render(SDL_Renderer* renderer, int cameraX, int cameraY) {
    if (!texture) return;
    
    SDL_Rect dest = {x - cameraX, y - cameraY, width, height};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
}
