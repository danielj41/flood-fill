#ifndef INVISIBLE_WALL_HPP
#define INVISIBLE_WALL_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"

class InvisibleWall : public GameObject, public CollisionObject{

public:
    InvisibleWall(glm::vec3 _position, glm::vec3 _scaleVector);

    void setup();
    void update();

    void collided(CollisionObject *);

private:
    glm::vec3 position, scaleVector;
};

#endif
