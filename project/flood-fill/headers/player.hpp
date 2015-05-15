#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "game_object.hpp"
#include "collision_object.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "player_hand.hpp"
#include "color_change.hpp"
#include "fluid_box.hpp"

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

    float strafeVelocity;
    float forwardVelocity;

    Object *sky;
    Object *gun;
    PlayerHand *hand;
    ColorChange *change;

    bool shootPressed;
    float shootTimer;

    //Fluid Box Remotion Control Variables and Methods
    unsigned int removeFluidShootRange; // How far you can remove the fluid
    unsigned int removeFluidNumberBlocks; // How many boxes you can remove at once
    std::vector<FluidBox *> boxesToRemove;

    void pickFluidBoxesToRemove();
    void pickOneFluidBoxToRemove(glm::vec3 pos);
    void removeFluidBoxes();
};

#endif
