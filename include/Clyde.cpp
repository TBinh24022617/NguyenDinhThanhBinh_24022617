#include "Clyde.h"
#include "MapUtils.h"
#include <cmath>

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

Clyde::Clyde(int x, int y, int w, int h) : Ghost(x, y, w, h) {
    speed = 2;
    homeCorner.x = 32;
    homeCorner.y = windowHeight - 32; // Góc trái dưới
    releaseTimer = 20000; // 20 giây để ra khỏi nhà
}

void Clyde::update(SDL_Rect pacman, int pacmanDirection) {
    if (!active) return;
    currentPacmanDirection = pacmanDirection;
    Ghost::update(pacman, pacmanDirection);
}

void Clyde::chooseDirection(SDL_Rect pacman) {
    int targetX = pacman.x;
    int targetY = pacman.y;
    
    // Tính khoảng cách đến Pacman
    int dx = targetX - rect.x;
    int dy = targetY - rect.y;
    float distance = sqrt(dx*dx + dy*dy);
    
    // Nếu khoảng cách < 8 ô hoặc ở chế độ scatter, đi về góc nhà
    if (distance < 128 || currentState == SCATTER) {
        targetX = homeCorner.x;
        targetY = homeCorner.y;
    }
    
    // Chọn hướng di chuyển dựa trên khoảng cách xa nhất
    dx = targetX - rect.x;
    dy = targetY - rect.y;
    
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
