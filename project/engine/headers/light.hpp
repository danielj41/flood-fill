#ifndef LIGHT_HPP
#define LIGHT_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

class Light {

public:
    static const int DIRECTIONAL;
    static const int POINT_LIGHT;
    static const int SPOT_LIGHT;

    Light();

    //Directional Light Constructor
    Light(glm::vec3 _color, float _intensity, glm::vec3 _direction);

    //Point Light Constructor
    Light(glm::vec3 _color, float _intensity, glm::vec3 _position,
            glm::vec3 _fallOff);

    //Spot Light Constructor
    Light(glm::vec3 _color, float _intensity, glm::vec3 _position,
            glm::vec3 _direction, glm::vec3 _fallOff, float _cutOffAngle);

    glm::vec3 getPosition();
    glm::vec3 getColor();
    glm::vec3 getDirection();
    glm::vec3 getFallOff();
    float getIntensity();
    float getCutOffAngle();
    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    void setPosition(glm::vec3 _position);
    void setColor(glm::vec3 _color);
    void setDirection(glm::vec3 _direction);
    void setFallOff(glm::vec3 _fallOff);
    void setIntensity(float _intensity);
    void setCutOffAngle(float _cutOffAngle);
    void setViewMatrix(glm::mat4 matrix);
    void setProjectionMatrix(glm::mat4 matrix);

    bool isDirectional();
    bool isPointLight();
    bool isSpotLight();

    bool isON();
    bool isOFF();
    void turnON();
    void turnOFF();

private:
    static const int NOT_VALID_LIGHT;

    int type;
    glm::vec3 color, position, direction, fallOff;
    float intensity, cutOffAngle;
    bool ON;

    glm::mat4 view, projection;

    bool isValid(); // Test if the light type is not NOT_VALID_LIGHT
};

#endif
