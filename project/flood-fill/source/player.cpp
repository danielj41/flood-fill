#include "player.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GLIncludes.h"
#include "global_variables.hpp"
#include "bounding_box.hpp"
#include "time_manager.hpp"

Player::Player(Camera * _camera)
    : GameObject(), CollisionObject(BoundingBox(glm::vec3(1), glm::vec3(-1))),
        camera(_camera), points(0) {}

void Player::setup() {
    INFO("Player Setup...");

    setCollisionID(2);
    setCollideWithID(1 | 4);

    camera->fix(false, true, false);
}

void Player::update() {
    lastPosition = camera->getEye();

    float cameraSpeed = 5.0f*TimeManager::getDeltaTime();
    if(glfwGetKey(Global::window, GLFW_KEY_W) == GLFW_PRESS){
        camera->zoom(Camera::FORWARD_DIRECTION, cameraSpeed);
    }
    else if(glfwGetKey(Global::window, GLFW_KEY_S) == GLFW_PRESS){
        camera->zoom(Camera::BACKWARD_DIRECTION, cameraSpeed);
    }
    if(glfwGetKey(Global::window, GLFW_KEY_A) == GLFW_PRESS){
        camera->strafe(Camera::LEFT_DIRECTION, cameraSpeed);
    }
    else if(glfwGetKey(Global::window, GLFW_KEY_D) == GLFW_PRESS){
        camera->strafe(Camera::RIGHT_DIRECTION, cameraSpeed);
    }

    boundingBox.loadIdentity();
    boundingBox.translate(camera->getEye());
}

void Player::collided(CollisionObject * collidedWith){
    if(collidedWith->getCollisionID() == 4){
        points++;
    }
    else if(collidedWith->getCollisionID() == 1){
        boundingBox.loadIdentity();
        boundingBox.translate(lastPosition);
        camera->setEye(lastPosition);
    }
}

int Player::getPoints(){
    return points;
}
