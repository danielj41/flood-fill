#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

class Material{

public:
    Material();

    //Material without emission color constructor
    Material(glm::vec3 _ambientColor, glm::vec3 _diffuseColor,
            glm::vec3 _specularColor, float _shininess);

    //Material with emission color constructor
    Material(glm::vec3 _ambientColor, glm::vec3 _diffuseColor,
            glm::vec3 _specularColor, glm::vec3 _emissionColor,
            float _shininess);

    glm::vec3 getAmbientColor();
    glm::vec3 getDiffuseColor();
    glm::vec3 getSpecularColor();
    glm::vec3 getEmissionColor();
    float getShininess();

    void setAmbientColor(glm::vec3 _ambientColor);
    void setDiffuseColor(glm::vec3 _diffuseColor);
    void setSpecularColor(glm::vec3 _specularColor);
    void setEmissionColor(glm::vec3 _emissionColor);
    void setShininess(float _shininess);

private:
    glm::vec3 ambientColor, diffuseColor, specularColor, emissionColor;
    float shininess;
};

#endif
