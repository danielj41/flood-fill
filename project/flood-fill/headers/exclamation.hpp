#ifndef EXCLAMATION_HPP
#define EXCLAMATION_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "object.hpp"

#include "pointer_macros.h"
DEF_PTR(Exclamation, ExclamationPtr);

class Exclamation : public GameObject {
    
public:
    Exclamation(glm::vec3 _position);
    
    void setup();
    void update();
    
private:
    ObjectPtr exclamation1;
    ObjectPtr exclamation2;
    glm::vec3 position;
    glm::vec3 size;

    float timer;
};

#endif
