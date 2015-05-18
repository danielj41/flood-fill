#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "pointer_macros.h"
DEF_PTR(GameObject, GameObjectPtr);

class GameObject{
public:
    GameObject();

    virtual void setup() = 0;
    virtual void update() = 0;
};

#endif
