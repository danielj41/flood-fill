#ifndef MAIN_LEVEL_HPP
#define MAIN_LEVEL_HPP

#include "scene.hpp"
#include "debug_player.hpp"

class MainLevel : public Scene{

public:
    MainLevel();

    void setup();
    void update();

private:
    DebugPlayer * debugPlayer;
};

#endif
