#ifndef BLINKY_H
#define BLINKY_H

#include "Ghost.h"

class Blinky : public Ghost {
public:
    Blinky(int x, int y, int w, int h);
    void update(SDL_Rect pacman, int pacmanDirection) override;
    bool isSpecialGhost() const override { return true; } // Ghi đè method này
protected:
    void chooseDirection(SDL_Rect pacman) override;
};

#endif
