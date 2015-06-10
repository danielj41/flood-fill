#include "camera_polygons_render.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "glm/gtc/type_ptr.hpp"

#include "global_variables.hpp"
#include "load_manager.hpp"
#include "director.hpp"

const std::string CameraPolygonsRender::VERTEX_SHADER_FILE = "vertex-camera.glsl";
const std::string CameraPolygonsRender::FRAGMENT_SHADER_FILE = "fragment-camera.glsl";

CameraPolygonsRender::CameraPolygonsRender() : RenderElement(false) {}

void CameraPolygonsRender::loadShader(){
    LoadManager::loadShader(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
    shader = LoadManager::getShader(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);

    shader->loadHandle("aNormal", 'a');
    shader->loadHandle("aPosition", 'a');
    shader->loadHandle("aTexCoord", 'a');

    shader->loadHandle("uModel", 'u');
    shader->loadHandle("uView", 'u');
    shader->loadHandle("uProjection", 'u');
    shader->loadHandle("uNormalMatrix", 'u');
    shader->loadHandle("uDiffuseColor", 'u');
    shader->loadHandle("uSpecularColor", 'u');
    shader->loadHandle("uAmbientColor", 'u');
    shader->loadHandle("uShininess", 'u');
    shader->loadHandle("uEyePosition", 'u');
    shader->loadHandle("uLightDirection", 'u');
    shader->loadHandle("uTexID", 'u');

    shader->loadHandle("uSecondColor", 'u');
    shader->loadHandle("uSecondAmbient", 'u');
}

void CameraPolygonsRender::setupEnviroment(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
}

void CameraPolygonsRender::tearDownEnviroment(){
    glDisableVertexAttribArray(shader->getHandle("aPosition"));
    glDisableVertexAttribArray(shader->getHandle("aNormal"));
    glDisableVertexAttribArray(shader->getHandle("aTexCoord"));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);

    glDisable(GL_CULL_FACE);
}

void CameraPolygonsRender::setupShader(){
    glUseProgram(shader->getID());

    CameraPtr camera = Director::getScene()->getCamera();

    glm::mat4 P = glm::perspective(glm::radians(90.0f),
                         (float) Global::ScreenWidth/Global::ScreenHeight,
                         0.1f, 100.f);
    //Common information to all Objects
    glUniformMatrix4fv(shader->getHandle("uProjection"), 1, GL_FALSE,
      glm::value_ptr(P));

    glUniformMatrix4fv(shader->getHandle("uView"), 1, GL_FALSE,
      glm::value_ptr(camera->getViewMatrix()));

    glUniform3f(shader->getHandle("uEyePosition"),
                camera->getEye().x,
                camera->getEye().y,
                camera->getEye().z);

    std::map<std::string, LightPtr> lights = Director::getScene()->getLights();

    //Load the Lights
    for(std::map<std::string, LightPtr>::iterator it = lights.begin();
            it != lights.end(); it++ ){
        LightPtr light = it->second;

        glUniform3f(shader->getHandle("uLightDirection"),
                    light->getDirection().x,
                    light->getDirection().y,
                    light->getDirection().z);
    }
}

void CameraPolygonsRender::setupMesh(Mesh* mesh){
        glEnableVertexAttribArray(shader->getHandle("aPosition"));
        glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
        glVertexAttribPointer(shader->getHandle("aPosition"), 3,
                              GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(shader->getHandle("aNormal"));
        glBindBuffer(GL_ARRAY_BUFFER, mesh->getNormalBuffer());
        glVertexAttribPointer(shader->getHandle("aNormal"), 3,
                              GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(shader->getHandle("aTexCoord"));
        glBindBuffer(GL_ARRAY_BUFFER, mesh->getTextureCoordinateBuffer());
        glVertexAttribPointer(shader->getHandle("aTexCoord"), 2,
                              GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());
}

void CameraPolygonsRender::renderObject(ObjectPtr object){
    MeshPtr mesh = object->getMesh();
    CameraPtr playerCamera = Director::getScene()->getCamera("Camera1");

    glUniformMatrix4fv(shader->getHandle("uNormalMatrix"), 1, GL_FALSE,
        glm::value_ptr(glm::transpose(Director::getScene()->getCamera()->getViewMatrix())));

    glUniformMatrix4fv(shader->getHandle("uModel"), 1, GL_FALSE,
                        glm::value_ptr(glm::inverse(playerCamera->getViewMatrix()) * object->getModelMatrix()));

    glUniform3f(shader->getHandle("uDiffuseColor"),
                object->getMaterial()->getDiffuseColor().x,
                object->getMaterial()->getDiffuseColor().y,
                object->getMaterial()->getDiffuseColor().z);
    glUniform3f(shader->getHandle("uSpecularColor"),
                object->getMaterial()->getSpecularColor().x,
                object->getMaterial()->getSpecularColor().y,
                object->getMaterial()->getSpecularColor().z);
    glUniform3f(shader->getHandle("uAmbientColor"),
                object->getMaterial()->getAmbientColor().x,
                object->getMaterial()->getAmbientColor().y,
                object->getMaterial()->getAmbientColor().z);
    glUniform1f(shader->getHandle("uShininess"), object->getMaterial()->getShininess());

    glUniform3f(shader->getHandle("uSecondColor"),
                object->getAlternativeMaterial()->getDiffuseColor().x,
                object->getAlternativeMaterial()->getDiffuseColor().y,
                object->getAlternativeMaterial()->getDiffuseColor().z);
    glUniform3f(shader->getHandle("uSecondAmbient"),
                object->getAlternativeMaterial()->getAmbientColor().x,
                object->getAlternativeMaterial()->getAmbientColor().y,
                object->getAlternativeMaterial()->getAmbientColor().z);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, object->getTexture()->getTexture());
    glUniform1i(shader->getHandle("uTexID"), 0);

    glDrawElements(GL_TRIANGLES, (int) mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
}
