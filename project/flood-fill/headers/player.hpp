#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "game_object.hpp"
#include "collision_object.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "player_hand.hpp"
#include "color_change.hpp"


class Player: public GameObject, public CollisionObject{

public:
    Player(Camera * _camera);

    void setup();
    void update();

    void collided(CollisionObject * collidedWith);
    bool isKeyPressed(unsigned int key);

private:
    Camera * camera;

    glm::vec3 lastPosition;
    
    bool jumping;
    float velocity;
    float gravity;

    int colorMask;

    Object *sky;
    Object *gun;
    PlayerHand *hand;
    ColorChange *change;
    
    bool shootPressed;
    float shootTimer;
};

#endif
