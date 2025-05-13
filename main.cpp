#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include "include/SDL.h"
#include "include/SDL_image.h"
#include "include/SDL_mixer.h"
#include "include/Blinky.h"
#include "include/Pinky.h"
#include "include/Inky.h"
#include "include/Clyde.h"
#include "include/MapUtils.h"
#include "include/SoundManager.h"
#include <unistd.h>
#undef main

using namespace std;

// Define window dimensions
const int windowWidth = 800;
const int windowHeight = 600;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

SDL_Texture* playerTexture = nullptr;
SDL_Texture* screenTexture = nullptr;
SDL_Texture* gameOverTexture = nullptr;  // Texture cho màn hình game over
SDL_Texture* blinkyTexture = nullptr;
SDL_Texture* pinkyTexture = nullptr;
SDL_Texture* inkyTexture = nullptr;
SDL_Texture* clydeTexture = nullptr;
SDL_Texture* tilesetTexture = nullptr;

SDL_Rect playerRect = {0, 0, 16, 16};
int playerSpeed = 2;
double playerAngle = 0;
SDL_RendererFlip playerFlip = SDL_FLIP_NONE;
int pacmanDirection = -1; // -1: không di chuyển, 0: lên, 1: xuống, 2: trái, 3: phải

// Khai báo các ghost
Blinky blinky(0, 0, 16, 16);
Pinky pinky(0, 0, 16, 16);
Inky inky(0, 0, 16, 16);
Clyde clyde(0, 0, 16, 16);

vector<vector<int>> mapData;
int tilesetCols = 0;
bool playerMoved = false;
bool isGameOver = false;  // Biến để kiểm tra trạng thái game over

SDL_Point FindSpawnPosition() {
    int offsetX = (windowWidth - mapData[0].size() * TILE_SIZE) / 2;
    int offsetY = (windowHeight - mapData.size() * TILE_SIZE) / 2;

    for (int y = 0; y < mapData.size(); ++y) {
        for (int x = 0; x < mapData[y].size(); ++x) {
            if (mapData[y][x] == 136)
                return SDL_Point{offsetX + x * TILE_SIZE, offsetY + y * TILE_SIZE};
        }
    }
    return SDL_Point{offsetX, offsetY};
}

SDL_Point FindGhostSpawn(int tileID) {
    int offsetX = (windowWidth - mapData[0].size() * TILE_SIZE) / 2;
    int offsetY = (windowHeight - mapData.size() * TILE_SIZE) / 2;

    for (int y = 0; y < mapData.size(); ++y) {
        for (int x = 0; x < mapData[y].size(); ++x) {
            if (mapData[y][x] == tileID)
                return SDL_Point{offsetX + x * TILE_SIZE, offsetY + y * TILE_SIZE};
        }
    }
    return SDL_Point{offsetX, offsetY};
}

vector<vector<int>> LoadMap(const string& filename) {
    ifstream file(filename);
    vector<vector<int>> result;
    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        vector<int> row;
        string token;
        while (getline(ss, token, ',')) {
            token.erase(remove_if(token.begin(), token.end(), ::isspace), token.end());
            if (!token.empty()) row.push_back(stoi(token));
        }
        if (!row.empty()) result.push_back(row);
    }
    return result;
}

void RenderMap(SDL_Renderer* renderer, SDL_Texture* tileset,
               const vector<vector<int>>& map, int tileSize, int cols) {
    SDL_Rect srcRect, dstRect;
    srcRect.w = srcRect.h = dstRect.w = dstRect.h = tileSize;

    int offsetX = (windowWidth - map[0].size() * tileSize) / 2;
    int offsetY = (windowHeight - map.size() * tileSize) / 2;

    for (int y = 0; y < map.size(); ++y) {
        for (int x = 0; x < map[y].size(); ++x) {
            int idx = map[y][x];
            if (idx < 0) continue;
            srcRect.x = (idx % cols) * tileSize;
            srcRect.y = (idx / cols) * tileSize;
            dstRect.x = offsetX + x * tileSize;
            dstRect.y = offsetY + y * tileSize;
            SDL_RenderCopy(renderer, tileset, &srcRect, &dstRect);
        }
    }
}

