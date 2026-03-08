#include "Labirin.h"
#include <iostream>

Labirin::Labirin() {
    width = 30;
    height = 20;
    level = 1;
    
    for (int i = 0; i < 3; i++) wallTextures[i] = nullptr;
    wallLeftTexture = nullptr;
    wallRightTexture = nullptr;
    for (int i = 0; i < 2; i++) floorTextures[i] = nullptr;
    
    startPos = Position(5, 5);
    exitPos = Position(25, 15);
}

Labirin::~Labirin() {}

void Labirin::setTextures(SDL_Texture* w1, SDL_Texture* w2, SDL_Texture* w3,
                          SDL_Texture* wLeft, SDL_Texture* wRight,
                          SDL_Texture* f1, SDL_Texture* f2) {
    wallTextures[0] = w1;
    wallTextures[1] = w2;
    wallTextures[2] = w3;
    wallLeftTexture = wLeft;
    wallRightTexture = wRight;
    floorTextures[0] = f1;
    floorTextures[1] = f2;
}

void Labirin::generate(int newLevel) {
    level = newLevel;
    width = 30 + level * 2;
    height = 20 + level;
    
    grid.resize(height);
    for (int y = 0; y < height; y++) {
        grid[y].resize(width, 0);
    }
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x == 0 || y == 0 || x == width-1 || y == height-1) {
                grid[y][x] = 1;
            } else if (x == 1) {
                grid[y][x] = 2;
            } else if (x == width-2) {
                grid[y][x] = 3;
            } else if (randomInt(0, 100) < 15) {
                grid[y][x] = 1;
            }
        }
    }
    
    startPos = Position(3, 3);
    exitPos = Position(width-4, height-4);
    grid[exitPos.y][exitPos.x] = 0;
}

void Labirin::render(SDL_Renderer* renderer, int cameraX, int cameraY) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int screenX = x * 20 - cameraX;
            int screenY = y * 20 - cameraY;
            
            if (screenX < -20 || screenX > 800 || screenY < -20 || screenY > 600) continue;
            
            SDL_Rect dest = {screenX, screenY, 20, 20};
            
            if (grid[y][x] == 0) {
                int floorVar = randomInt(0, 1);
                if (floorTextures[floorVar])
                    SDL_RenderCopy(renderer, floorTextures[floorVar], NULL, &dest);
            } else if (grid[y][x] == 1) {
                int wallVar = randomInt(0, 2);
                if (wallTextures[wallVar])
                    SDL_RenderCopy(renderer, wallTextures[wallVar], NULL, &dest);
            } else if (grid[y][x] == 2) {
                if (wallLeftTexture) {
                    dest.w = 1;
                    SDL_RenderCopy(renderer, wallLeftTexture, NULL, &dest);
                }
            } else if (grid[y][x] == 3) {
                if (wallRightTexture) {
                    dest.w = 1;
                    SDL_RenderCopy(renderer, wallRightTexture, NULL, &dest);
                }
            }
        }
    }
}

std::vector<SDL_Rect> Labirin::getWalls() {
    std::vector<SDL_Rect> walls;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[y][x] != 0) {
                walls.push_back({x * 20, y * 20, 20, 20});
            }
        }
    }
    
    return walls;
}

std::vector<Position> Labirin::getEnemySpawnPoints() {
    std::vector<Position> points;
    for (int i = 0; i < 3; i++) {
        int x = randomInt(5, width-5);
        int y = randomInt(5, height-5);
        if (grid[y][x] == 0) {
            points.push_back(Position(x * 20, y * 20));
        }
    }
    return points;
}

std::vector<Position> Labirin::getBarrelSpawnPoints() {
    std::vector<Position> points;
    for (int i = 0; i < 5; i++) {
        int x = randomInt(3, width-3);
        int y = randomInt(3, height-3);
        if (grid[y][x] == 0) {
            points.push_back(Position(x * 20, y * 20));
        }
    }
    return points;
}
