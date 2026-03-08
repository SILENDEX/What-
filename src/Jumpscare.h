#ifndef JUMPSCARE_H
#define JUMPSCARE_H

#include <SDL2/SDL.h>

class Jumpscare {
private:
    SDL_Texture* normalTex;
    SDL_Texture* invertTex;
    bool active;
    int timer;
    
public:
    Jumpscare();
    ~Jumpscare();
    
    void setTextures(SDL_Texture* normal, SDL_Texture* invert);
    void trigger();
    void update();
    void render(SDL_Renderer* renderer);
    bool isActive() { return active; }
};

#endif
