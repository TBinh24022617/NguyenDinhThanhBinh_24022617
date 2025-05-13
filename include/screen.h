#ifndef SCREEN_H
#define SCREEN_H

#include "SDL.h"
#include "SDL_image.h"
#include <string>

// Hiển thị màn hình chờ, trả về true nếu nhấn Enter, false nếu thoát
bool showScreen(SDL_Renderer* renderer, const std::string& imagePath);

#endif
