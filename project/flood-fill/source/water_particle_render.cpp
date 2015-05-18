#include "water_particle_render.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "glm/gtc/type_ptr.hpp"

#include "load_manager.hpp"
#include "director.hpp"

const std::string WaterParticleRender::VERTEX_SHADER_FILE = "vertex-water-particle.glsl";
const std::string WaterParticleRender::FRAGMENT_SHADER_FILE = "fragment-water-particle.glsl";

WaterParticleRender::WaterParticleRender() : RenderElement() {}

void WaterParticleRender::loadShader(){
    LoadManager::loadShader(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
    shader = LoadManager::getShader(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);

    shader->loadHandle("aNormal", 'a');
    shader->loadHandle("aPosition", 'a');
    shader->loadHandle("aTexCoord", 'a');
    shader->loadHandle("uModel", 'u');
    shader->loadHandle("uView", 'u');
    shader->loadHandle("uProjection", 'u');
    shader->loadHandle("uDiffuseColor", 'u');
    shader->loadHandle("uSpecularColor", 'u');
    shader->loadHandle("uAmbientColor", 'u');
    shader->loadHandle("uEmissionColor", 'u');
    shader->loadHandle("uShininess", 'u');
    shader->loadHandle("uEyePosition", 'u');
    shader->loadHandle("uLightDirection", 'u');
    shader->loadHandle("uLightColor", 'u');
    shader->loadHandle("uDTime", 'u');
    shader->loadHandle("uVelocity", 'u');

    cull = false;
}

void WaterParticleRender::setupEnviroment(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);
}

void WaterParticleRender::tearDownEnviroment(){
    glDisableVertexAttribArray(shader->getHandle("aPosition"));
    glDisableVertexAttribArray(shader->getHandle("aNormal"));
    glDisableVertexAttribArray(shader->getHandle("aTexCoord"));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);

    glEnable(GL_CULL_FACE);
    glDepthMask(GL_TRUE);
}

void WaterParticleRender::setupShader(){
    glUseProgram(shader->getID());

    CameraPtr camera = Director::getScene()->getCamera();

    //Common information to all Objects
    glUniformMatrix4fv(shader->getHandle("uView"), 1, GL_FALSE,
      glm::value_ptr(camera->getViewMatrix()));
    glUniformMatrix4fv(shader->getHandle("uProjection"), 1, GL_FALSE,
      glm::value_ptr(camera->getProjectionMatrix()));


    glUniform3f(shader->getHandle("uEyePosition"),
                camera->getEye().x,
                camera->getEye().y,
                camera->getEye().z);

    std::map<std::string, LightPtr> lights = Director::getScene()->getLights();

    //Load the Lights
    for(std::map<std::string, LightPtr>::iterator it = lights.begin();
            it != lights.end(); it++ ){
        LightPtr light = it->second;

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

void WaterParticleRender::setupMesh(MeshPtr mesh){
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

void WaterParticleRender::renderObject(ObjectPtr object){
    MeshPtr mesh = object->getMesh();

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

    glUniform2f(shader->getHandle("uDTime"), object->getDTime().x, object->getDTime().y);
    glUniform3f(shader->getHandle("uVelocity"), object->getVelocity().x, object->getVelocity().y, object->getVelocity().z);

    glDrawElements(GL_TRIANGLES, (int) mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
}
