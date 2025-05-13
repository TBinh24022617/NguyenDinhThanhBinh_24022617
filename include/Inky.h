#ifndef INKY_H
#define INKY_H

#include "Ghost.h"

class Inky : public Ghost {
public:
    Inky(int x, int y, int w, int h);
    void update(SDL_Rect pacman, int pacmanDirection) override;
    void updateWithBlinky(SDL_Rect pacman, int pacmanDirection, SDL_Rect blinkyPos);
protected:
    void chooseDirection(SDL_Rect pacman) override;
    SDL_Rect blinkyPosition;
};

#endif
