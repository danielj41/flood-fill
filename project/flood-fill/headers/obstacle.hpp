#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"

#include "pointer_macros.h"
DEF_PTR(Obstacle, ObstaclePtr);

class Obstacle : public GameObject, public CollisionObject{

public:
    Obstacle(glm::vec3 _position, glm::vec3 _movementDirection, float _speed);

    void setup();
    void update();

    void collided(CollisionObjectPtr collidedWith);

private:
    ObjectPtr box;
    glm::vec3 position, movementDirection, size;
    float speed;

    bool shrink;
    float shrinkRate;
};

#endif
