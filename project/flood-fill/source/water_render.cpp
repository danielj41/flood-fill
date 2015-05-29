#include "water_render.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "glm/gtc/type_ptr.hpp"

#include "load_manager.hpp"
#include "director.hpp"
#include "shadow_occluder_render.hpp"
#include "render_engine.hpp"
#include "global_variables.hpp"

const std::string WaterRender::VERTEX_SHADER_FILE = "vertex-water.glsl";
const std::string WaterRender::FRAGMENT_SHADER_FILE = "fragment-water.glsl";

WaterRender::WaterRender() : RenderElement() {}

void WaterRender::loadShader(){
    LoadManager::loadShader(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);
    shader = LoadManager::getShader(VERTEX_SHADER_FILE, FRAGMENT_SHADER_FILE);

    shader->loadHandle("aPosition", 'a');
    shader->loadHandle("aTexCoord", 'a');
    shader->loadHandle("aTangent", 'a');

    shader->loadHandle("uModel", 'u');
    shader->loadHandle("uView", 'u');
    shader->loadHandle("uProjection", 'u');
    shader->loadHandle("uShadowMatrix", 'u');

    shader->loadHandle("uDiffuseColor", 'u');
    shader->loadHandle("uSpecularColor", 'u');
    shader->loadHandle("uAmbientColor", 'u');
    shader->loadHandle("uEmissionColor", 'u');
    shader->loadHandle("uShininess", 'u');
    shader->loadHandle("uEyePosition", 'u');
    shader->loadHandle("uLightDirection", 'u');
    shader->loadHandle("uLightColor", 'u');

    shader->loadHandle("uWaterData", 'u');
    shader->loadHandle("uWaterColor", 'u');
    shader->loadHandle("uWaterBlock", 'u');
    shader->loadHandle("uDTime", 'u');

    shader->loadHandle("uTextureID", 'u');
    shader->loadHandle("uNormalTexID", 'u');
    shader->loadHandle("uGridScale", 'u');
    shader->loadHandle("uShadowTexID", 'u');
    shader->loadHandle("uScreenSize", 'u');
}

void WaterRender::setupEnviroment(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);
}

void WaterRender::tearDownEnviroment(){
    glDisableVertexAttribArray(shader->getHandle("aPosition"));
    glDisableVertexAttribArray(shader->getHandle("aTangent"));
    glDisableVertexAttribArray(shader->getHandle("aTexCoord"));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);

    glDisable(GL_CULL_FACE);
}

void WaterRender::setupShader(){
    glUseProgram(shader->getID());

    CameraPtr camera = Director::getScene()->getCamera();

    glUniform2f(shader->getHandle("uScreenSize"), Global::FbWidth, Global::FbHeight);

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

        glUniformMatrix4fv(shader->getHandle("uShadowMatrix"), 1, GL_FALSE,
          glm::value_ptr(light->getProjectionMatrix()*light->getViewMatrix()));

        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D,
            PTR_CAST(ShadowOccluderRender, RenderEngine::getRenderElement("shadow"))->getFBO()->getDepthTexture()->getTexture());
        glUniform1i(shader->getHandle("uShadowTexID"), 3);
    }
}

void WaterRender::setupMesh(Mesh* mesh){
    glEnableVertexAttribArray(shader->getHandle("aPosition"));
    glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
    glVertexAttribPointer(shader->getHandle("aPosition"), 3,
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

void WaterRender::renderObject(ObjectPtr object){
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

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, object->getWaterData());
    glUniform1i(shader->getHandle("uWaterData"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, object->getWaterColor());
    glUniform1i(shader->getHandle("uWaterColor"), 1);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, object->getWaterBlock());
    glUniform1i(shader->getHandle("uWaterBlock"), 2);

    glUniform2f(shader->getHandle("uGridScale"), object->getGridScale().x, object->getGridScale().y);

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, object->getTexture()->getTexture());
    glUniform1i(shader->getHandle("uTextureID"), 4);

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, object->getNormalMap()->getTexture());
    glUniform1i(shader->getHandle("uNormalTexID"), 5);

    glUniform2f(shader->getHandle("uDTime"), object->getDTime().x, object->getDTime().y);

    glDrawElements(GL_TRIANGLES, (int) mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
}
