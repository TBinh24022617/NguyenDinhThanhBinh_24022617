#include "SoundManager.h"
#include <iostream>
#include <SDL.h>

Mix_Music* SoundManager::music = nullptr;

void SoundManager::Play(const std::string& path, bool loop) {
    if (music != nullptr) {
        Mix_FreeMusic(music);
        music = nullptr;
    }

    music = Mix_LoadMUS(path.c_str());
    if (music == nullptr) {
        SDL_Log("Failed to load music: %s", Mix_GetError());
        return;
    }

    Mix_PlayMusic(music, loop ? -1 : 1);
}

void SoundManager::Stop() {
    Mix_HaltMusic();
}

void SoundManager::Free() {
    if (music != nullptr) {
        Mix_FreeMusic(music);
        music = nullptr;
    }
}
