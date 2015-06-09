#ifndef CINEMATIC_PLAYER_HPP
#define CINEMATIC_PLAYER_HPP

#include "game_object.hpp"
#include "camera.hpp"

#include "pointer_macros.h"
DEF_PTR(CinematicPlayer, CinematicPlayerPtr);

class CinematicPlayer: public GameObject{

public:
    float timer;
    float timedTheta;
    float theta;

    CinematicPlayer(CameraPtr _camera);

    void setup();
    void update();

    bool isActive();

private:
    CameraPtr camera;
    bool active;
    bool mousePressed;
};

#endif
