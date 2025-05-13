#include "Ghost.h"
#include "MapUtils.h"
#include <cmath>
#include <algorithm>

Ghost::Ghost(int x, int y, int w, int h) {
    rect = {x, y, w, h};
    speed = 1;
    active = false;
    currentDirection = 3;
    currentState = SCATTER;
    lastUpdateTime = SDL_GetTicks();
    stateTimer = SDL_GetTicks();
    inScatterMode = true;
    isInHouse = true;
    releaseTimer = 5000;
    homeCorner = {0, 0};
}

bool Ghost::canMove(int x, int y) {
    // Check boundaries first
    int offsetX = (windowWidth - currentMapData[0].size() * TILE_SIZE) / 2;
    int offsetY = (windowHeight - currentMapData.size() * TILE_SIZE) / 2;
    
    // Make sure we don't go outside the map boundaries
    if (x < offsetX || x >= offsetX + currentMapData[0].size() * TILE_SIZE ||
        y < offsetY || y >= offsetY + currentMapData.size() * TILE_SIZE) {
        return false;
    }

    return GhostCanMoveTo(x, y);
}

void Ghost::reverseDirection() {
    switch (currentDirection) {
        case 0: currentDirection = 1; break;
        case 1: currentDirection = 0; break;
        case 2: currentDirection = 3; break;
        case 3: currentDirection = 2; break;
    }
}

void Ghost::updateState() {
    int currentTime = SDL_GetTicks();

    if (currentTime - stateTimer > SCATTER_DURATION) {
        if (currentState == SCATTER) {
            currentState = CHASE;
            inScatterMode = false;
        } else if (currentState == CHASE) {
            currentState = SCATTER;
            inScatterMode = true;
        }
        stateTimer = currentTime;
        reverseDirection();
    }

    if (isInHouse) {
        releaseTimer -= currentTime - lastUpdateTime;
        if (isSpecialGhost()) {
            releaseTimer = std::min(releaseTimer, 3000);
        }
        if (releaseTimer <= 0) {
            isInHouse = false;
        }
    }

    lastUpdateTime = currentTime;
}

void Ghost::chooseDirection(SDL_Rect pacman) {
    if (isInHouse) return;

    SDL_Point target;
    if (inScatterMode) {
        target = homeCorner;
    } else {
        target = {pacman.x, pacman.y};
    }

    // Tính toán khoảng cách đến đích cho mỗi ngã rẽ có thể
    struct PathOption {
        int direction;
        double distance;
        bool isIntersection;
    };
    std::vector<PathOption> options;

    // Kiểm tra các hướng có thể đi được
    for (int dir = 0; dir < 4; dir++) {
        // Không đi ngược lại
        if ((dir == 0 && currentDirection == 1) ||
            (dir == 1 && currentDirection == 0) ||
            (dir == 2 && currentDirection == 3) ||
            (dir == 3 && currentDirection == 2)) {
            continue;
        }

        int testX = rect.x;
        int testY = rect.y;

        switch (dir) {
            case 0: testY -= speed; break; // Up
            case 1: testY += speed; break; // Down
            case 2: testX -= speed; break; // Left
            case 3: testX += speed; break; // Right
        }

        if (canMove(testX, testY)) {
            // Kiểm tra xem đây có phải là ngã rẽ không
            bool isIntersection = false;
            int availablePaths = 0;
            
            // Đếm số hướng có thể đi từ vị trí tiếp theo
            if (dir != 1 && canMove(testX, testY - speed)) availablePaths++;
            if (dir != 0 && canMove(testX, testY + speed)) availablePaths++;
            if (dir != 3 && canMove(testX - speed, testY)) availablePaths++;
            if (dir != 2 && canMove(testX + speed, testY)) availablePaths++;
            
            isIntersection = (availablePaths > 1);

            // Tính điểm cho hướng này
            double distance = std::hypot(testX - target.x, testY - target.y);
            
            // Nếu là ngã rẽ, giảm khoảng cách một chút để ưu tiên rẽ
            if (isIntersection) {
                distance *= 0.9;
            }
            
            options.push_back({dir, distance, isIntersection});
        }
    }

    if (!options.empty()) {
        // Sắp xếp các lựa chọn theo khoảng cách
        std::sort(options.begin(), options.end(),
                 [](const PathOption& a, const PathOption& b) {
                     return a.distance < b.distance;
                 });

        // Nếu đang ở ngã rẽ và có nhiều lựa chọn
        if (options.size() > 1) {
            // Có 30% cơ hội chọn hướng thứ hai nếu nó cũng là ngã rẽ
            if (options[1].isIntersection && (rand() % 100) < 30) {
                currentDirection = options[1].direction;
                return;
            }
        }

        // Chọn hướng có khoảng cách ngắn nhất
        currentDirection = options[0].direction;
    }
}

void Ghost::update(SDL_Rect pacman, int pacmanDirection) {
    if (!active) return;
    
    updateState();
    currentPacmanDirection = pacmanDirection;

    // Tính toán vị trí tiếp theo
    int nextX = rect.x;
    int nextY = rect.y;
    
    switch (currentDirection) {
        case 0: nextY -= speed; break;
        case 1: nextY += speed; break;
        case 2: nextX -= speed; break;
        case 3: nextX += speed; break;
    }

    // Kiểm tra xem có thể di chuyển tiếp không
    if (canMove(nextX, nextY)) {
        rect.x = nextX;
        rect.y = nextY;
    } else {
        // Nếu không thể di chuyển, tìm hướng mới ngay lập tức
        chooseDirection(pacman);
        
        // Thử di chuyển theo hướng mới
        nextX = rect.x;
        nextY = rect.y;
        switch (currentDirection) {
            case 0: nextY -= speed; break;
            case 1: nextY += speed; break;
            case 2: nextX -= speed; break;
            case 3: nextX += speed; break;
        }
        
        if (canMove(nextX, nextY)) {
            rect.x = nextX;
            rect.y = nextY;
        }
    }

    // Cập nhật hướng định kỳ để tìm đường tốt hơn
    if (SDL_GetTicks() - lastUpdateTime > UPDATE_INTERVAL) {
        chooseDirection(pacman);
        lastUpdateTime = SDL_GetTicks();
    }
}

void Ghost::render(SDL_Renderer* renderer, SDL_Texture* texture) {
    if (!active) return;
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

