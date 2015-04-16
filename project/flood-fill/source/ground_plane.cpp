#include "ground_plane.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "load_manager.hpp"

GroundPlane::GroundPlane() : GameObject(){}

void GroundPlane::setup(){
    INFO("Creating the ground plane...");

    plane = new Object(
                    LoadManager::getMesh("plane.obj"),
                    LoadManager::getShader("vertex.glsl", "fragment.glsl"));
    plane->rotate(90.0f, glm::vec3(1, 0, 0));
    plane->scale(glm::vec3(30, 1, 30));
}

void GroundPlane::update(){
}

void GroundPlane::draw(){
    plane->draw();
}
