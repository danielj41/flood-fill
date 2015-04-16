#include "camera.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "glm/gtc/matrix_transform.hpp"

#define degreesToRadians(x) (x*(3.141592f/180.0f))


const int Camera::FORWARD_DIRECTION  = -1;
const int Camera::BACKWARD_DIRECTION =  1;
const int Camera::LEFT_DIRECTION     =  1;
const int Camera::RIGHT_DIRECTION    = -1;
unsigned int Camera::FIXED_AXIS = 0;
unsigned int Camera::UNFIXED_AXIS = 1;

Camera::Camera() : ready(false) {
    DEBUG("Empty Camera!");
}

Camera::Camera(glm::vec3 _eye, glm::vec3 _target, glm::vec3 _up)
    : eye(_eye), target(_target), up(_up), theta(-90.0f), phi(0.0f), ready(true),
    projectionMatrixLoaded(false), fixedAxis(glm::vec3(UNFIXED_AXIS)){}

glm::mat4 Camera::getViewMatrix(){
    ASSERT(isReady(), "The camera base vectors missing!");

    updateTarget();
    return lookAt(eye, target, up);
}

glm::vec3 Camera::getViewVector(){
    ASSERT(isReady(), "The camera base vectors missing!");

    return eye - target;
}

glm::vec3 Camera::getStrafeVector(){
    ASSERT(isReady(), "The camera base vectors missing!");

    return cross(getViewVector(), up);
}

glm::vec3 Camera::getEye(){
    ASSERT(isReady(), "The camera base vectors missing!");

    return eye;
}

glm::vec3 Camera::getTarget(){
    ASSERT(isReady(), "The camera base vectors missing!");

    return target;
}

glm::vec3 Camera::getUp(){
    ASSERT(isReady(), "The camera base vectors missing!");

    return up;
}

void Camera::setEye(glm::vec3 _eye){
    eye = _eye;
}

void Camera::setTarget(glm::vec3 _target){
    target = _target;
}

void Camera::setUp(glm::vec3 _up){
    up = _up;
}

void Camera::strafe(int direction, float velocity){
    ASSERT(isReady(), "The camera base vectors missing!");

    updateTarget();

    INFO("Strafing the camera...");
    float dir = (float)direction;

    eye    += fixedAxis*getStrafeVector()*velocity*dir;
    target += fixedAxis*getStrafeVector()*velocity*dir;
}

void Camera::zoom(int direction, float velocity){
    ASSERT(isReady(), "The camera base vectors missing!");

    updateTarget();

    INFO("Zooming the camera...");
    float dir = (float)direction;

    eye    += fixedAxis*getViewVector()*velocity*dir;
    target += fixedAxis*getViewVector()*velocity*dir;
}

void Camera::setTheta(float degreeAngle){
    theta = degreeAngle;
}

void Camera::setPhi(float degreeAngle){
    phi = degreeAngle;
}

void Camera::setAngles(float _theta, float _phi){
    theta = _theta;
    phi = _phi;
}

void Camera::setProjectionMatrix(glm::mat4 matrix){
    INFO("Setting projection matrix...");
    projectionMatrix = matrix;
    projectionMatrixLoaded = true;
}

float Camera::getTheta(){
    return theta;
}

float Camera::getPhi(){
    return phi;
}

glm::mat4 Camera::getProjectionMatrix(){
    ASSERT(hasProjectionMatrix(),
            "This camera does not have a projection matrix.");
    return projectionMatrix;
}

bool Camera::isReady(){
    return ready;
}

bool Camera::hasProjectionMatrix(){
    return projectionMatrixLoaded;
}

void Camera::fix(bool x, bool y, bool z){
    INFO("Camera fixed!");
    fixedAxis = glm::vec3(UNFIXED_AXIS);

    if(x) fixedAxis.x = FIXED_AXIS;
    if(y) fixedAxis.y = FIXED_AXIS;
    if(z) fixedAxis.z = FIXED_AXIS;
}

void Camera::fix(){
    INFO("Camera fixed!");
    fixedAxis = glm::vec3(FIXED_AXIS);
}

void Camera::unfix(){
    INFO("Camera unfixed!");
    fixedAxis = glm::vec3(UNFIXED_AXIS);
}

/** Private Methods **/

void Camera::updateTarget(){
    ASSERT(isReady(), "The camera base vectors missing!");

    //INFO("Updating the look at point...")

    target.x = cos(degreesToRadians(phi))*cos(degreesToRadians(theta)) + eye.x;
    target.y = sin(degreesToRadians(phi)) + eye.y;
    target.z = cos(degreesToRadians(phi))*cos(degreesToRadians(90 - theta)) + eye.z;
}
