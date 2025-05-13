#ifndef CLYDE_H
#define CLYDE_H

#include "Ghost.h"

class Clyde : public Ghost {
public:
    Clyde(int x, int y, int w, int h);
    void update(SDL_Rect pacman, int pacmanDirection) override;
protected:
    void chooseDirection(SDL_Rect pacman) override;
};

#endif
