#include "camera.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "glm/gtc/matrix_transform.hpp"


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

void Camera::jump(float velocity) {
    ASSERT(isReady(), "The camera base vectors missing!");
    updateTarget();

    INFO("Jumping..." );    
    eye    += up*velocity;
    // target += up*velocity;
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

const int POSITVE_SIDE = 1;
const int NEGATVE_SIDE = -1;

int getPlaneSide(glm::vec4 plane, glm::vec4 point){
    float distanceFromPlane = glm::dot(plane, point);

    if(distanceFromPlane < 0){
        return NEGATVE_SIDE;
    }
    else{
        return POSITVE_SIDE;
    }
}

bool Camera::insideViewFrustum(Object * object){
    glm::mat4 MVP = projectionMatrix*getViewMatrix()*object->getModelMatrix();

    glm::vec4 planes[6] = {
        //Left Plane
        glm::vec4(MVP[0][3] + MVP[0][0],
                  MVP[1][3] + MVP[1][0],
                  MVP[2][3] + MVP[2][0],
                  MVP[3][3] + MVP[3][0]),

        //Right Plane
        glm::vec4(MVP[0][3] - MVP[0][0],
                  MVP[1][3] - MVP[1][0],
                  MVP[2][3] - MVP[2][0],
                  MVP[3][3] - MVP[3][0]),

        //Bottom Plane
        glm::vec4(MVP[0][3] + MVP[0][1],
                  MVP[1][3] + MVP[1][1],
                  MVP[2][3] + MVP[2][1],
                  MVP[3][3] + MVP[3][1]),

        //Top Plane
        glm::vec4(MVP[0][3] - MVP[0][1],
                  MVP[1][3] - MVP[1][1],
                  MVP[2][3] - MVP[2][1],
                  MVP[3][3] - MVP[3][1]),

        //Near Plane
        glm::vec4(MVP[0][3] + MVP[0][2],
                  MVP[1][3] + MVP[1][2],
                  MVP[2][3] + MVP[2][2],
                  MVP[3][3] + MVP[3][2]),

        //Top Plane
        glm::vec4(MVP[0][3] - MVP[0][2],
                  MVP[1][3] - MVP[1][2],
                  MVP[2][3] - MVP[2][2],
                  MVP[3][3] - MVP[3][2])
    };

    glm::vec3 min = object->getMesh()->getMinLimits();
    glm::vec3 max = object->getMesh()->getMaxLimits();

    glm::vec4 vertices[8];
    vertices[0] = glm::vec4(max.x, min.y, max.z, 1);
    vertices[1] = glm::vec4(max.x, max.y, max.z, 1);
    vertices[2] = glm::vec4(min.x, max.y, max.z, 1);
    vertices[3] = glm::vec4(min.x, min.y, max.z, 1);

    vertices[4] = glm::vec4(max.x, min.y, min.z, 1);
    vertices[5] = glm::vec4(max.x, max.y, min.z, 1);
    vertices[6] = glm::vec4(min.x, max.y, min.z, 1);
    vertices[7] = glm::vec4(min.x, min.y, min.z, 1);

    for(int i = 0; i < 8; i++){
        int pointsInside = 0;
        for(int j = 0; j < 6; j+=2){
            if(getPlaneSide(planes[j + 0], vertices[i]) ==
               getPlaneSide(planes[j + 1], vertices[i])){
               pointsInside++;
            }
            else
                break;
        }
        if(pointsInside == 3){
            return true;
        }
    }

    return false;
}

/** Private Methods **/

void Camera::updateTarget(){
    ASSERT(isReady(), "The camera base vectors missing!");

    target.x = cos(glm::radians(phi))*cos(glm::radians(theta)) + eye.x;
    target.y = sin(glm::radians(phi)) + eye.y;
    target.z = cos(glm::radians(phi))*cos(glm::radians(90 - theta)) + eye.z;
}
