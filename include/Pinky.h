#ifndef PINKY_H
#define PINKY_H

#include "Ghost.h"  // Kế thừa từ lớp Ghost

class Pinky : public Ghost {
public:
    Pinky(int x, int y);  // Constructor để khởi tạo vị trí của Pinky
    void update(SDL_Rect pacman, int pacmanDirection) override;  // Cập nhật hành vi của Pinky
};

#endif