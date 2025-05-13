#include "Ghost.h"
#include "MapUtils.h"

Ghost::Ghost(int x, int y, int w, int h) {
    rect = {x, y, w, h};
    speed = 2;
    active = false;
}

void Ghost::update(SDL_Rect pacman, int pacmanDirection) {
    if (!active) return;

    int nextX = rect.x;
    int nextY = rect.y;

    // Di chuyển theo trục X
    if (rect.x < pacman.x) nextX += speed;
    else if (rect.x > pacman.x) nextX -= speed;

    if (GhostCanMoveTo(nextX, rect.y))
        rect.x = nextX;

    // Di chuyển theo trục Y
    if (rect.y < pacman.y) nextY += speed;
    else if (rect.y > pacman.y) nextY -= speed;

    if (GhostCanMoveTo(rect.x, nextY))
        rect.y = nextY;
}

void Ghost::render(SDL_Renderer* renderer, SDL_Texture* texture) {
    SDL_RenderCopy(renderer, texture, nullptr, &rect);
}

// Hàm Move mới thêm, gọi đơn giản để di chuyển ghost từng bước (ví dụ AI chưa chạy)
void Ghost::Move(const std::vector<std::vector<int>>& mapData) {
    int tileSize = 16;
    int offsetX = (800 - mapData[0].size() * tileSize) / 2;
    int offsetY = (600 - mapData.size() * tileSize) / 2;

    int tileX = (rect.x - offsetX) / tileSize;
    int tileY = (rect.y - offsetY) / tileSize;

    // Di chuyển đơn giản xuống dưới nếu ô tiếp theo là hợp lệ
    int nextTileY = tileY + 1;

    if (nextTileY >= 0 && nextTileY < mapData.size()) {
        int nextTile = mapData[nextTileY][tileX];
        if (nextTile == 136 || nextTile == 176) {
            rect.y += speed;
        }
    }
}

