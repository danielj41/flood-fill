#ifndef SWITCH_HPP
#define SWITCH_HPP

#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"
#include "player.hpp"

#include "pointer_macros.h"
DEF_PTR(Switch, SwitchPtr);

class Switch: public GameObject, public CollisionObject{

public:
    
    Switch(glm::vec3 _scale,glm::vec3 _position, glm::vec3 _rotation, float _offsetAngle, int _direction);

    void setup();
    void update();

    void collided(CollisionObjectPtr collideWith);

    bool isOn();

private:

    ObjectPtr handle;
    ObjectPtr shaft;
        
    glm::vec3 scale;
    glm::vec3 position;
    glm::vec3 rotation;
    float offsetAngle;

    int direction;    

    bool triggered;
    bool on;

    float theta;
    float amplitude;
    float speed;


};

#endif
