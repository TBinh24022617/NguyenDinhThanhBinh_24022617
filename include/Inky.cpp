#include "Inky.h"
#include "MapUtils.h"
#include <cmath>

extern const int windowWidth;
extern const int windowHeight;

Inky::Inky(int x, int y, int w, int h) : Ghost(x, y, w, h) {
    speed = 2;
    homeCorner.x = windowWidth - 32;
    homeCorner.y = windowHeight - 32;
    releaseTimer = 15000; // 15 giây để ra khỏi nhà
}

void Inky::update(SDL_Rect pacman, int pacmanDirection) {
    if (!active) return;
    currentPacmanDirection = pacmanDirection;
    Ghost::update(pacman, pacmanDirection);
}

void Inky::updateWithBlinky(SDL_Rect pacman, int pacmanDirection, SDL_Rect blinkyPos) {
    if (!active) return;
    blinkyPosition = blinkyPos;
    update(pacman, pacmanDirection);
}

void Inky::chooseDirection(SDL_Rect pacman) {
    // Inky nhắm tới điểm cách 2 ô phía trước Pacman
    int targetX = pacman.x;
    int targetY = pacman.y;
    
    // Điều chỉnh target dựa trên hướng của Pacman
    switch (currentPacmanDirection) {
        case 0: targetY -= 96; break; // Lên
        case 1: targetY += 96; break; // Xuống
        case 2: targetX -= 96; break; // Trái
        case 3: targetX += 96; break; // Phải
    }
    
    // Nếu ở chế độ scatter, đi về góc
    if (currentState == SCATTER) {
        targetX = homeCorner.x;
        targetY = homeCorner.y;
    }
    
    int dx = targetX - rect.x;
    int dy = targetY - rect.y;
    
    // Chọn hướng di chuyển dựa trên khoảng cách xa nhất
    if (abs(dx) > abs(dy)) {
        currentDirection = (dx > 0) ? 3 : 2;
        if (!canMove(rect.x + (currentDirection == 3 ? speed : -speed), rect.y)) {
            currentDirection = (dy > 0) ? 1 : 0;
        }
    } else {
        currentDirection = (dy > 0) ? 1 : 0;
        if (!canMove(rect.x, rect.y + (currentDirection == 1 ? speed : -speed))) {
            currentDirection = (dx > 0) ? 3 : 2;
        }
    }
}
