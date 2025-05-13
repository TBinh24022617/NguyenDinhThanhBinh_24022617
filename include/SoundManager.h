#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <string>
#include <SDL_mixer.h>

class SoundManager {
public:
    static void Play(const std::string& path, bool loop);
    static void Stop();
    static void Free();

private:
    static Mix_Music* music;
};

#endif
