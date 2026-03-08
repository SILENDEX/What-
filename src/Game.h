#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>
#include <memory>
#include "Player.h"
#include "Kunti.h"
#include "Barrel.h"
#include "Jumpscare.h"
#include "Labirin.h"

enum GameState {
    STATE_MENU,
    STATE_PLAYING,
    STATE_SAFEROOM,
    STATE_GAMEOVER
};

class Game {
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    GameState state;
    bool running;
    int currentLevel;
    
    // Kamera
    int cameraX, cameraY;
    
    // Game objects
    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Kunti>> kuntis;
    std::vector<std::unique_ptr<Barrel>> barrels;
    std::unique_ptr<Labirin> labirin;
    std::unique_ptr<Jumpscare> jumpscare;
    
    // Textures
    SDL_Texture* texKunti[4];
    SDL_Texture* texHead[4];
    SDL_Texture* texBody[4][3];
    SDL_Texture* texWall[3];
    SDL_Texture* texWallLeft;
    SDL_Texture* texWallRight;
    SDL_Texture* texFloor[2];
    SDL_Texture* texBarrel;
    SDL_Texture* texJumpscare[2];
    
public:
    Game();
    ~Game();
    
    bool init();
    void run();
    void handleEvents();
    void update();
    void render();
    void clean();
    
    bool loadAllTextures();
    void generateLevel();
    void checkCollisions();
    void goToSafeRoom();
    void gameOver();
};

#endif
