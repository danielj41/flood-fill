#include "render_element.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "director.hpp"

RenderElement::RenderElement(bool viewFrustumCullingEnable) : cull(viewFrustumCullingEnable) {}

void RenderElement::setup() {}

void RenderElement::addObject(ObjectPtr object){
    INFO("Adding object to the Rendering Element...");

    objects[object->getMesh().get()].push_front(object);
}

void RenderElement::removeObject(ObjectPtr object){
    INFO("Removing object from Rendering Element...");

    MeshPtr mesh = object->getMesh();

    ASSERT(objects.find(mesh.get()) != objects.end(), "Mesh does not exist!");

    for(auto it = objects[mesh.get()].begin() ; it != objects[mesh.get()].end(); it++){
        if(*it == object){
            objects[mesh.get()].remove(*it);
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

    CameraPtr camera = Director::getScene()->getCamera();
    CameraPtr cullingCamera = Director::getScene()->getCullingCamera();

    if(objects.size() == 0) return;

    INFO("Setup Shader...");
    setupShader();

    for(auto it = objects.begin(); it != objects.end(); it++){
        Mesh* mesh = it->first;

        if(it->second.size() == 0) continue;

        INFO("Setup Mesh...");
        setupMesh(mesh);

        for(auto objIt = it->second.begin(); objIt != it->second.end(); objIt++){
            //View Frustum Culling
            if(cull == false || (cull && cullingCamera->insideViewFrustum((*objIt)))){
                renderObject(*objIt);
            }
        }
    }
}
