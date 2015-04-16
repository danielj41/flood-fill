#include "debug_player.hpp"

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

DebugPlayer::DebugPlayer(Camera * _camera) : camera(_camera), active(false) {}

void DebugPlayer::setup() {
    INFO("DebugPlayer Setup...");
}

void DebugPlayer::update() {
    if(glfwGetKey(Global::window, GLFW_KEY_0) == GLFW_PRESS){
        INFO("Debug Player activated!");
        active = true;
    }
    else if(glfwGetKey(Global::window, GLFW_KEY_9) == GLFW_PRESS){
        INFO("Debug Player deactivated!");
        active = false;
    }

    if(active){
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
    }
}

void DebugPlayer::draw() {
}

bool DebugPlayer::isActive(){
    return active;
}
