#include "screen.h"
#include <iostream>

bool showScreen(SDL_Renderer* renderer, const std::string& imagePath) {
    SDL_Surface* surface = IMG_Load(imagePath.c_str());
    if (!surface) {
        std::cerr << "Failed to load screen image: " << IMG_GetError() << std::endl;
        return false;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        std::cerr << "Failed to create screen texture: " << SDL_GetError() << std::endl;
        return false;
    }

    SDL_Event e;
    bool waiting = true;

    while (waiting) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                SDL_DestroyTexture(texture);
                return false;
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
                waiting = false;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, nullptr, nullptr); // vẽ ảnh full màn hình
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    return true;
}
