#include "MapUtils.h"
#include <vector>

static std::vector<std::vector<int>> currentMapData;
const int TILE_SIZE = 16; // Đảm bảo TILE_SIZE trùng với main.cpp
const int WALKABLE_TILE_ID = 136;

void SetMapData(const std::vector<std::vector<int>>& data) {
    currentMapData = data;
}

bool GhostCanMoveTo(int newX, int newY) {
    if (currentMapData.empty() || currentMapData[0].empty()) return false;

    int mapWidthInPixels = currentMapData[0].size() * TILE_SIZE;
    int mapHeightInPixels = currentMapData.size() * TILE_SIZE;
    int offsetX = (800 - mapWidthInPixels) / 2;
    int offsetY = (600 - mapHeightInPixels) / 2;

    int tileX = (newX - offsetX) / TILE_SIZE;
    int tileY = (newY - offsetY) / TILE_SIZE;

    if (tileY >= 0 && tileY < currentMapData.size() &&
        tileX >= 0 && tileX < currentMapData[0].size()) {
        return currentMapData[tileY][tileX] == WALKABLE_TILE_ID;
    }

    return false;
}
