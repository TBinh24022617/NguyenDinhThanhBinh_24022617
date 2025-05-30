#include "Pinky.h"
#include "MapUtils.h"
#include <cmath>

extern const int WINDOW_WIDTH;
extern const int WINDOW_HEIGHT;

Pinky::Pinky(int x, int y, int w, int h) : Ghost(x, y, w, h) {
    speed = 2;
    homeCorner.x = 32;
    homeCorner.y = 32;
    releaseTimer = 7000;
}

void Pinky::update(SDL_Rect pacman, int pacmanDirection) {
    if (!active) return;
    currentPacmanDirection = pacmanDirection;
    Ghost::update(pacman, pacmanDirection);
}

void Pinky::chooseDirection(SDL_Rect pacman) {
    // Pinky nhắm tới điểm trước mặt Pacman
    int targetX = pacman.x;
    int targetY = pacman.y;
    
    // Dự đoán vị trí phía trước Pacman
    switch (currentPacmanDirection) {
        case 0: targetY -= 64; break; // Lên
        case 1: targetY += 64; break; // Xuống
        case 2: targetX -= 64; break; // Trái
        case 3: targetX += 64; break; // Phải
    }
    
    int dx = targetX - rect.x;
    int dy = targetY - rect.y;
    
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

