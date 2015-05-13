#include "shadow_occluder_render.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "load_manager.hpp"
#include "texture.hpp"
#include "global_variables.hpp"
#include "director.hpp"

const std::string ShadowOccluderRender::VERTEX_SHADER_FILE   = "occluder_vertex.glsl";
const std::string ShadowOccluderRender::FRAGMENT_SHADER_FILE = "occluder_fragment.glsl";

ShadowOccluderRender::ShadowOccluderRender() : RenderElement(false) {}

void ShadowOccluderRender::setup(){
    fbo = new FBO();
    fbo->addDepthTexture(Global::ScreenWidth, Global::ScreenHeight);

    fbo->load();
}

void ShadowOccluderRender::loadShader(){
    LoadManager::loadShader(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
    shader = LoadManager::getShader(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);

    shader->loadHandle("aPosition", 'a');

    shader->loadHandle("uModel", 'u');
    shader->loadHandle("uView", 'u');
    shader->loadHandle("uProjection", 'u');
}

void ShadowOccluderRender::setupEnviroment(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
    fbo->enable();
}

void ShadowOccluderRender::tearDownEnviroment(){
    glDisableVertexAttribArray(shader->getHandle("aPosition"));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
    fbo->disable();

    glDisable(GL_CULL_FACE);
}

void ShadowOccluderRender::setupShader(){
    glUseProgram(shader->getID());

    Camera * camera = Director::getScene()->getCamera();
    std::map<std::string, Light *> lights = Director::getScene()->getLights();
    Light * light = lights.begin()->second;

    glm::mat4 view = glm::lookAt(-glm::length(light->getPosition())*light->getDirection(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

    glUniformMatrix4fv(shader->getHandle("uView"), 1, GL_FALSE,
      glm::value_ptr(view));

    glm::mat4 projection = camera->getProjectionMatrix();
    projection = glm::ortho<float>(-10,10,-10,10,-10,100);
    glUniformMatrix4fv(shader->getHandle("uProjection"), 1, GL_FALSE,
      glm::value_ptr(projection));
}

void ShadowOccluderRender::setupMesh(Mesh * mesh){
    glEnableVertexAttribArray(shader->getHandle("aPosition"));
    glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
    glVertexAttribPointer(shader->getHandle("aPosition"), 3,
                          GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());
}

void ShadowOccluderRender::renderObject(Object * object){
    Mesh * mesh = object->getMesh();

    glUniformMatrix4fv(shader->getHandle("uModel"), 1, GL_FALSE,
                        glm::value_ptr(object->getModelMatrix()));

    glDrawElements(GL_TRIANGLES, (int) mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
}

FBO * ShadowOccluderRender::getFBO(){
    return fbo;
}
