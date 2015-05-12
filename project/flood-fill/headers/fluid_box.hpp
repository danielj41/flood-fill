#ifndef FLUID_BOX_HPP
#define FLUID_BOX_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"

class FluidBox : public GameObject, public CollisionObject{

public:
  FluidBox(glm::vec3 _position);
  FluidBox(glm::vec3 _position, int _colorMask);

  void setup();
  void update();

  void collided(CollisionObject * collidedWith);

private:

  Object * fluidBox;
  glm::vec3 position;
  glm::vec3 size;
  int colorMask;
  std::string color;
  float timer;
  bool visible;
  bool deleting;
};

#endif
