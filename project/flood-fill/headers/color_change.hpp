#ifndef COLOR_CHANGE_HPP
#define COLOR_CHANGE_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"

#include "pointer_macros.h"
DEF_PTR(ColorChange, ColorChangePtr);

class ColorChange : public GameObject, public CollisionObject{

public:
  ColorChange(glm::vec3 _position, int _colorMask);

  void setup();
  void update();

  void collided(CollisionObjectPtr collidedWith);
  int getColor();

private:

  ObjectPtr colorChange;
  glm::vec3 position;
  glm::vec3 size;
  std::string color;

  float timer;
  int colorMask;
};

#endif
