#include "Blinky.h"
#include "MapUtils.h"
#include <cmath>

extern const int windowWidth;
extern const int windowHeight;

Blinky::Blinky(int x, int y, int w, int h) : Ghost(x, y, w, h) {
    speed = 2;
    homeCorner.x = windowWidth - 32;
    homeCorner.y = 32;
    releaseTimer = 3000; // 3 giây để ra khỏi nhà
}

void Blinky::update(SDL_Rect pacman, int pacmanDirection) {
    if (!active) return;
    currentPacmanDirection = pacmanDirection;
    Ghost::update(pacman, pacmanDirection);
}

void Blinky::chooseDirection(SDL_Rect pacman) {
    // Blinky luôn đuổi theo thẳng Pacman
    int targetX = pacman.x;
    int targetY = pacman.y;
    
    // Nếu ở chế độ scatter thì về góc nhà
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
