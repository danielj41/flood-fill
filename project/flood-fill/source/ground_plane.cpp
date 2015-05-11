#include "ground_plane.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "load_manager.hpp"
#include "material_manager.hpp"
#include "render_engine.hpp"

GroundPlane::GroundPlane() : GameObject(){}

void GroundPlane::setup(){
    INFO("Creating the ground plane...");

    plane = new Object(
                    LoadManager::getMesh("plane.obj"),
                    MaterialManager::getMaterial("FlatGrey"));
    

	plane->translate(glm::vec3(0, 0, 0));
	plane->rotate(90.0f, glm::vec3(-1, 0, 0));
    plane->scale(glm::vec3(1, 1, 3));


    RenderEngine::getRenderElement("regular")->addObject(plane);
}

void GroundPlane::update(){
}

