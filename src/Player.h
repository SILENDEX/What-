#ifndef PLAYER_H
#define PLAYER_H

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "utils.h"

class Barrel;

class Player {
private:
    int x, y;
    int width, height;
    Direction facing;
    int currentFrame;
    int frameTimer;
    bool isMoving;
    bool alive;
    bool inBarrel;
    int breathTimer;
    int headVariation;
    int bodyVariation;
    SDL_Texture* headTextures[4];
    SDL_Texture* bodyTextures[4][3];
    
public:
    Player(int startX, int startY);
    ~Player();
    
    void setHeadTexture(Direction dir, SDL_Texture* tex);
    void setBodyTexture(Direction dir, int frame, SDL_Texture* tex);
    
    void update(bool up, bool down, bool left, bool right, 
                const std::vector<SDL_Rect>& walls,
                const std::vector<Barrel*>& barrels);
    void render(SDL_Renderer* renderer, int cameraX, int cameraY);
    
    // Getters - dengan const
    int getX() const { return x; }
    int getY() const { return y; }
    bool isAlive() const { return alive; }
    bool isInBarrel() const { return inBarrel; }
    SDL_Rect getRect() const { return {x, y, width, height}; }
    
    void enterBarrel(class Barrel* barrel);
    void exitBarrel();
    void die();
    void respawn(int startX, int startY);
};

#endif
