#ifndef KUNTI_H
#define KUNTI_H

#include "Enemy.h"
#include <vector>

class Kunti : public Enemy {
private:
    SDL_Texture* textures[4];
    int chaseRange;
    int wanderTimer;
    int wanderDir;
    
public:
    Kunti(int startX, int startY);
    ~Kunti();
    
    void setTexture(Direction dir, SDL_Texture* tex);
    
    virtual void update(const Player& player, const std::vector<SDL_Rect>& walls) override;
    virtual void render(SDL_Renderer* renderer, int cameraX, int cameraY) override;
    
    void respawn(int newX, int newY);
};

#endif
