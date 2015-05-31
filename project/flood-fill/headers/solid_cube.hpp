#ifndef SOLID_CUBE_HPP
#define SOLID_CUBE_HPP

#include <set>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"

#include "pointer_macros.h"
DEF_PTR(SolidCube, SolidCubePtr);

class SolidCube : public GameObject, public CollisionObject{

public:
    SolidCube(glm::vec3 _position);
    SolidCube(glm::vec3 _position, std::set<int> _facingFillSpaces);
    
    void setup();
    void update();
    
    void collided(CollisionObjectPtr collidedWith);
    
    ObjectPtr getObject();

    void animateFrom(glm::vec3 from, float amount);

private:
  ObjectPtr cube;
  glm::vec3 position;

  std::set<int> facingFillSpaces;
};

#endif
