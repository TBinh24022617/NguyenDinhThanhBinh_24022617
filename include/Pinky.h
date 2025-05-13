#ifndef PINKY_H
#define PINKY_H

#include "Ghost.h"

class Pinky : public Ghost {
public:
    Pinky(int x, int y, int w, int h);
    void update(SDL_Rect pacman, int pacmanDirection) override;
protected:
    void chooseDirection(SDL_Rect pacman) override;
};

#endif
