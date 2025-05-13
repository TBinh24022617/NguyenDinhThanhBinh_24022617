#ifndef PINKY_H
#define PINKY_H

#include "Ghost.h"

class Pinky : public Ghost {
public:
    Pinky(int x, int y, int w, int h);

    void update(SDL_Rect pacman, int pacmanDirection) override;

    // Thêm hàm Move để gọi được từ main.cpp
    void Move(const std::vector<std::vector<int>>& mapData) override;
};

#endif
