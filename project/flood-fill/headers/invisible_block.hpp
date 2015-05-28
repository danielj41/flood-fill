#ifndef INVISIBLE_BLOCK_HPP
#define INVISIBLE_BLOCK_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"

#include "pointer_macros.h"
DEF_PTR(InvisibleBlock, InvisibleBlockPtr);

class InvisibleBlock : public GameObject, public CollisionObject{

public:
    InvisibleBlock(glm::vec3 _position);
    
    void setup();
    void update();
    
    void collided(CollisionObjectPtr collidedWith);
    

private:
  glm::vec3 position;

};

#endif
