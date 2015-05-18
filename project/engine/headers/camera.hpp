#ifndef CAMERA_H
#define CAMERA_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "object.hpp"

#include "pointer_macros.h"
DEF_PTR(Camera, CameraPtr);

class Camera {
public:
    static const int FORWARD_DIRECTION;
    static const int BACKWARD_DIRECTION;
    static const int LEFT_DIRECTION;
    static const int RIGHT_DIRECTION;

    Camera();
    Camera(glm::vec3 _eye, glm::vec3 _target, glm::vec3 _up);

    glm::mat4 getViewMatrix();
    glm::vec3 getViewVector();
    glm::vec3 getStrafeVector();

    glm::vec3 getEye();
    glm::vec3 getTarget();
    glm::vec3 getUp();

    void setEye(glm::vec3 _eye);
    void setTarget(glm::vec3 _target);
    void setUp(glm::vec3 _up);

    void strafe(int direction, float velocity);
    void zoom(int direction, float velocity);
    void jump(float velocity);

    void setTheta(float degreeAngle);
    void setPhi(float degreeAngle);
    void setAngles(float _theta, float _phi);
    void setProjectionMatrix(glm::mat4 matrix);

    float getTheta();
    float getPhi();
    glm::mat4 getProjectionMatrix();

    bool isReady();
    bool hasProjectionMatrix();

    /* The camera will be fixed in the Axis that you specify
        i.e. if you pass (true, false, true), the camera won't move in X and Z*/
    void fix(bool x, bool y, bool z);
    //Fix the camera position in all Axis
    void fix();
    void unfix();

    bool insideViewFrustum(ObjectPtr object);

private:
    static unsigned int FIXED_AXIS;
    static unsigned int UNFIXED_AXIS;

    glm::vec3 eye, target, up;
    float theta, phi; // Both angles are expressed in degrees
    bool ready; // Flag that says if it is an empty camera or not
    bool projectionMatrixLoaded;
    glm::vec3 fixedAxis;

    glm::mat4 projectionMatrix;

    void updateTarget();
};

#endif
