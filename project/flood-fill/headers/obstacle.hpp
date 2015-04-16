#ifndef OBSTACLE_HPP
#define OBSTACLE_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"

class Obstacle : public GameObject, public CollisionObject{

public:
    Obstacle(glm::vec3 _position, glm::vec3 _movementDirection, float _speed);

    void setup();
    void update();
    void draw();

    void collided(CollisionObject * collidedWith);

private:
    Object * box;
    glm::vec3 movementDirection, position, size;
    float speed;

    bool shrink;
    float shrinkRate;
};

#endif
