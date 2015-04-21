#ifndef FLUID_PROJECTILE_HPP
#define FLUID_PROJECTILE_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"

class FluidProjectile : public GameObject, public CollisionObject{

public:

  FluidProjectile(glm::vec3 _position, glm::vec3 _movementDirection);

  void setup();
  void update();

  void collided(CollisionObject * collidedWith);

private:

  Object * fluidProjectile;
  glm::vec3 position;
  glm::vec3 oldPosition;
  glm::vec3 origPosition;
  glm::vec3 movementDirection; 
  glm::vec3 size;
  
};

#endif
