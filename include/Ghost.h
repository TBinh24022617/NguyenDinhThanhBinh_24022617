#ifndef GHOST_H
#define GHOST_H

#include <SDL.h>
#include <vector>

class Ghost {
public:
    SDL_Rect rect;  
    int speed;
    bool active;
    int currentDirection; // 0: up, 1: down, 2: left, 3: right

    enum State {
        SCATTER,    // Ma đi đến góc của nó
        CHASE,      // Ma đuổi theo Pacman
        FRIGHTENED  // Ma sợ hãi và chạy ngược lại
    };

    Ghost(int x, int y, int w, int h);
    virtual void update(SDL_Rect pacman, int pacmanDirection);
    void render(SDL_Renderer* renderer, SDL_Texture* texture);  
    SDL_Rect getRect() const { return rect; }
    virtual bool isSpecialGhost() const { return false; }
    
protected:
    virtual void chooseDirection(SDL_Rect pacman);
    State currentState;
    SDL_Point homeCorner;  // Góc của mỗi con ma
    int lastUpdateTime;
    int stateTimer;
    bool inScatterMode;
    const int UPDATE_INTERVAL = 50;
    const int SCATTER_DURATION = 7000;
    int currentPacmanDirection;
    bool isInHouse;
    int releaseTimer;
    
    void updateState();
    bool canMove(int x, int y);
    void reverseDirection();
};

#endif
