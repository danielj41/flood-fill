#include "cinematic_player.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GLIncludes.h"
#include "global_variables.hpp"
#include "time_manager.hpp"

CinematicPlayer::CinematicPlayer(CameraPtr _camera)
    : camera(_camera), active(true), timer(0.0), timedTheta(0.0), mousePressed(true) {}

void CinematicPlayer::setup() {
    INFO("CinematicPlayer Setup...");
    theta = camera->getTheta();
}

void CinematicPlayer::update() {
    if(glfwGetMouseButton(Global::window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE && mousePressed) {
        mousePressed = false;
    }

    timer += TimeManager::getDeltaTime();
    if(glfwGetKey(Global::window, GLFW_KEY_9) == GLFW_PRESS ||
       (glfwGetMouseButton(Global::window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && !mousePressed) ||
       timer >= 30.0){
        INFO("Cinematic Player deactivated!");
        active = false;
    }

    if(active){
        float cameraSpeed = 5.0f*TimeManager::getDeltaTime();
        timedTheta += TimeManager::getDeltaTime()/10;
        camera->setTheta(theta + theta*timedTheta);
        camera->strafe(Camera::RIGHT_DIRECTION, cameraSpeed);

    }
}

bool CinematicPlayer::isActive(){
    return active;
}
