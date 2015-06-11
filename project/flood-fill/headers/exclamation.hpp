#ifndef EXCLAMATION_HPP
#define EXCLAMATION_HPP

#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "object.hpp"
#include "material.hpp"

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

    float rotationAngle, rotationAngleIncrement;

    int materialIndex;
    double changeMaterialTime, coolDownMaterialTime;
    bool useBlackMaterial;
    std::vector<MaterialPtr> availableMaterials;
    MaterialPtr getNextMaterial();
};

#endif
