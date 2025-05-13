#ifndef MAPUTILS_H
#define MAPUTILS_H

#include <vector>

extern std::vector<std::vector<int>> mapData;

// Hàm này sẽ được gọi để truyền mapData vào
void SetMapData(const std::vector<std::vector<int>>& data);

// Hàm kiểm tra liệu Ghost có thể di chuyển tới vị trí mới không
bool GhostCanMoveTo(int newX, int newY);

#endif // MAPUTILS_H
