#include "Game.h"
#include <iostream>

Game::Game() {
    window = nullptr;
    renderer = nullptr;
    state = STATE_MENU;
    running = true;
    currentLevel = 1;
    cameraX = 0;
    cameraY = 0;
    
    for (int i = 0; i < 4; i++) {
        texKunti[i] = nullptr;
        texHead[i] = nullptr;
        for (int j = 0; j < 3; j++) {
            texBody[i][j] = nullptr;
        }
    }
    for (int i = 0; i < 3; i++) texWall[i] = nullptr;
    texWallLeft = nullptr;
    texWallRight = nullptr;
    for (int i = 0; i < 2; i++) texFloor[i] = nullptr;
    texBarrel = nullptr;
    for (int i = 0; i < 2; i++) texJumpscare[i] = nullptr;
}

Game::~Game() {
    clean();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL init gagal: %s\n", SDL_GetError());
        return false;
    }
    
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("SDL_image init gagal: %s\n", IMG_GetError());
        return false;
    }
    
    window = SDL_CreateWindow("WHAT? - Labirin Horor",
                              SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              800, 600, SDL_WINDOW_SHOWN);
    
    if (!window) {
        printf("Window gagal: %s\n", SDL_GetError());
        return false;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer gagal: %s\n", SDL_GetError());
        return false;
    }
    
    if (!loadAllTextures()) {
        printf("Gagal load textures!\n");
        return false;
    }
    
    labirin = std::make_unique<Labirin>();
    labirin->setTextures(texWall[0], texWall[1], texWall[2],
                         texWallLeft, texWallRight,
                         texFloor[0], texFloor[1]);
    
    jumpscare = std::make_unique<Jumpscare>();
    jumpscare->setTextures(texJumpscare[0], texJumpscare[1]);
    
    generateLevel();
    
    return true;
}

bool Game::loadAllTextures() {
    // Load Kunti
    texKunti[DIR_DOWN] = loadTexture(renderer, "assets/enemy/kunti/kunti_down.png");
    texKunti[DIR_UP] = loadTexture(renderer, "assets/enemy/kunti/kunti_up.png");
    texKunti[DIR_LEFT] = loadTexture(renderer, "assets/enemy/kunti/kunti_left.png");
    texKunti[DIR_RIGHT] = loadTexture(renderer, "assets/enemy/kunti/kunti_right.png");
    
    // Load Head
    texHead[DIR_DOWN] = loadTexture(renderer, "assets/player/head/head_down.png");
    texHead[DIR_UP] = loadTexture(renderer, "assets/player/head/head_up.png");
    texHead[DIR_LEFT] = loadTexture(renderer, "assets/player/head/head_left.png");
    texHead[DIR_RIGHT] = loadTexture(renderer, "assets/player/head/head_right.png");
    
    // Load Body
    texBody[DIR_DOWN][0] = loadTexture(renderer, "assets/player/body/down/body_down_1.png");
    texBody[DIR_DOWN][1] = loadTexture(renderer, "assets/player/body/down/body_down_2.png");
    texBody[DIR_DOWN][2] = loadTexture(renderer, "assets/player/body/down/body_down_3.png");
    texBody[DIR_UP][0] = loadTexture(renderer, "assets/player/body/up/body_up_1.png");
    texBody[DIR_UP][1] = loadTexture(renderer, "assets/player/body/up/body_up_2.png");
    texBody[DIR_UP][2] = loadTexture(renderer, "assets/player/body/up/body_up_3.png");
    texBody[DIR_LEFT][0] = loadTexture(renderer, "assets/player/body/left/body_left_1.png");
    texBody[DIR_LEFT][1] = loadTexture(renderer, "assets/player/body/left/body_left_2.png");
    texBody[DIR_LEFT][2] = loadTexture(renderer, "assets/player/body/left/body_left_3.png");
    texBody[DIR_RIGHT][0] = loadTexture(renderer, "assets/player/body/right/body_right_1.png");
    texBody[DIR_RIGHT][1] = loadTexture(renderer, "assets/player/body/right/body_right_2.png");
    texBody[DIR_RIGHT][2] = loadTexture(renderer, "assets/player/body/right/body_right_3.png");
    
    // Load Environment
    texWall[0] = loadTexture(renderer, "assets/environment/walls/wall1.png");
    texWall[1] = loadTexture(renderer, "assets/environment/walls/wall2.png");
    texWall[2] = loadTexture(renderer, "assets/environment/walls/wall3.png");
    texWallLeft = loadTexture(renderer, "assets/environment/walls/wall_left.png");
    texWallRight = loadTexture(renderer, "assets/environment/walls/wall_right.png");
    texFloor[0] = loadTexture(renderer, "assets/environment/floors/floor1.png");
    texFloor[1] = loadTexture(renderer, "assets/environment/floors/floor2.png");
    texBarrel = loadTexture(renderer, "assets/environment/barrel.png");
    
    // Load Jumpscare
    texJumpscare[0] = loadTexture(renderer, "assets/jumpscare/jumpscare_kunti_1.png");
    texJumpscare[1] = loadTexture(renderer, "assets/jumpscare/jumpscare_kunti_2.png");
    
    return true;
}

