#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "include/SDL.h"
#include "include/SDL_image.h"
#include "Ghost.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"

#undef main
using namespace std;

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;

SDL_Texture* playerTexture = nullptr;
SDL_Rect playerRect = {190, 270, 20, 20};
int playerSpeed = 3;
double playerAngle = 0;
SDL_RendererFlip playerFlip = SDL_FLIP_NONE;

SDL_Texture* blinkyTexture = nullptr;
SDL_Texture* pinkyTexture = nullptr;
SDL_Texture* inkyTexture = nullptr;
SDL_Texture* clydeTexture = nullptr;
SDL_Texture* tilesetTexture = nullptr;

Ghost blinky((800 - 20) / 2, (600 - 20) / 2);
Pinky pinky((800 - 20) / 2, (600 - 20) / 2);
Inky inky((800 - 20) / 2, (600 - 20) / 2);
Clyde clyde((800 - 20) / 2, (600 - 20) / 2);

const int windowWidth  = 800;
const int windowHeight = 600;
const int TILE_SIZE    = 8;

vector<vector<int>> mapData;
int tilesetCols = 0;

bool playerMoved = false;

vector<vector<int>> LoadMap(const string& filename) {
    ifstream file(filename);
    vector<vector<int>> result;
    string line;
    while (getline(file, line)) {
        istringstream ss(line);
        vector<int> row;
        string token;
        while (ss >> token) {
            row.push_back(stoi(token, nullptr, 16));
        }
        result.push_back(row);
    }
    return result;
}

void RenderMap(SDL_Renderer* renderer, SDL_Texture* tileset,
               const vector<vector<int>>& map, int tileSize, int cols) {
    SDL_Rect srcRect, dstRect;
    srcRect.w = srcRect.h = dstRect.w = dstRect.h = tileSize;

    int offsetX = (windowWidth  - map[0].size() * tileSize) / 2;
    int offsetY = (windowHeight - map.size()   * tileSize) / 2;

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

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        cerr << "SDL could not initialize! " << SDL_GetError() << endl;
        return false;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        cerr << "SDL_image could not initialize! " << IMG_GetError() << endl;
        return false;
    }

    window = SDL_CreateWindow("\u0110i s\u0103n hay b\u1ecb s\u0103n",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        cerr << "Window creation failed! " << SDL_GetError() << endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        cerr << "Renderer creation failed! " << SDL_GetError() << endl;
        return false;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    SDL_Surface* surface = IMG_Load("assets/tilesetpacman2.png");
    if (!surface) {
        cerr << "Failed to load tileset! " << IMG_GetError() << endl;
        return false;
    }
    tilesetCols    = surface->w / TILE_SIZE;
    tilesetTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    mapData = LoadMap("map.txt");

    surface = IMG_Load("assets/player.png");
    if (!surface) { cerr << "player.png load failed! " << IMG_GetError() << endl; return false; }
    playerTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("assets/blinky.png");
    if (!surface) { cerr << "blinky.png load failed! " << IMG_GetError() << endl; return false; }
    blinkyTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("assets/pinky.png");
    if (!surface) { cerr << "pinky.png load failed! " << IMG_GetError() << endl; return false; }
    pinkyTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("assets/inky.png");
    if (!surface) { cerr << "inky.png load failed! " << IMG_GetError() << endl; return false; }
    inkyTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("assets/clyde.png");
    if (!surface) { cerr << "clyde.png load failed! " << IMG_GetError() << endl; return false; }
    clydeTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return true;
}

void close() {
    SDL_DestroyTexture(tilesetTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(blinkyTexture);
    SDL_DestroyTexture(pinkyTexture);
    SDL_DestroyTexture(inkyTexture);
    SDL_DestroyTexture(clydeTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void render() {
    SDL_RenderClear(renderer);

    RenderMap(renderer, tilesetTexture, mapData, TILE_SIZE, tilesetCols);

    SDL_RenderCopyEx(renderer, playerTexture, nullptr, &playerRect,
                     playerAngle, nullptr, playerFlip);

    blinky.render(renderer, blinkyTexture);
    pinky.render(renderer, pinkyTexture);
    inky.render(renderer, inkyTexture);
    clyde.render(renderer, clydeTexture);

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    if (!init()) {
        cerr << "Failed to initialize!\n";
        system("pause");
        return -1;
    }

    bool running = true;
    SDL_Event event;
    int pacmanDirection = 0;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        playerRect.y -= playerSpeed;
                        playerAngle = 270; pacmanDirection = 0; playerFlip = SDL_FLIP_NONE; playerMoved = true;
                        break;
                    case SDLK_DOWN:
                        playerRect.y += playerSpeed;
                        playerAngle = 90;  pacmanDirection = 1; playerFlip = SDL_FLIP_NONE; playerMoved = true;
                        break;
                    case SDLK_LEFT:
                        playerRect.x -= playerSpeed;
                        playerAngle = 0;   pacmanDirection = 2; playerFlip = SDL_FLIP_VERTICAL; playerMoved = true;
                        break;
                    case SDLK_RIGHT:
                        playerRect.x += playerSpeed;
                        playerAngle = 0;   pacmanDirection = 3; playerFlip = SDL_FLIP_NONE; playerMoved = true;
                        break;
                }
                if (playerMoved) {
                    blinky.active = pinky.active = inky.active = clyde.active = true;
                }
            }
        }

        if (blinky.active) blinky.update(playerRect, pacmanDirection);
        if (pinky.active)  pinky.update(playerRect, pacmanDirection);
        if (inky.active)   inky.update(playerRect, pacmanDirection, blinky.rect);
        if (clyde.active)  clyde.update(playerRect, pacmanDirection);

        render();
    }

    close();
    return 0;
}