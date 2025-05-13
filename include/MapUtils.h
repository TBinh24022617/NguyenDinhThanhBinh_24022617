#ifndef MAPUTILS_H
#define MAPUTILS_H

#include <vector>

// Global constants
extern const int windowWidth;
extern const int windowHeight;
const int TILE_SIZE = 16;

extern std::vector<std::vector<int>> currentMapData;
void SetMapData(const std::vector<std::vector<int>>& data);
bool GhostCanMoveTo(int newX, int newY);

#endif // MAPUTILS_H
