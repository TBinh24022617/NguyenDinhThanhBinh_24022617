#include "Clyde.h"
#include <cmath>
#include "MapUtils.h"

Clyde::Clyde(int x, int y, int w, int h) : Ghost(x, y, w, h) {
    speed = 1;
}

void Clyde::update(SDL_Rect pacman, int pacmanDirection) {
    if (!active) return;

    int dx = pacman.x - rect.x;
    int dy = pacman.y - rect.y;
    float distance = std::sqrt(dx * dx + dy * dy);

    int nextX = rect.x;
    int nextY = rect.y;

    if (distance > 100) {
        if (dx != 0) nextX += (dx > 0) ? speed : -speed;
        if (GhostCanMoveTo(nextX, rect.y)) rect.x = nextX;

        if (dy != 0) nextY += (dy > 0) ? speed : -speed;
        if (GhostCanMoveTo(rect.x, nextY)) rect.y = nextY;
    } else {
        // Nếu gần Pac-Man thì chạy về góc dưới bên trái bản đồ
        int tx = 0;
        int ty = 600 - rect.h;

        int txDiff = tx - rect.x;
        int tyDiff = ty - rect.y;

        if (txDiff != 0) nextX += (txDiff > 0) ? speed : -speed;
        if (GhostCanMoveTo(nextX, rect.y)) rect.x = nextX;

        if (tyDiff != 0) nextY += (tyDiff > 0) ? speed : -speed;
        if (GhostCanMoveTo(rect.x, nextY)) rect.y = nextY;
    }
}

// ✅ Hàm Move() đơn giản gọi lại update() để dùng được trong main
void Clyde::Move(const std::vector<std::vector<int>>& mapData) {
    // Không cần dùng mapData ở đây vì đã xử lý va chạm trong GhostCanMoveTo
    // Nếu bạn cần AI phức tạp hơn có thể dùng mapData để tìm đường
    // Tạm thời để Pac-Man đứng yên nếu không được truyền vào
    SDL_Rect dummyPacman = { rect.x, rect.y, rect.w, rect.h };
    update(dummyPacman, 0);
}
