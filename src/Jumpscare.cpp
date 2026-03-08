#include "Jumpscare.h"

Jumpscare::Jumpscare() {
    normalTex = nullptr;
    invertTex = nullptr;
    active = false;
    timer = 0;
}

Jumpscare::~Jumpscare() {}

void Jumpscare::setTextures(SDL_Texture* normal, SDL_Texture* invert) {
    normalTex = normal;
    invertTex = invert;
}

void Jumpscare::trigger() {
    active = true;
    timer = 20;  // 20 frame kedip-kedip
}

void Jumpscare::update() {
    if (active) {
        timer--;
        if (timer <= 0) {
            active = false;
        }
    }
}

void Jumpscare::render(SDL_Renderer* renderer) {
    if (!active) return;
    
    SDL_Texture* tex = (timer % 4 < 2) ? normalTex : invertTex;
    
    if (tex) {
        int w, h;
        SDL_QueryTexture(tex, NULL, NULL, &w, &h);
        
        SDL_Rect dest = {
            400 - w/2,
            300 - h/2,
            w, h
        };
        
        SDL_RenderCopy(renderer, tex, NULL, &dest);
    }
}
