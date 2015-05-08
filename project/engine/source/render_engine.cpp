#include "render_engine.hpp"

#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "director.hpp"
#include "load_manager.hpp"
#include "camera.hpp"
#include "light.hpp"


bool RenderEngine::loaded = false;
std::list< RenderElement * > RenderEngine::renderElements;


void RenderEngine::setup(){
    INFO("Setup RenderEngine...");

    setupOpenGL();

    loaded = true;
    INFO("RenderEngine set!");
}

void RenderEngine::render(){
    INFO("Render Enging: rendering objects...");

    ASSERT(loaded, "You dind't load the rendering engine!");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(auto it = renderElements.begin(); it != renderElements.end(); it++){
        (*it)->setupEnviroment();
        (*it)->renderPass();
        (*it)->tearDownEnviroment();
    }
}

void RenderEngine::addRenderElement(RenderElement * renderElement){
    INFO("Adding render element to the render engine...");
    ASSERT(loaded, "You dind't load the rendering engine!");

    renderElement->loadShader();
    renderElements.push_back(renderElement);
}

void RenderEngine::removeRenderElement(RenderElement * renderElement){
    INFO("Removing Render Element from Render Engine...");
    ASSERT(loaded, "You dind't load the rendering engine!");

    for(auto it = renderElements.begin(); it != renderElements.end(); it++){
        if(*it == renderElement){
            renderElements.erase(it);
            INFO("Render Element Removed!");
            return;
        }
    }

    DEBUG("Could not find Render Element!");
}

/* Private Methods */

void RenderEngine::setupOpenGL(){
    INFO("Setup OpenGL...");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_2D);
}
