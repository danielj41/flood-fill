#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "game_object.hpp"
#include "collision_object.hpp"
#include "camera.hpp"

class Player: public GameObject, public CollisionObject{

public:
    Player(Camera * _camera);

    void setup();
    void update();

    void collided(CollisionObject * collidedWith);

private:
    Camera * camera;

    glm::vec3 lastPosition;
};

#endif
