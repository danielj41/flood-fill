#ifndef SOUND_HPP
#define SOUND_HPP

#include <string>
//#include <SFML/Audio.hpp>
#include "GLIncludes.h"

#include "pointer_macros.h"
DEF_PTR(Sound, SoundPtr);

class Sound{
public:
    Sound();
    Sound(std::string _path);

    void load();
    void playSound();
    void stopSound();

private:
    //sf::SoundBuffer buffer;
    //sf::Sound audio;

    static const std::string INVALID_EXT;
    static const std::string INVALID_PATH;

    bool loaded;
    std::string path;
};

#endif
