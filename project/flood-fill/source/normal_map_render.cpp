#include "normal_map_render.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "glm/gtc/type_ptr.hpp"

#include "load_manager.hpp"
#include "director.hpp"

const std::string NormalMapRender::VERTEX_SHADER_FILE = "vertex-normal-map.glsl";
const std::string NormalMapRender::FRAGMENT_SHADER_FILE = "fragment-normal-map.glsl";

NormalMapRender::NormalMapRender() : RenderElement() {}

void NormalMapRender::loadShader(){
    LoadManager::loadShader(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
    shader = LoadManager::getShader(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);

    shader->loadHandle("aPosition", 'a');
    shader->loadHandle("aNormal", 'a');
    shader->loadHandle("aTexCoord", 'a');
    shader->loadHandle("aTangent", 'a');

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
    shader->loadHandle("uLightDirection", 'u');
    shader->loadHandle("uLightColor", 'u');

    shader->loadHandle("uTextureID", 'u');
    shader->loadHandle("uNormalTexID", 'u');
    shader->loadHandle("uNormalMapBias", 'u');
    shader->loadHandle("uNormalMapScale", 'u');

    shader->loadHandle("uGridScale", 'u');
}

void NormalMapRender::setupEnviroment(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
}

void NormalMapRender::tearDownEnviroment(){
    glDisableVertexAttribArray(shader->getHandle("aPosition"));
    glDisableVertexAttribArray(shader->getHandle("aTangent"));
    glDisableVertexAttribArray(shader->getHandle("aTexCoord"));
    glDisableVertexAttribArray(shader->getHandle("aNormal"));
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);

    glDisable(GL_CULL_FACE);
}

void NormalMapRender::setupShader(){
    glUseProgram(shader->getID());

    Camera * camera = Director::getScene()->getCamera();

    //Common information to all Objects
    glUniformMatrix4fv(shader->getHandle("uView"), 1, GL_FALSE,
      glm::value_ptr(camera->getViewMatrix()));
    glUniformMatrix4fv(shader->getHandle("uProjection"), 1, GL_FALSE,
      glm::value_ptr(camera->getProjectionMatrix()));


    glUniform3f(shader->getHandle("uEyePosition"),
                camera->getEye().x,
                camera->getEye().y,
                camera->getEye().z);

    std::map<std::string, Light *> lights = Director::getScene()->getLights();

    //Load the Lights
    for(std::map<std::string, Light *>::iterator it = lights.begin();
            it != lights.end(); it++ ){
        Light * light = it->second;

        glUniform3f(shader->getHandle("uLightColor"),
                    light->getColor().x,
                    light->getColor().y,
                    light->getColor().z);

        glUniform3f(shader->getHandle("uLightDirection"),
                    light->getDirection().x,
                    light->getDirection().y,
                    light->getDirection().z);
    }
}

void NormalMapRender::setupMesh(Mesh * mesh){
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

    glEnableVertexAttribArray(shader->getHandle("aTangent"));
    glBindBuffer(GL_ARRAY_BUFFER, mesh->getTangentBuffer());
    glVertexAttribPointer(shader->getHandle("aTangent"), 4,
                          GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());
}

void NormalMapRender::renderObject(Object * object){
    Mesh * mesh = object->getMesh();

    glUniformMatrix4fv(shader->getHandle("uNormalMatrix"), 1, GL_FALSE,
        glm::value_ptr(glm::transpose(glm::inverse(object->getModelMatrix()))));

    glUniformMatrix4fv(shader->getHandle("uModel"), 1, GL_FALSE,
                        glm::value_ptr(object->getModelMatrix()));

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
    glUniform3f(shader->getHandle("uEmissionColor"),
                object->getMaterial()->getEmissionColor().x,
                object->getMaterial()->getEmissionColor().y,
                object->getMaterial()->getEmissionColor().z);
    glUniform1f(shader->getHandle("uShininess"), object->getMaterial()->getShininess());

    glUniform1f(shader->getHandle("uNormalMapScale"), object->getNormalMapBias());
    glUniform1f(shader->getHandle("uNormalMapBias"), object->getNormalMapScale());
    glUniform2f(shader->getHandle("uGridScale"), object->getGridScale().x, object->getGridScale().y);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, object->getTexture()->getTexture());
    glUniform1i(shader->getHandle("uTextureID"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, object->getNormalMap()->getTexture());
    glUniform1i(shader->getHandle("uNormalTexID"), 1);

    glDrawElements(GL_TRIANGLES, (int) mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
}
