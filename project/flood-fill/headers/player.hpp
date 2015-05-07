#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "game_object.hpp"
#include "collision_object.hpp"
#include "camera.hpp"
#include "object.hpp"

class Player: public GameObject, public CollisionObject{

public:
    Player(Camera * _camera);

    void setup();
    void update();

    void collided(CollisionObject * collidedWith);

private:
    Camera * camera;

    glm::vec3 lastPosition;
    
    bool jumping;
    float velocity;
    float gravity;

    Object *sky;
    
    bool shootPressed;
};

#endif
