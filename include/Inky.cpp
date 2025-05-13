#include "Inky.h"
#include "MapUtils.h"

Inky::Inky(int x, int y, int w, int h) : Ghost(x, y, w, h) {
    speed = 2;
}

// Hàm update gốc sử dụng vị trí pacman + blinky
void Inky::update(SDL_Rect pacman, int pacmanDirection, SDL_Rect blinkyPos) {
    if (!active) return;

    SDL_Point target = { pacman.x, pacman.y };

    switch (pacmanDirection) {
        case 0: target.y -= 40; break; // Lên
        case 1: target.y += 40; break; // Xuống
        case 2: target.x -= 40; break; // Trái
        case 3: target.x += 40; break; // Phải
    }

    int vecX = target.x - blinkyPos.x;
    int vecY = target.y - blinkyPos.y;

    target.x = blinkyPos.x + 2 * vecX;
    target.y = blinkyPos.y + 2 * vecY;

    int nextX = rect.x;
    int nextY = rect.y;

    if (rect.x < target.x) nextX += speed;
    else if (rect.x > target.x) nextX -= speed;
    if (GhostCanMoveTo(nextX, rect.y)) rect.x = nextX;

    if (rect.y < target.y) nextY += speed;
    else if (rect.y > target.y) nextY -= speed;
    if (GhostCanMoveTo(rect.x, nextY)) rect.y = nextY;
}

// Hàm Move được gọi từ main.cpp
void Inky::Move(const std::vector<std::vector<int>>& mapData) {
    // ⚠️ Nếu không có blinky thật, ta dùng vị trí giả tạm thời
    SDL_Rect fakeBlinky = {200, 200, rect.w, rect.h};

    // Giả định pacman và hướng pacman (0 = lên)
    SDL_Rect fakePacman = {300, 300, rect.w, rect.h};
    int pacmanDir = 0;

    update(fakePacman, pacmanDir, fakeBlinky);
}
