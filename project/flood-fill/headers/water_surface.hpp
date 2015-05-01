#ifndef WATER_SURFACE_HPP
#define WATER_SURFACE_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"

class WaterSurface : public GameObject, public CollisionObject{

public:

  WaterSurface(glm::vec3 _position);

  void setup();
  void update();

  void collided(CollisionObject * collidedWith);

private:

  Object * waterSurface;
  glm::vec3 position;
  glm::vec3 movementDirection; 
  glm::vec3 size;
  
  float speed;

};

#endif
