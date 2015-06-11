#include "exclamation.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#include "load_manager.hpp"
#include "time_manager.hpp"
#include "director.hpp"
#include "material_manager.hpp"
#include "render_engine.hpp"

Exclamation::Exclamation(glm::vec3 _position)
    : GameObject(), position(_position), size(glm::vec3(1)), timer(0) {}

void Exclamation::setup() {

    // . of !
    exclamation1 = ObjectPtr(new Object(LoadManager::getMesh("cube.obj"),
                                       MaterialManager::getMaterial("Black")));
    
    RenderEngine::getRenderElement("regular")->addObject(exclamation1);

    exclamation1->scale(glm::vec3(.5));
    exclamation1->translate(position);

    // | of !
    exclamation2 = ObjectPtr(new Object(LoadManager::getMesh("cube.obj"),
                                       MaterialManager::getMaterial("Black")));
    
    RenderEngine::getRenderElement("regular")->addObject(exclamation2);

    exclamation2->scale(glm::vec3(.5, 1.5, .5));
    exclamation2->translate(position);
}

void Exclamation::update() {
    timer += TimeManager::getDeltaTime();
    
    exclamation1->loadIdentity();
    exclamation1->scale(glm::vec3(.5));
    exclamation1->translate(glm::vec3(position.x, position.y + cos(timer), position.z));
    

    exclamation2->loadIdentity();
    exclamation2->scale(glm::vec3(.5, 1.5, .5));
    exclamation2->translate(glm::vec3(position.x, position.y + cos(timer) + 2.5, position.z));  
}
