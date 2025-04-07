#ifndef CLYDE_H
#define CLYDE_H

#include "Ghost.h"

class Clyde : public Ghost {
public:
    Clyde(int x, int y);
    void update(SDL_Rect pacman, int pacmanDirection) override;
};

#endif