bool CanMoveTo(int newX, int newY) {
    int offsetX = (windowWidth - mapData[0].size() * TILE_SIZE) / 2;
    int offsetY = (windowHeight - mapData.size() * TILE_SIZE) / 2;

    int tileX = (newX - offsetX) / TILE_SIZE;
    int tileY = (newY - offsetY) / TILE_SIZE;

    if (tileY >= 0 && tileY < mapData.size() &&
        tileX >= 0 && tileX < mapData[0].size()) {
        int tileID = mapData[tileY][tileX];

        if (tileID == 176) {
            mapData[tileY][tileX] = 136;
            return true;
        }

        return tileID == 136 || tileID == 30 || tileID == 31;
    }
    return false;
}

bool CheckCollisionWithGhosts(SDL_Rect player, SDL_Rect ghost) {
    // Checking if rectangles intersect
    int leftA = player.x;
    int rightA = player.x + player.w;
    int topA = player.y;
    int bottomA = player.y + player.h;

    int leftB = ghost.x;
    int rightB = ghost.x + ghost.w;
    int topB = ghost.y;
    int bottomB = ghost.y + ghost.h;

    // Nếu một trong các điều kiện này đúng, không có va chạm
    if (bottomA <= topB) return false;
    if (topA >= bottomB) return false;
    if (rightA <= leftB) return false;
    if (leftA >= rightB) return false;

    // Nếu không, có va chạm
    return true;
}

void ShowGameOverScreen() {
    // Dừng nhạc hiện tại
    SoundManager::Stop();
    // Phát âm thanh pacman die
    SoundManager::Play("music/pacman-die.mp3", false);

    bool waiting = true;
    SDL_Event event;

    while (waiting) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || 
               (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)) {
                waiting = false;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, gameOverTexture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) return false;
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return false;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) return false;

    window = SDL_CreateWindow("Đi săn hay bị săn", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, windowWidth,
                              windowHeight, SDL_WINDOW_SHOWN);
    if (!window) return false;

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) return false;    SDL_Surface* screenSurface = IMG_Load("assets/Pacmanscreen.png");
    if (!screenSurface) return false;
    screenTexture = SDL_CreateTextureFromSurface(renderer, screenSurface);
    SDL_FreeSurface(screenSurface);

    // Load game over texture
    SDL_Surface* gameOverSurface = IMG_Load("assets/gameover.png");
    if (!gameOverSurface) return false;
    gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    SDL_FreeSurface(gameOverSurface);

    return screenTexture != nullptr && gameOverTexture != nullptr;
}

void close() {
    SoundManager::Free();

    SDL_DestroyTexture(screenTexture);
    SDL_DestroyTexture(tilesetTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(blinkyTexture);
    SDL_DestroyTexture(pinkyTexture);
    SDL_DestroyTexture(inkyTexture);
    SDL_DestroyTexture(clydeTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_CloseAudio();
    IMG_Quit();
    SDL_Quit();
}

void showStartScreen() {
    bool waiting = true;
    SDL_Event event;

    SoundManager::Play("music/screensound.mp3", true);

    while (waiting) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) exit(0);
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN)
                waiting = false;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, screenTexture, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SoundManager::Stop();
    SoundManager::Play("music/playsound.mp3", true);
}

