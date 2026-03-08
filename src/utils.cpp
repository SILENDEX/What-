#include "utils.h"

SDL_Texture* loadTexture(SDL_Renderer* renderer, std::string path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path.c_str());
    if (!texture) {
        printf("Gagal load texture: %s - %s\n", path.c_str(), IMG_GetError());
    }
    return texture;
}

int randomInt(int min, int max) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(min, max);
    return dist(gen);
}
