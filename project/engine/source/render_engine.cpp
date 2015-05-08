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
/*
int RenderEngine::renderObjects(){
    int numberObjectsRendered = 0;

    if(objectsCount == 0){
        return 0;
    }

    glUseProgram(geometryShader->getID());

    Camera * camera = Director::getScene()->getCamera();

    //Common information to all Objects
    glUniformMatrix4fv(geometryShader->getHandle("uView"), 1, GL_FALSE,
      glm::value_ptr(camera->getViewMatrix()));
    glUniformMatrix4fv(geometryShader->getHandle("uProjection"), 1, GL_FALSE,
      glm::value_ptr(camera->getProjectionMatrix()));


    glUniform3f(geometryShader->getHandle("uEyePosition"),
                camera->getEye().x,
                camera->getEye().y,
                camera->getEye().z);

    std::map<std::string, Light *> lights = Director::getScene()->getLights();

    //Load the Lights
    for(std::map<std::string, Light *>::iterator it = lights.begin();
            it != lights.end(); it++ ){
        Light * light = it->second;

        glUniform3f(geometryShader->getHandle("uLightColor"),
                    light->getColor().x,
                    light->getColor().y,
                    light->getColor().z);

        glUniform3f(geometryShader->getHandle("uLightDirection"),
                    light->getDirection().x,
                    light->getDirection().y,
                    light->getDirection().z);
    }

    //Rendering all objects of each Mesh
    std::map< Mesh *, std::list<Object *> > tempMap = objects;
    for(std::map< Mesh *, std::list<Object *> >::iterator it =
            tempMap.begin(); it != tempMap.end(); it++){

        //Common information to all Objects of this Mesh
        Mesh * mesh = it->first;

        glEnableVertexAttribArray(geometryShader->getHandle("aPosition"));
        glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
        glVertexAttribPointer(geometryShader->getHandle("aPosition"), 3,
                              GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(geometryShader->getHandle("aNormal"));
        glBindBuffer(GL_ARRAY_BUFFER, mesh->getNormalBuffer());
        glVertexAttribPointer(geometryShader->getHandle("aNormal"), 3,
                              GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());


        //Rendering each object with the specific information
        for(std::list<Object *>::iterator obj = it->second.begin();
                obj != it->second.end(); obj++){

            //View Frustum Culling
            if(camera->insideViewFrustum((*obj))){
                (*obj)->draw(geometryShader);
                numberObjectsRendered++;
            }
        }

        glDisableVertexAttribArray(geometryShader->getHandle("aPosition"));
        glDisableVertexAttribArray(geometryShader->getHandle("aNormal"));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glUseProgram(0);

    return numberObjectsRendered;
}


int RenderEngine::renderWaterObjects(){
    int numberObjectsRendered = 0;

    if(waterObjectsCount == 0){
        return 0;
    }

    glUseProgram(geometryWaterShader->getID());

    Camera * camera = Director::getScene()->getCamera();

    //Common information to all Objects
    glUniformMatrix4fv(geometryWaterShader->getHandle("uView"), 1, GL_FALSE,
      glm::value_ptr(camera->getViewMatrix()));
    glUniformMatrix4fv(geometryWaterShader->getHandle("uProjection"), 1, GL_FALSE,
      glm::value_ptr(camera->getProjectionMatrix()));


    glUniform3f(geometryWaterShader->getHandle("uEyePosition"),
                camera->getEye().x,
                camera->getEye().y,
                camera->getEye().z);

    std::map<std::string, Light *> lights = Director::getScene()->getLights();

    //Load the Lights
    for(std::map<std::string, Light *>::iterator it = lights.begin();
            it != lights.end(); it++ ){
        Light * light = it->second;

        glUniform3f(geometryWaterShader->getHandle("uLightColor"),
                    light->getColor().x,
                    light->getColor().y,
                    light->getColor().z);

        glUniform3f(geometryWaterShader->getHandle("uLightDirection"),
                    light->getDirection().x,
                    light->getDirection().y,
                    light->getDirection().z);
    }

    //Rendering all objects of each Mesh
    std::map< Mesh *, std::list<Object *> > tempMap = waterObjects;
    for(std::map< Mesh *, std::list<Object *> >::iterator it =
            tempMap.begin(); it != tempMap.end(); it++){

        //Common information to all Objects of this Mesh
        Mesh * mesh = it->first;

        glEnableVertexAttribArray(geometryWaterShader->getHandle("aPosition"));
        glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
        glVertexAttribPointer(geometryWaterShader->getHandle("aPosition"), 3,
                              GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(geometryWaterShader->getHandle("aNormal"));
        glBindBuffer(GL_ARRAY_BUFFER, mesh->getNormalBuffer());
        glVertexAttribPointer(geometryWaterShader->getHandle("aNormal"), 3,
                              GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());


        //Rendering each object with the specific information
        for(std::list<Object *>::iterator obj = it->second.begin();
                obj != it->second.end(); obj++){

            //View Frustum Culling
            if(camera->insideViewFrustum((*obj))){
                (*obj)->draw(geometryWaterShader);
                numberObjectsRendered++;
            }
        }

        glDisableVertexAttribArray(geometryWaterShader->getHandle("aPosition"));
        glDisableVertexAttribArray(geometryWaterShader->getHandle("aNormal"));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glUseProgram(0);

    return numberObjectsRendered;
}

int RenderEngine::renderTexturedObjects(){
    int numberObjectsRendered = 0;

    if(texturedObjectsCount == 0){
        return 0;
    }

    glUseProgram(geometryTextureShader->getID());

    Camera * camera = Director::getScene()->getCamera();

    //Common information to all Objects
    glUniformMatrix4fv(geometryTextureShader->getHandle("uView"), 1, GL_FALSE,
      glm::value_ptr(camera->getViewMatrix()));
    glUniformMatrix4fv(geometryTextureShader->getHandle("uProjection"), 1, GL_FALSE,
      glm::value_ptr(camera->getProjectionMatrix()));


    glUniform3f(geometryTextureShader->getHandle("uEyePosition"),
                camera->getEye().x,
                camera->getEye().y,
                camera->getEye().z);

    std::map<std::string, Light *> lights = Director::getScene()->getLights();

    //Load the Lights
    for(std::map<std::string, Light *>::iterator it = lights.begin();
            it != lights.end(); it++ ){
        Light * light = it->second;

        glUniform3f(geometryTextureShader->getHandle("uLightColor"),
                    light->getColor().x,
                    light->getColor().y,
                    light->getColor().z);

        glUniform3f(geometryTextureShader->getHandle("uLightDirection"),
                    light->getDirection().x,
                    light->getDirection().y,
                    light->getDirection().z);
    }

    //Rendering all objects of each Mesh
    std::map< Mesh *, std::list<Object *> > tempMap = texturedObjects;
    for(std::map< Mesh *, std::list<Object *> >::iterator it =
            tempMap.begin(); it != tempMap.end(); it++){

        //Common information to all Objects of this Mesh
        Mesh * mesh = it->first;

        glEnableVertexAttribArray(geometryTextureShader->getHandle("aPosition"));
        glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
        glVertexAttribPointer(geometryTextureShader->getHandle("aPosition"), 3,
                              GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(geometryTextureShader->getHandle("aNormal"));
        glBindBuffer(GL_ARRAY_BUFFER, mesh->getNormalBuffer());
        glVertexAttribPointer(geometryTextureShader->getHandle("aNormal"), 3,
                              GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(geometryTextureShader->getHandle("aTexCoord"));
        glBindBuffer(GL_ARRAY_BUFFER, mesh->getTextureCoordinateBuffer());
        glVertexAttribPointer(geometryTextureShader->getHandle("aTexCoord"), 2,
                              GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());


        //Rendering each object with the specific information
        for(std::list<Object *>::iterator obj = it->second.begin();
                obj != it->second.end(); obj++){

            //View Frustum Culling
            if(camera->insideViewFrustum((*obj))){
                (*obj)->draw(geometryTextureShader);
                numberObjectsRendered++;
            }
        }

        glDisableVertexAttribArray(geometryTextureShader->getHandle("aPosition"));
        glDisableVertexAttribArray(geometryTextureShader->getHandle("aNormal"));
        glDisableVertexAttribArray(geometryTextureShader->getHandle("aTexCoord"));
        glBindTexture(GL_TEXTURE_2D, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glUseProgram(0);

    return numberObjectsRendered;
}

void RenderEngine::loadShaders(){
    INFO("Loading all shaders...");


    LoadManager::loadShader("vertex.glsl", "fragment.glsl");
    geometryShader = LoadManager::getShader("vertex.glsl", "fragment.glsl");
    geometryShader->loadHandle("aNormal", 'a');
    geometryShader->loadHandle("aPosition", 'a');
    geometryShader->loadHandle("uModel", 'u');
    geometryShader->loadHandle("uView", 'u');
    geometryShader->loadHandle("uProjection", 'u');
    geometryShader->loadHandle("uNormalMatrix", 'u');
    geometryShader->loadHandle("uDiffuseColor", 'u');
    geometryShader->loadHandle("uSpecularColor", 'u');
    geometryShader->loadHandle("uAmbientColor", 'u');
    geometryShader->loadHandle("uEmissionColor", 'u');
    geometryShader->loadHandle("uShininess", 'u');
    geometryShader->loadHandle("uEyePosition", 'u');
    //shader->loadHandle("uLightPosition", 'u');
    geometryShader->loadHandle("uLightDirection", 'u');
    geometryShader->loadHandle("uLightColor", 'u');
    geometryShader->loadHandle("alpha", 'u');

    LoadManager::loadShader("vertex-water.glsl", "fragment-water.glsl");
    geometryWaterShader = LoadManager::getShader("vertex-water.glsl", "fragment-water.glsl");
    geometryWaterShader->loadHandle("aNormal", 'a');
    geometryWaterShader->loadHandle("aPosition", 'a');
    geometryWaterShader->loadHandle("uModel", 'u');
    geometryWaterShader->loadHandle("uView", 'u');
    geometryWaterShader->loadHandle("uProjection", 'u');
    geometryWaterShader->loadHandle("uNormalMatrix", 'u');
    geometryWaterShader->loadHandle("uDiffuseColor", 'u');
    geometryWaterShader->loadHandle("uSpecularColor", 'u');
    geometryWaterShader->loadHandle("uAmbientColor", 'u');
    geometryWaterShader->loadHandle("uEmissionColor", 'u');
    geometryWaterShader->loadHandle("uShininess", 'u');
    geometryWaterShader->loadHandle("uEyePosition", 'u');
    //shader->loadHandle("uLightPosition", 'u');
    geometryWaterShader->loadHandle("uLightDirection", 'u');
    geometryWaterShader->loadHandle("uLightColor", 'u');
    geometryWaterShader->loadHandle("uWaterData", 'u');
    geometryWaterShader->loadHandle("uWaterColor", 'u');
    geometryWaterShader->loadHandle("uWaterBlock", 'u');

    LoadManager::loadShader("vertex-texture.glsl", "fragment-texture.glsl");
    geometryTextureShader = LoadManager::getShader("vertex-texture.glsl",
                                                   "fragment-texture.glsl");
    geometryTextureShader->loadHandle("aNormal", 'a');
    geometryTextureShader->loadHandle("aPosition", 'a');
    geometryTextureShader->loadHandle("aTexCoord", 'a');
    geometryTextureShader->loadHandle("uModel", 'u');
    geometryTextureShader->loadHandle("uView", 'u');
    geometryTextureShader->loadHandle("uProjection", 'u');
    geometryTextureShader->loadHandle("uNormalMatrix", 'u');
    geometryTextureShader->loadHandle("uDiffuseColor", 'u');
    geometryTextureShader->loadHandle("uSpecularColor", 'u');
    geometryTextureShader->loadHandle("uAmbientColor", 'u');
    geometryTextureShader->loadHandle("uEmissionColor", 'u');
    geometryTextureShader->loadHandle("uShininess", 'u');
    geometryTextureShader->loadHandle("uEyePosition", 'u');
    geometryTextureShader->loadHandle("uTextureID", 'u');
    //shader->loadHandle("uLightPosition", 'u');
    geometryTextureShader->loadHandle("uLightDirection", 'u');
    geometryTextureShader->loadHandle("uLightColor", 'u');

    LoadManager::loadShader("bounding_box_vertex.glsl",
                            "bounding_box_fragment.glsl");
    debugShader = LoadManager::getShader("bounding_box_vertex.glsl",
                                         "bounding_box_fragment.glsl");
    debugShader->loadHandle("aPosition", 'a');
    debugShader->loadHandle("uModel", 'u');
    debugShader->loadHandle("uView", 'u');
    debugShader->loadHandle("uProjection", 'u');
}
*/
void RenderEngine::setupOpenGL(){
    INFO("Setup OpenGL...");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
    glEnable(GL_TEXTURE_2D);
}
