#ifndef ACTION_INVISIBLE_BLOCK_HPP
#define ACTION_INVISIBLE_BLOCK_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"

#include "pointer_macros.h"
DEF_PTR(ActionInvisibleBlock, ActionInvisibleBlockPtr);

class ActionInvisibleBlock : public GameObject, public CollisionObject{

public:
    ActionInvisibleBlock(glm::vec3 _position);

    virtual void setup();
    virtual void update();

    virtual void setCollisionIDs() = 0;
    virtual void doAction() = 0;

    virtual void collided(CollisionObjectPtr collidedWith);

protected:
    glm::vec3 position;
};

#endif
