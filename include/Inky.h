#ifndef INKY_H
#define INKY_H

#include "Ghost.h"
#include <vector>

class Inky : public Ghost {
public:
    Inky(int x, int y, int w, int h);
    
    // Cập nhật AI của Inky (cần vị trí Blinky)
    void update(SDL_Rect pacman, int pacmanDirection, SDL_Rect blinkyPos);

    // Hàm Move để gọi từ main.cpp
    void Move(const std::vector<std::vector<int>>& mapData) override;
};

#endif
