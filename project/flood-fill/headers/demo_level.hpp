#ifndef DEMO_LEVEL_HPP
#define DEMO_LEVEL_HPP

#include "scene.hpp"
#include "debug_player.hpp"
#include "player.hpp"

class DemoLevel : public Scene{

public:
    DemoLevel();

    void setup();
    void update();

private:
    DebugPlayer * debugPlayer;
    Player * player;

};

#endif
