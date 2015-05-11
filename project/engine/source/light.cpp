#include "light.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

const int Light::DIRECTIONAL = 1;
const int Light::POINT_LIGHT = 2;
const int Light::SPOT_LIGHT = 3;
const int Light::NOT_VALID_LIGHT = -1;

Light::Light(): type(NOT_VALID_LIGHT), ON(false){}

//Directional Light Constructor
Light::Light(glm::vec3 _color, float _intensity, glm::vec3 _direction)
    : type(DIRECTIONAL), color(_color), direction(_direction),
        intensity(_intensity), ON(true){}

//Point Light Constructor
Light::Light(glm::vec3 _color, float _intensity, glm::vec3 _position,
        glm::vec3 _fallOff)
    : type(POINT_LIGHT), color(_color), position(_position), fallOff(_fallOff),
        intensity(_intensity), ON(true){}

//Spot Light Constructor
Light::Light(glm::vec3 _color, float _intensity, glm::vec3 _position,
        glm::vec3 _direction, glm::vec3 _fallOff, float _cutOffAngle)
    : type(SPOT_LIGHT), color(_color),
        position(_position), direction(_direction), fallOff(_fallOff),
        intensity(_intensity), cutOffAngle(_cutOffAngle), ON(true){}

glm::vec3 Light::getPosition(){
    ASSERT(isValid(), "This light is not a valid light");
    return position;
}

glm::vec3 Light::getColor(){
    ASSERT(isValid(), "This light is not a valid light");
    return color;
}

glm::vec3 Light::getDirection(){
    ASSERT(isValid(), "This light is not a valid light");
    ASSERT(!isPointLight(),
        "Point Lights does not have direction");
    return direction;
}

glm::vec3 Light::getFallOff(){
    ASSERT(isValid(), "This light is not a valid light");
    ASSERT(!isDirectional(),
        "Directional Lights does not have a fall off");
    return fallOff;
}

float Light::getIntensity(){
    ASSERT(isValid(), "This light is not a valid light");
    return intensity;
}

float Light::getCutOffAngle(){
    ASSERT(isValid(), "This light is not a valid light");
    ASSERT(!isDirectional(),
        "Directional Lights does not have a cut off angle");
    ASSERT(!isPointLight(),
        "Point Lights does not have a cut off angle");
    return cutOffAngle;
}

void Light::setPosition(glm::vec3 _position){
    ASSERT(isValid(), "This light is not a valid light");
    position = _position;
}

void Light::setColor(glm::vec3 _color){
    ASSERT(isValid(), "This light is not a valid light");
    color = _color;
}

void Light::setDirection(glm::vec3 _direction){
    ASSERT(isValid(), "This light is not a valid light");
    ASSERT(!isPointLight(),
        "Point Lights does not have direction");
    direction = _direction;
}

void Light::setFallOff(glm::vec3 _fallOff){
    ASSERT(isValid(), "This light is not a valid light");
    ASSERT(!isDirectional(),
        "Directional Lights does not have a fall off");
    fallOff = _fallOff;
}

void Light::setIntensity(float _intensity){
    ASSERT(isValid(), "This light is not a valid light");
    intensity = _intensity;
}

void Light::setCutOffAngle(float _cutOffAngle){
    ASSERT(isValid(), "This light is not a valid light");
    ASSERT(!isDirectional(),
        "Directional Lights does not have a cut off angle");
    ASSERT(!isPointLight(),
        "Point Lights does not have a cut off angle");
    cutOffAngle = _cutOffAngle;
}

bool Light::isDirectional(){
    return type == DIRECTIONAL;
}

bool Light::isPointLight(){
    return type == POINT_LIGHT;
}

bool Light::isSpotLight(){
    return type == SPOT_LIGHT;
}

bool Light::isValid(){
    return type != NOT_VALID_LIGHT;
}

bool Light::isON(){
    return ON == true;
}

bool Light::isOFF(){
    return ON == false;
}

void Light::turnON(){
    ON = true;
}

void Light::turnOFF(){
    ON = false;
}
