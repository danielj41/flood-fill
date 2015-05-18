#include "debug_render.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "glm/gtc/type_ptr.hpp"

#include "load_manager.hpp"
#include "director.hpp"

const std::string DebugRender::VERTEX_SHADER_FILE = "bounding_box_vertex.glsl";
const std::string DebugRender::FRAGMENT_SHADER_FILE = "bounding_box_fragment.glsl";

DebugRender::DebugRender() : RenderElement() {}

void DebugRender::loadShader(){
    LoadManager::loadShader(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
    shader = LoadManager::getShader(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);

    shader->loadHandle("aPosition", 'a');

    shader->loadHandle("uModel", 'u');
    shader->loadHandle("uView", 'u');
    shader->loadHandle("uProjection", 'u');
}

void DebugRender::setupEnviroment(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glEnable(GL_CULL_FACE);
}

void DebugRender::tearDownEnviroment(){
    glDisableVertexAttribArray(shader->getHandle("aPosition"));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);

    glDisable(GL_CULL_FACE);
}

void DebugRender::setupShader(){
    glUseProgram(shader->getID());

    CameraPtr camera = Director::getScene()->getCamera();

    //Common information to all Objects
    glUniformMatrix4fv(shader->getHandle("uView"), 1, GL_FALSE,
      glm::value_ptr(camera->getViewMatrix()));
    glUniformMatrix4fv(shader->getHandle("uProjection"), 1, GL_FALSE,
      glm::value_ptr(camera->getProjectionMatrix()));
}

void DebugRender::setupMesh(MeshPtr){
    Mesh * mesh = LoadManager::getMesh("cube.obj");

    glEnableVertexAttribArray(shader->getHandle("aPosition"));
    glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
    glVertexAttribPointer(shader->getHandle("aPosition"), 3,
                          GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());
}

void DebugRender::renderObject(ObjectPtr){
}

void DebugRender::renderPass(){
    if(boxes.size() == 0) return;

    INFO("Setup Shader...");
    setupShader();
    setupMesh(NULL);

    /*for(auto box = boxes.begin(); box != boxes.end(); box++){
        renderBoundingBox(*box);
    }*/
}

void DebugRender::addBoundingBox(BoundingBoxPtr box){
    boxes.push_front(box);
}

void DebugRender::removeBoundingBox(BoundingBoxPtr box){
    for(auto it = boxes.begin(); it != boxes.end(); it++){
        if(box == *it){
            boxes.erase(it);
        }
    }
}

void DebugRender::renderBoundingBox(BoundingBoxPtr box){
    MeshPtr mesh = LoadManager::getMesh("cube.obj");

    glUniformMatrix4fv(shader->getHandle("uModel"), 1, GL_FALSE,
                        glm::value_ptr(box->getModelMatrix()));

    glDrawElements(GL_TRIANGLES, (int) mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
}
