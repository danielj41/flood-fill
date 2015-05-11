#ifndef ACTIVE_TERRAIN_H
#define ACTIVE_TERRAIN_H

#include <list>

#include "glm/glm.hpp"
#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"
#include "switch.hpp"
#include "solid_cube.hpp"

class ActiveTerrain : public GameObject{

public:
    ActiveTerrain(Switch* _s, glm::vec3 _initialPos, glm::vec3 _finalPos, float _speed);

    void setup();
    void update();
    
private:
    
    std::list<SolidCube*> solidCubes;
    
    Switch *s;
    glm::vec3 direction;
    glm::vec3 position;
    glm::vec3 initialPos;
    glm::vec3 finalPos;
    float speed;
    
    bool active;
};
    
#endif
