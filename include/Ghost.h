#ifndef GHOST_H
#define GHOST_H

#include <SDL.h>

class Ghost {
public:
    SDL_Rect rect;  
    int speed;
    bool active;

    Ghost(int x, int y);  
    virtual void update(SDL_Rect pacman, int pacmanDirection);
    void render(SDL_Renderer* renderer, SDL_Texture* texture);  
};

#endif