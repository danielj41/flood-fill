#include "material.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

Material::Material(){}

Material::Material(glm::vec3 _ambientColor, glm::vec3 _diffuseColor,
        glm::vec3 _specularColor, float _shininess)
        : ambientColor(_ambientColor), diffuseColor(_diffuseColor),
          specularColor(_specularColor), emissionColor(glm::vec3(0)),
          shininess(_shininess){}

Material::Material(glm::vec3 _ambientColor, glm::vec3 _diffuseColor,
        glm::vec3 _specularColor, glm::vec3 _emissionColor, float _shininess)
        : ambientColor(_ambientColor), diffuseColor(_diffuseColor),
          specularColor(_specularColor), emissionColor(_emissionColor),
          shininess(_shininess){}


glm::vec3 Material::getAmbientColor(){
    return ambientColor;
}

glm::vec3 Material::getDiffuseColor(){
    return diffuseColor;
}

glm::vec3 Material::getSpecularColor(){
    return specularColor;
}

glm::vec3 Material::getEmissionColor(){
    return emissionColor;
}

float Material::getShininess(){
    return shininess;
}

void Material::setAmbientColor(glm::vec3 _ambientColor){
    ambientColor = _ambientColor;
}

void Material::setDiffuseColor(glm::vec3 _diffuseColor){
    diffuseColor = _diffuseColor;
}

void Material::setSpecularColor(glm::vec3 _specularColor){
    specularColor = _specularColor;
}

void Material::setEmissionColor(glm::vec3 _emissionColor){
    emissionColor = _emissionColor;
}

void Material::setShininess(float _shininess){
    shininess = _shininess;
}
