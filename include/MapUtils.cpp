#include "MapUtils.h"
#include <vector>
#include <cmath>

std::vector<std::vector<int>> currentMapData;

void SetMapData(const std::vector<std::vector<int>>& data) {
    currentMapData = data;
}

bool GhostCanMoveTo(int x, int y) {
    int offsetX = (windowWidth - currentMapData[0].size() * TILE_SIZE) / 2;
    int offsetY = (windowHeight - currentMapData.size() * TILE_SIZE) / 2;

    int tileX = (x - offsetX) / TILE_SIZE;
    int tileY = (y - offsetY) / TILE_SIZE;

    if (tileY >= 0 && tileY < currentMapData.size() &&
        tileX >= 0 && tileX < currentMapData[0].size()) {
        int tileID = currentMapData[tileY][tileX];
        return tileID == 136 || tileID == 30 || tileID == 31 || tileID == 176;
    }
    return false;
}
