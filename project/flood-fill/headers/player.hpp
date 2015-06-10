#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "game_object.hpp"
#include "collision_object.hpp"
#include "camera.hpp"
#include "object.hpp"
#include "player_hand.hpp"
#include "color_change.hpp"
#include "fluid_box.hpp"

#include "pointer_macros.h"
DEF_PTR(Player, PlayerPtr);

class Player: public GameObject, public CollisionObject{

public:
    Player(CameraPtr _camera, int _initialColor);

    void setup();
    void update();

    void collided(CollisionObjectPtr collidedWith);
    bool isKeyPressed(unsigned int key);
    void setActive(bool _active);

private:
    CameraPtr camera;

    glm::vec3 lastPosition;

    bool jumping;
    int ceilingFrame;
    float velocity;
    float gravity;
    float jumpMultiplier;

    float strafeVelocity;
    float forwardVelocity;
    float moveMultiplier;
    
    int initialColor;

    ObjectPtr sky;
    ObjectPtr gun;
    PlayerHandPtr hand;
    ColorChangePtr change;

    bool shootPressed;
    float shootTimer;

    float walkAmount;
    float eyeOffset;

    bool addMultiplier;
    bool active;

    //Fluid Box Remotion Control Variables and Methods
    unsigned int removeFluidShootRange; // How far you can remove the fluid
    unsigned int removeFluidNumberBlocks; // How many boxes you can remove at once
    std::vector<FluidBoxPtr> boxesToRemove;

    void pickFluidBoxesToRemove();
    void pickOneFluidBoxToRemove(glm::vec3 pos);
    void removeFluidBoxes();

    float angleP = 90.0f;
    float angleInc = 0.5f, angledir = 1;
    float anglePMax = 120.0f, anglePMin = 90.0f;
    bool activateCameraWarp = false;

    void doCameraWarp();
};

#endif
