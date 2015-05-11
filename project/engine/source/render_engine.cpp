#include "render_engine.hpp"

#include <vector>
#include <algorithm>
#include <utility>

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
std::map< std::string, unsigned int > RenderEngine::renderElementsPriority;


void RenderEngine::setup(){
    INFO("Setup RenderEngine...");

    setupOpenGL();

    loaded = true;
    INFO("RenderEngine set!");
}

void RenderEngine::render(){
    INFO("Render Engine: rendering objects...");

    ASSERT(loaded, "You dind't load the rendering engine!");

    std::vector< std::pair<std::string, unsigned int> > elements(renderElementsPriority.begin(), renderElementsPriority.end());
    std::sort(elements.begin(), elements.end(),
        [](const std::pair<std::string, unsigned int> & a, const std::pair<std::string, unsigned int> & b) -> bool
        {
            if(a.second == b.second){
                return a.first < b.first;
            }
            return a.second < b.second;
        }
    );

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for(unsigned int i = 0; i < elements.size(); i++){
        RenderElement * element = renderElements[elements[i].first];

        INFO("Rendering Objects from Render Element " << elements[i].first << "...");

        INFO("Setup Enviroment...")
        element->setupEnviroment();
        INFO("Render Pass...")
        element->renderPass();
        INFO("Tear Down Enviroment...")
        element->tearDownEnviroment();
    }
}

void RenderEngine::addRenderElement(std::string name, RenderElement * renderElement, unsigned int priority){
    INFO("Adding render element " << name << " to the render engine...");
    ASSERT(loaded, "You dind't load the rendering engine!");

    INFO("Setting up Render Element...");
    renderElement->setup();

    INFO("Loading the Render Element Shader...");
    renderElement->loadShader();

    renderElements[name] = renderElement;
    renderElementsPriority[name] = priority;
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
            renderElementsPriority.erase(renderElementsPriority.find(name));
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
