#ifndef SOLID_CUBE_HPP
#define SOLID_CUBE_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"

class SolidCube : public GameObject, public CollisionObject{

public:
  SolidCube(glm::vec3 _position);

  void setup();
  void update();

  void collided(CollisionObject * collidedWith);

private:
  Object * cube;
  glm::vec3 position;

};

#endif
