#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

class Camera {
public:
    static const int UP_DIRECTION;
    static const int DOWN_DIRECTION;
    static const int LEFT_DIRECTION;
    static const int RIGHT_DIRECTION;

    Camera();
    Camera(glm::vec3 _eye, glm::vec3 _target, glm::vec3 _up);

    glm::mat4 getViewMatrix();
    glm::vec3 getViewVector();
    glm::vec3 getStrafeVector();

    void strafe(int direction, float velocity);
    void zoom(int direction, float velocity);

    void setTheta(float degreeAngle);
    void setPhi(float degreeAngle);
    void setProjectionMatrix(glm::mat4 matrix);

    float getTheta();
    float getPhi();
    glm::mat4 getProjectionMatrix();

    bool isReady();
    bool hasProjectionMatrix();

private:
    float theta, phi; // Both angles are expressed in degrees
    glm::vec3 eye, target, up;
    glm::mat4 projectionMatrix;

    bool ready; // Flag that says if it is an empty camera or not
    bool projectionMatrixLoaded;

    void updateTarget();
};

#endif
