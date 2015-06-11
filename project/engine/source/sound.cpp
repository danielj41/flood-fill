#include "sound.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

const std::string Sound::INVALID_EXT = "INVALID_EXT";
const std::string Sound::INVALID_PATH = "INVALID_PATH";

Sound::Sound() : path(INVALID_PATH), loaded(false) {}

Sound::Sound(std::string _path): path(_path), loaded(false) {}

void Sound::load(){
    ASSERT(path != INVALID_PATH, "You didn't provide a path for the Sound");

    INFO("Loading Sound " << path << "...");

    ASSERT(buffer.loadFromFile(path), "soundBuffer failed to load"); 
        
    audio.setBuffer(buffer);

    loaded = true;

    INFO("Sound " << path << " loaded!");
}

void Sound::playSound() {
    sf::Sound::Status status = audio.getStatus();
    if (status == sf::Sound::Stopped) 
    audio.play();
// Stopped
// Paused
// Playing  
}  

void Sound::stopSound() {
        audio.stop();
}

void Sound::loopSound() {
    audio.setLoop(true);
}
