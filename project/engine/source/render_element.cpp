#include "render_element.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "director.hpp"

RenderElement::RenderElement(bool viewFrustumCullingEnable) : cull(viewFrustumCullingEnable) {}

void RenderElement::setup() {}

void RenderElement::addObject(Object * object){
    INFO("Adding object to the Rendering Element...");

    objects[object->getMesh()].push_front(object);
}

void RenderElement::removeObject(Object * object){
    INFO("Removing object from Rendering Element...");

    Mesh * mesh = object->getMesh();

    ASSERT(objects.find(mesh) != objects.end(), "Mesh does not exist!");

    for(auto it = objects[mesh].begin() ; it != objects[mesh].end(); it++){
        if(*it == object){
            objects[mesh].remove(*it);
            INFO("Object Removed!");
            return;
        }
    }

    DEBUG("Could not remove object!");
}

void RenderElement::setupEnviroment() {}
void RenderElement::tearDownEnviroment() {}

void RenderElement::renderPass(){
    INFO("Render Pass: render all objects");

    Camera * camera = Director::getScene()->getCamera();

    if(objects.size() == 0) return;

    INFO("Setup Shader...");
    setupShader();

    for(auto it = objects.begin(); it != objects.end(); it++){
        Mesh * mesh = it->first;

        if(it->second.size() == 0) continue;

        INFO("Setup Mesh...");
        setupMesh(mesh);

        for(auto objIt = it->second.begin(); objIt != it->second.end(); objIt++){
            //View Frustum Culling
            if(cull == false || (cull && camera->insideViewFrustum((*objIt)))){
                renderObject(*objIt);
            }
        }
    }
}
