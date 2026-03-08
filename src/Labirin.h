#ifndef LABIRIN_H
#define LABIRIN_H

#include <SDL2/SDL.h>
#include <vector>
#include "utils.h"

class Labirin {
private:
    int width, height;
    int level;
    std::vector<std::vector<int>> grid;
    Position startPos;
    Position exitPos;
    SDL_Texture* wallTextures[3];
    SDL_Texture* wallLeftTexture;
    SDL_Texture* wallRightTexture;
    SDL_Texture* floorTextures[2];
    
public:
    Labirin();
    ~Labirin();
    
    void setTextures(SDL_Texture* w1, SDL_Texture* w2, SDL_Texture* w3,
                     SDL_Texture* wLeft, SDL_Texture* wRight,
                     SDL_Texture* f1, SDL_Texture* f2);
    
    void generate(int level);
    void render(SDL_Renderer* renderer, int cameraX, int cameraY);
    
    std::vector<SDL_Rect> getWalls();
    Position getStartPos() { return startPos; }
    Position getExitPos() { return exitPos; }
    
    std::vector<Position> getEnemySpawnPoints();
    std::vector<Position> getBarrelSpawnPoints();
};

#endif
