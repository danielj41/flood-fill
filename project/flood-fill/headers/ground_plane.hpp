#ifndef GROUND_PLANE_HPP
#define GROUND_PLANE_HPP

#include "game_object.hpp"
#include "object.hpp"

class GroundPlane : public GameObject{

public:
    GroundPlane();

    void setup();
    void update();

private:
    Object * plane;
};

#endif