void Game::generateLevel() {
    labirin->generate(currentLevel);
    
    Position start = labirin->getStartPos();
    player = std::make_unique<Player>(start.x, start.y);
    
    for (int i = 0; i < 4; i++) {
        player->setHeadTexture((Direction)i, texHead[i]);
        for (int j = 0; j < 3; j++) {
            player->setBodyTexture((Direction)i, j, texBody[i][j]);
        }
    }
    
    // Spawn Kunti
    kuntis.clear();
    auto enemyPoints = labirin->getEnemySpawnPoints();
    for (auto& pos : enemyPoints) {
        auto kunti = std::make_unique<Kunti>(pos.x, pos.y);
        for (int i = 0; i < 4; i++) {
            kunti->setTexture((Direction)i, texKunti[i]);
        }
        kuntis.push_back(std::move(kunti));
    }
    
    // Spawn Barrel
    barrels.clear();
    auto barrelPoints = labirin->getBarrelSpawnPoints();
    for (auto& pos : barrelPoints) {
        barrels.push_back(std::make_unique<Barrel>(pos.x, pos.y, texBarrel));
    }
}

void Game::run() {
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(16);
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        }
        
        if (event.type == SDL_KEYDOWN) {
            if (state == STATE_MENU && event.key.keysym.sym == SDLK_SPACE) {
                state = STATE_PLAYING;
            }
            if (state == STATE_GAMEOVER && event.key.keysym.sym == SDLK_r) {
                currentLevel = 1;
                generateLevel();
                state = STATE_PLAYING;
            }
        }
    }
}

void Game::update() {
    if (state != STATE_PLAYING) return;
    
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    
    bool up = keys[SDL_SCANCODE_UP] || keys[SDL_SCANCODE_W];
    bool down = keys[SDL_SCANCODE_DOWN] || keys[SDL_SCANCODE_S];
    bool left = keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A];
    bool right = keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D];
    
    auto walls = labirin->getWalls();
    
    // Convert vector<unique_ptr<Barrel>> to vector<Barrel*>
    std::vector<Barrel*> barrelPtrs;
    for (auto& b : barrels) {
        barrelPtrs.push_back(b.get());
    }
    
    player->update(up, down, left, right, walls, barrelPtrs);
    
    for (auto& kunti : kuntis) {
        kunti->update(*player, walls);
    }
    
    jumpscare->update();
    
    checkCollisions();
    
    cameraX = player->getX() - 400;
    cameraY = player->getY() - 300;
}

void Game::checkCollisions() {
    if (jumpscare->isActive()) return;
    
    for (auto& kunti : kuntis) {
        if (kunti->checkCollision(*player)) {
            jumpscare->trigger();
            player->die();
            break;
        }
    }
    
    Position exit = labirin->getExitPos();
    SDL_Rect exitRect = {exit.x, exit.y, 20, 20};
    SDL_Rect playerRect = player->getRect();
    if (SDL_HasIntersection(&playerRect, &exitRect)) {
        currentLevel++;
        generateLevel();
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    if (state == STATE_MENU) {
        // Menu sederhana
        SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);
        SDL_RenderClear(renderer);
    } 
    else if (state == STATE_PLAYING) {
        labirin->render(renderer, cameraX, cameraY);
        
        for (auto& barrel : barrels) {
            barrel->render(renderer, cameraX, cameraY);
        }
        
        for (auto& kunti : kuntis) {
            kunti->render(renderer, cameraX, cameraY);
        }
        
        player->render(renderer, cameraX, cameraY);
        
        jumpscare->render(renderer);
    }
    else if (state == STATE_GAMEOVER) {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
    }
    
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    for (int i = 0; i < 4; i++) {
        if (texKunti[i]) SDL_DestroyTexture(texKunti[i]);
        if (texHead[i]) SDL_DestroyTexture(texHead[i]);
        for (int j = 0; j < 3; j++) {
            if (texBody[i][j]) SDL_DestroyTexture(texBody[i][j]);
        }
    }
    for (int i = 0; i < 3; i++) {
        if (texWall[i]) SDL_DestroyTexture(texWall[i]);
    }
    if (texWallLeft) SDL_DestroyTexture(texWallLeft);
    if (texWallRight) SDL_DestroyTexture(texWallRight);
    for (int i = 0; i < 2; i++) {
        if (texFloor[i]) SDL_DestroyTexture(texFloor[i]);
    }
    if (texBarrel) SDL_DestroyTexture(texBarrel);
    for (int i = 0; i < 2; i++) {
        if (texJumpscare[i]) SDL_DestroyTexture(texJumpscare[i]);
    }
    
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    
    IMG_Quit();
    SDL_Quit();
}

void Game::goToSafeRoom() {
    state = STATE_SAFEROOM;
}

void Game::gameOver() {
    state = STATE_GAMEOVER;
}
