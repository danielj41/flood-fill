#ifndef PLAYER_HAND_HPP
#define PLAYER_HAND_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"

#include "pointer_macros.h"
DEF_PTR(PlayerHand, PlayerHandPtr);

class PlayerHand : public GameObject, public CollisionObject {
    
public:
    
    PlayerHand(glm::vec3 _position, ObjectPtr _gun, int _initialColor);  
    void setup();
    void update();
    void setPosition(glm::vec3 _position);
    
    void collided(CollisionObjectPtr collidedWith);
    void setColorMask(int color);
    int getColorMask();
    void changeColorMask();
    double getToggleTime();

private:
    
    glm::vec3 position;
    
    ObjectPtr gun;

    int colors[2];
    int numColors;
    int clrndx;
    double toggleTime;
    
};

#endif
