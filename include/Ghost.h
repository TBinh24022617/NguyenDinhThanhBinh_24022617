#ifndef GHOST_H
#define GHOST_H

#include <SDL.h>
#include <vector>

class Ghost {
public:
    SDL_Rect rect;  
    int speed;
    bool active;

    Ghost(int x, int y, int w, int h);
    virtual void update(SDL_Rect pacman, int pacmanDirection);
    void render(SDL_Renderer* renderer, SDL_Texture* texture);  

    SDL_Rect getRect() const { return rect; }

    // Hàm mới thêm để cho phép ghost di chuyển vào ô hợp lệ
    virtual void Move(const std::vector<std::vector<int>>& mapData);
};

#endif
