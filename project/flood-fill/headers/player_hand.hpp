#ifndef PLAYER_HAND_HPP
#define PLAYER_HAND_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"

class PlayerHand : public GameObject, public CollisionObject {

public:

  PlayerHand(glm::vec3 _position);

  void setup();
  void update();
  void setPosition(glm::vec3 _position);

  void collided(CollisionObject * collidedWith);

private:
  glm::vec3 position;
};

#endif
