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
Shader * RenderEngine::geometryShader;
std::map< Mesh *, std::list<Object *> > RenderEngine::objectsToRender;

void RenderEngine::setup(){
    INFO("Setup RenderEngine...");

    loadShaders();
    setupOpenGL();

    loaded = true;
    INFO("RenderEngine set!");
}

void RenderEngine::render(){
    INFO("Render Enging: rendering objects...");

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

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
    for(std::map< Mesh *, std::list<Object *> >::iterator it =
            objectsToRender.begin(); it != objectsToRender.end(); it++){

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
            (*obj)->draw(geometryShader);
        }

        glDisableVertexAttribArray(geometryShader->getHandle("aPosition"));
        glDisableVertexAttribArray(geometryShader->getHandle("aNormal"));
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    glUseProgram(0);
}

void RenderEngine::addMesh(Mesh * mesh){
    INFO("Adding mesh " << mesh->getFileName() << " to the Rendering Engine...");

    ASSERT(loaded, "You have to setup the Rendering Engine!");

    std::list<Object *> l;
    objectsToRender[mesh] = l;
}

void RenderEngine::addObject(Object * object){
    INFO("Adding object to the Rendering Engine...");

    ASSERT(loaded, "You have to setup the Rendering Engine!");
    ASSERT(objectsToRender.find(object->getMesh()) != objectsToRender.end(),
            "You need to add the mesh " << object->getMesh()->getFileName()
            << " to the Rendering Engine!");

    objectsToRender[object->getMesh()].push_front(object);
}

void RenderEngine::removeMesh(Mesh * mesh){
    INFO("Removing mesh " << mesh->getFileName()
            << " from the Rendering Engine...");

    ASSERT(loaded, "You have to setup the Rendering Engine!");
    ASSERT(objectsToRender.find(mesh) != objectsToRender.end(),
            "You are trying to remove the mesh " << mesh->getFileName()
            << ", and it is not in the Rendering Engine");

    objectsToRender.erase(mesh);
}

void RenderEngine::removeObject(Object * object){
    INFO("Removing object to the Rendering Engine...");

    ASSERT(loaded, "You have to setup the Rendering Engine!");
    ASSERT(objectsToRender.find(object->getMesh()) != objectsToRender.end(),
            "Mesh " << object->getMesh()->getFileName()
            << " is not in the Rendering Engine! Can't Remove this object");

    for(std::list<Object *>::iterator it =
            objectsToRender[object->getMesh()].begin();
            it != objectsToRender[object->getMesh()].end(); it++){

        if(*it == object){
            objectsToRender[object->getMesh()].erase(it);
            INFO("Rendering Engine: Object removed!");
            return;
        }
    }

    INFO("Rendering Engine: Object not found!");
}


/* Private Methods */

void RenderEngine::loadShaders(){
    INFO("Loading all shaders...");

    Shader * shader;

    LoadManager::loadShader("vertex.glsl", "fragment.glsl");
    shader = LoadManager::getShader("vertex.glsl", "fragment.glsl");
    shader->loadHandle("aNormal", 'a');
    shader->loadHandle("aPosition", 'a');
    shader->loadHandle("uModel", 'u');
    shader->loadHandle("uView", 'u');
    shader->loadHandle("uProjection", 'u');
    shader->loadHandle("uNormalMatrix", 'u');
    shader->loadHandle("uDiffuseColor", 'u');
    shader->loadHandle("uSpecularColor", 'u');
    shader->loadHandle("uAmbientColor", 'u');
    shader->loadHandle("uEmissionColor", 'u');
    shader->loadHandle("uShininess", 'u');
    shader->loadHandle("uEyePosition", 'u');
    //shader->loadHandle("uLightPosition", 'u');
    shader->loadHandle("uLightDirection", 'u');
    shader->loadHandle("uLightColor", 'u');
    geometryShader = shader;

    LoadManager::loadShader("bounding_box_vertex.glsl",
                            "bounding_box_fragment.glsl");
    shader = LoadManager::getShader("bounding_box_vertex.glsl",
                                    "bounding_box_fragment.glsl");
    shader->loadHandle("aPosition", 'a');
    shader->loadHandle("uModel", 'u');
    shader->loadHandle("uView", 'u');
    shader->loadHandle("uProjection", 'u');
}

void RenderEngine::setupOpenGL(){
    INFO("Setup OpenGL...");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glCullFace(GL_BACK);
}
