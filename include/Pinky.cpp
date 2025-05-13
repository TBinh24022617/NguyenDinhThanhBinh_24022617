#include "Pinky.h"
#include "MapUtils.h"

// ✅ Gọi constructor cha Ghost
Pinky::Pinky(int x, int y, int w, int h) : Ghost(x, y, w, h) {}

void Pinky::update(SDL_Rect pacman, int pacmanDirection) {
    if (!active) return;

    int targetX = pacman.x;
    int targetY = pacman.y;
    int offset = 4 * 16;  // 4 tile phía trước Pac-Man

    // Tính vị trí mục tiêu dựa theo hướng Pac-Man đang đi
    switch (pacmanDirection) {
        case 0: targetY -= offset; break;  // Lên
        case 1: targetY += offset; break;  // Xuống
        case 2: targetX -= offset; break;  // Trái
        case 3: targetX += offset; break;  // Phải
    }

    int nextX = rect.x;
    int nextY = rect.y;

    // Di chuyển theo X
    if (rect.x < targetX) nextX += speed;
    else if (rect.x > targetX) nextX -= speed;
    if (GhostCanMoveTo(nextX, rect.y)) rect.x = nextX;

    // Di chuyển theo Y
    if (rect.y < targetY) nextY += speed;
    else if (rect.y > targetY) nextY -= speed;
    if (GhostCanMoveTo(rect.x, nextY)) rect.y = nextY;
}

// ✅ Bổ sung hàm Move để tương thích với main.cpp
void Pinky::Move(const std::vector<std::vector<int>>& mapData) {
    SetMapData(mapData);  // Cập nhật map cho hàm GhostCanMoveTo sử dụng
}

