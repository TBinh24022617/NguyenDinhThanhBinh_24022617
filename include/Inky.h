#ifndef INKY_H
#define INKY_H

#include "Ghost.h"

class Inky : public Ghost {
public:
    Inky(int x, int y);
    void update(SDL_Rect pacman, int pacmanDirection, SDL_Rect blinkyPos);
};

#endif