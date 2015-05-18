#ifndef DEBUG_PLAYER_HPP
#define DEBUG_PLAYER_HPP

#include "game_object.hpp"
#include "camera.hpp"

#include "pointer_macros.h"
DEF_PTR(DebugPlayer, DebugPlayerPtr);

class DebugPlayer: public GameObject{

public:
    DebugPlayer(Camera * _camera);

    void setup();
    void update();

    bool isActive();

private:
    Camera * camera;
    bool active;
};

#endif