int main(int argc, char* argv[]) {
    if (!init()) {
        std::cerr << "Init failed\n";
        return -1;
    }

    showStartScreen();

    SDL_Surface* surface = IMG_Load("assets/tilesetedit.png");
    tilesetCols = surface->w / TILE_SIZE;
    tilesetTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    mapData = LoadMap("mappacman.csv");
    SetMapData(mapData);

    playerRect = { FindSpawnPosition().x, FindSpawnPosition().y, 16, 16 };
    blinky.rect = { FindGhostSpawn(138).x, FindGhostSpawn(138).y, 16, 16 };
    pinky.rect  = { FindGhostSpawn(139).x, FindGhostSpawn(139).y, 16, 16 };
    inky.rect   = { FindGhostSpawn(159).x, FindGhostSpawn(159).y, 16, 16 };
    clyde.rect  = { FindGhostSpawn(160).x, FindGhostSpawn(160).y, 16, 16 };

    // Kích hoạt các con ma
    blinky.active = true;
    pinky.active = true;
    inky.active = true;
    clyde.active = true;

    auto loadTex = [](const char* path) -> SDL_Texture* {
        std::cerr << "Attempting to load texture from: " << path << std::endl;
        
        SDL_Surface* surf = IMG_Load(path);
        if (!surf) {
            std::cerr << "Failed to load surface: " << IMG_GetError() << std::endl;
            std::cerr << "Current working directory: ";
            char cwd[256];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                std::cerr << cwd << std::endl;
            }
            return nullptr;
        }
        
        std::cerr << "Successfully loaded surface. Size: " << surf->w << "x" << surf->h << std::endl;
        
        SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
        if (!tex) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
            SDL_FreeSurface(surf);
            return nullptr;
        }
        
        std::cerr << "Successfully created texture" << std::endl;
        SDL_FreeSurface(surf);
        return tex;
    };

    // Chỉ thử tải player.png
    playerTexture = loadTex("assets/player.png");
    if (!playerTexture) {
        std::cerr << "Critical error: Could not load player.png" << std::endl;
        // Kiểm tra xem file có tồn tại không
        FILE* file = fopen("assets/player.png", "rb");
        if (file) {
            std::cerr << "File exists but couldn't be loaded as an image" << std::endl;
            fclose(file);
        } else {
            std::cerr << "File does not exist" << std::endl;
        }
        return -1;
    }

    // Debug log for playerRect
    std::cerr << "Initial playerRect: x=" << playerRect.x << ", y=" << playerRect.y << ", w=" << playerRect.w << ", h=" << playerRect.h << "\n";

    blinkyTexture = loadTex("assets/blinky.png");
    pinkyTexture  = loadTex("assets/pinky.png");
    inkyTexture   = loadTex("assets/inky.png");
    clydeTexture  = loadTex("assets/clyde.png");

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) quit = true;
        }

        const Uint8* state = SDL_GetKeyboardState(nullptr);
        playerMoved = false;

        if (state[SDL_SCANCODE_UP] && CanMoveTo(playerRect.x, playerRect.y - playerSpeed)) {
            playerRect.y -= playerSpeed;
            pacmanDirection = 0;
            playerMoved = true;
        } else if (state[SDL_SCANCODE_DOWN] && CanMoveTo(playerRect.x, playerRect.y + playerSpeed)) {
            playerRect.y += playerSpeed;
            pacmanDirection = 1;
            playerMoved = true;
        } else if (state[SDL_SCANCODE_LEFT] && CanMoveTo(playerRect.x - playerSpeed, playerRect.y)) {
            playerRect.x -= playerSpeed;
            pacmanDirection = 2;
            playerMoved = true;
        } else if (state[SDL_SCANCODE_RIGHT] && CanMoveTo(playerRect.x + playerSpeed, playerRect.y)) {
            playerRect.x += playerSpeed;
            pacmanDirection = 3;
            playerMoved = true;
        }

        // Luôn cập nhật các con ma, không phụ thuộc vào việc người chơi di chuyển
        blinky.update(playerRect, pacmanDirection);
        pinky.update(playerRect, pacmanDirection);
        // Inky cần thông tin về vị trí của Blinky
        ((Inky&)inky).updateWithBlinky(playerRect, pacmanDirection, blinky.rect);
        clyde.update(playerRect, pacmanDirection);

        // Kiểm tra va chạm với các con ma
        if (CheckCollisionWithGhosts(playerRect, blinky.rect) ||
            CheckCollisionWithGhosts(playerRect, pinky.rect) ||
            CheckCollisionWithGhosts(playerRect, inky.rect) ||
            CheckCollisionWithGhosts(playerRect, clyde.rect)) {
            isGameOver = true;
        }

        SDL_RenderClear(renderer);
        RenderMap(renderer, tilesetTexture, mapData, TILE_SIZE, tilesetCols);

        if (playerTexture) {
            SDL_RenderCopy(renderer, playerTexture, nullptr, &playerRect);
        } else {
            std::cerr << "Error: playerTexture is null during rendering.\n";
        }

        SDL_RenderCopy(renderer, blinkyTexture, nullptr, &blinky.rect);
        SDL_RenderCopy(renderer, pinkyTexture, nullptr, &pinky.rect);
        SDL_RenderCopy(renderer, inkyTexture, nullptr, &inky.rect);
        SDL_RenderCopy(renderer, clydeTexture, nullptr, &clyde.rect);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);

        if (isGameOver) {
            ShowGameOverScreen();
            quit = true;
        }
    }

    close();
    return 0;
}
