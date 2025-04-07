#include "Inky.h"

Inky::Inky(int x, int y) : Ghost(x, y) {
    speed = 2;
}

void Inky::update(SDL_Rect pacman, int pacmanDirection, SDL_Rect blinkyPos) {
    if (!active) return;

    SDL_Point target = { pacman.x, pacman.y };

    switch (pacmanDirection) {
        case 0: target.x -= 40; break;        
        case 1: target.y -= 40; break;        
        case 2: target.x += 40; break;        
        case 3: target.y += 40; break;        
    }

    int vecX = target.x - blinkyPos.x;
    int vecY = target.y - blinkyPos.y;

    target.x = blinkyPos.x + 2 * vecX;
    target.y = blinkyPos.y + 2 * vecY;

    if (rect.x < target.x) rect.x += speed;
    else if (rect.x > target.x) rect.x -= speed;

    if (rect.y < target.y) rect.y += speed;
    else if (rect.y > target.y) rect.y -= speed;
}