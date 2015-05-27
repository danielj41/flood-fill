#ifndef FLUID_BOX_HPP
#define FLUID_BOX_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"

#include "pointer_macros.h"
DEF_PTR(FluidBox, FluidBoxPtr);

class FluidBox : public GameObject, public CollisionObject{

public:
  FluidBox(glm::vec3 _position);
  FluidBox(glm::vec3 _position, int _colorMask);

  void setup();
  void update();

  void collided(CollisionObjectPtr collidedWith);
  int getColorMask();

  //Remotion related methods
  void remove();
  void removeNow();
  void removeAtAdd();
  void highlightForRemotion();
  void deselect();

private:
  glm::vec3 position;
  int colorMask;
  glm::vec3 size;

  ObjectPtr fluidBox;
  std::string color;
  float timer;
  bool visible;

  //Remotion related private fields and methods
  bool deleting;
  bool removeLater;

  void remotionAnimation();
};

#endif
