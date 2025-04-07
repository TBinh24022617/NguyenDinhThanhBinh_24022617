#include "Clyde.h"
#include <cmath>

Clyde::Clyde(int x, int y) : Ghost(x, y) {
    speed = 1; // Tốc độ mặc định
}

void Clyde::update(SDL_Rect pacman, int pacmanDirection) {
    int dx = pacman.x - rect.x;
    int dy = pacman.y - rect.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 100) {
        if (dx != 0) rect.x += (dx > 0) ? speed : -speed;
        if (dy != 0) rect.y += (dy > 0) ? speed : -speed;
    } else {
       
        int tx = 0;
        int ty = 600 - rect.h;

        int txDiff = tx - rect.x;
        int tyDiff = ty - rect.y;

        if (txDiff != 0) rect.x += (txDiff > 0) ? speed : -speed;
        if (tyDiff != 0) rect.y += (tyDiff > 0) ? speed : -speed;
    }
}
