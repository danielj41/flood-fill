#ifndef GROUND_PLANE_HPP
#define GROUND_PLANE_HPP

#include "game_object.hpp"
#include "object.hpp"

#include "pointer_macros.h"
DEF_PTR(GroundPlane, GroundPlanePtr);

class GroundPlane : public GameObject{

public:
    GroundPlane();

    void setup();
    void update();

private:
    ObjectPtr plane;
};

#endif
