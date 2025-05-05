#include "Ghost.h"

Ghost::Ghost(int x, int y) {
    rect = {x, y, 20, 20};  
    speed = 2;
    active = false;
}

void Ghost::update(SDL_Rect pacman,int pacmanDirection) {
    if (!active) return;

    if (rect.x < pacman.x) rect.x += speed;
    else if (rect.x > pacman.x) rect.x -= speed;

    if (rect.y < pacman.y) rect.y += speed;
    else if (rect.y > pacman.y) rect.y -= speed;
}


void Ghost::render(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_RenderCopy(renderer, texture, NULL, &rect);
}