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
std::map< std::string, RenderElement * > RenderEngine::renderElements;
RenderGrid *RenderEngine::renderGrid = NULL;

void RenderEngine::setup(){
    INFO("Setup RenderEngine...");

    setupOpenGL();

    loaded = true;
    INFO("RenderEngine set!");
}

void RenderEngine::render(){
    renderGrid->clean();

    INFO("Render Engine: rendering objects...");

    ASSERT(loaded, "You dind't load the rendering engine!");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(auto it = renderElements.begin(); it != renderElements.end(); it++){
        INFO("Rendering Objects from Render Element " << it->first << "...");
        INFO("Setup Enviroment...")
        it->second->setupEnviroment();
        INFO("Render Pass...")
        it->second->renderPass();
        INFO("Tear Down Enviroment...")
        it->second->tearDownEnviroment();
    }
}

void RenderEngine::addRenderElement(std::string name, RenderElement * renderElement){
    INFO("Adding render element " << name << " to the render engine...");
    ASSERT(loaded, "You dind't load the rendering engine!");

    renderElement->loadShader();
    renderElements[name] = renderElement;
    INFO("Render element " << name << " added!");
}

RenderElement * RenderEngine::getRenderElement(std::string name){
    ASSERT(loaded, "You dind't load the rendering engine!");

    if(renderElements.find(name) != renderElements.end()){
        return renderElements[name];
    }

    ASSERT(false, "Render Element " << name << " not found!");
    return NULL;
}

void RenderEngine::removeRenderElement(std::string name){
    INFO("Removing Render Element " << name << " from Render Engine...");
    ASSERT(loaded, "You dind't load the rendering engine!");

    for(auto it = renderElements.begin(); it != renderElements.end(); it++){
        if(it->first == name){
            renderElements.erase(it);
            INFO("Render Element Removed!");
            return;
        }
    }

    DEBUG("Could not find Render Element!");
}

void RenderEngine::setRenderGrid(RenderGrid *_renderGrid) {
    _renderGrid->initialize();
    renderGrid = _renderGrid;
}

RenderGrid *RenderEngine::getRenderGrid() {
    return renderGrid;
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
