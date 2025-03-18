#include <iostream>
#include "include/SDL.h"
#include "include/SDL_image.h"

#undef main

SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
SDL_Texture* texture = nullptr;
SDL_Surface* surface = nullptr;

SDL_Texture* playerTexture = nullptr;
SDL_Rect playerRect = {190, 270, 20, 20};
int playerSpeed = 1;

const int windowWidth = 800;
const int windowHeight = 600;
const int mapWidth = 420;
const int mapHeight = 465;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 

    surface = IMG_Load("assets/map.png");
    if (!surface) {
        std::cerr << "Failed to load image! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    surface = IMG_Load("assets/player.png");
    if (!surface) {
        std::cerr << "Failed to load player image! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }
    playerTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return true;
}

void close() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    
    int x = (windowWidth - mapWidth) / 2;
    int y = (windowHeight - mapHeight) / 2;
    SDL_Rect destRect = {x, y, mapWidth, mapHeight};
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    SDL_RenderCopy(renderer, playerTexture, nullptr, &playerRect);

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    if (!init()) {
        std::cerr << "Failed to initialize!\n";
        return -1;
    }
    bool running = true;
SDL_Event event;

while (running) {
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            running = false;
        } 
        else if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_UP:
                    playerRect.y -= playerSpeed; 
                    break;
                case SDLK_DOWN:
                    playerRect.y += playerSpeed; 
                    break;
                case SDLK_LEFT:
                    playerRect.x -= playerSpeed; 
                    break;
                case SDLK_RIGHT:
                    playerRect.x += playerSpeed; 
                    break;
            }
        }
    }

    render(); 
    SDL_Delay(16); 
}
    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
        SDL_RenderClear(renderer);
        render();
    }

    close();
    return 0;
}
