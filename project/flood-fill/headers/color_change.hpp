#ifndef COLOR_CHANGE_HPP
#define COLOR_CHANGE_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"

class ColorChange : public GameObject, public CollisionObject{

public:
  ColorChange(glm::vec3 _position, int _colorMask);

  void setup();
  void update();

  void collided(CollisionObject * collidedWith);
  int getColor();

private:

  Object * colorChange;
  glm::vec3 position;
  glm::vec3 size;
  std::string color;

  float timer;
  int colorMask;
};

#endif
