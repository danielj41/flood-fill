#include "regular_polygons_render.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "glm/gtc/type_ptr.hpp"

#include "load_manager.hpp"
#include "director.hpp"

const std::string RegularPolygonsRender::VERTEX_SHADER_FILE = "vertex.glsl";
const std::string RegularPolygonsRender::FRAGMENT_SHADER_FILE = "fragment.glsl";

RegularPolygonsRender::RegularPolygonsRender() : RenderElement() {}

void RegularPolygonsRender::loadShader(){
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
    shader->loadHandle("uEmissionColor", 'u');
    shader->loadHandle("uShininess", 'u');
    shader->loadHandle("uEyePosition", 'u');
    shader->loadHandle("uTextureID", 'u');
    shader->loadHandle("uLightDirection", 'u');
    shader->loadHandle("uLightColor", 'u');
}

void RegularPolygonsRender::setupEnviroment(){
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_CULL_FACE);
}

void RegularPolygonsRender::tearDownEnviroment(){
    glDisableVertexAttribArray(shader->getHandle("aPosition"));
    glDisableVertexAttribArray(shader->getHandle("aNormal"));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);

    glDisable(GL_CULL_FACE);
}

void RegularPolygonsRender::setupShader(){
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

    //Rendering all objects of each Mesh
    std::map< Mesh *, std::list<Object *> > tempMap = objects;
    for(std::map< Mesh *, std::list<Object *> >::iterator it =
            tempMap.begin(); it != tempMap.end(); it++){

        //Common information to all Objects of this Mesh
        Mesh * mesh = it->first;



        //Rendering each object with the specific information
        for(std::list<Object *>::iterator obj = it->second.begin();
                obj != it->second.end(); obj++){

            //View Frustum Culling
            if(camera->insideViewFrustum((*obj))){
                (*obj)->draw(shader);
                numberObjectsRendered++;
            }
        }

}

void RegularPolygonsRender::setupMesh(Mesh * mesh){
        glEnableVertexAttribArray(shader->getHandle("aPosition"));
        glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
        glVertexAttribPointer(shader->getHandle("aPosition"), 3,
                              GL_FLOAT, GL_FALSE, 0, 0);

        glEnableVertexAttribArray(shader->getHandle("aNormal"));
        glBindBuffer(GL_ARRAY_BUFFER, mesh->getNormalBuffer());
        glVertexAttribPointer(shader->getHandle("aNormal"), 3,
                              GL_FLOAT, GL_FALSE, 0, 0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());
}

void RegularPolygonsRender::renderObject(Object * object){
    Mesh * mesh = object->getMesh();

    glUniformMatrix4fv(shader->getHandle("uNormalMatrix"), 1, GL_FALSE,
        glm::value_ptr(glm::transpose(glm::inverse(object->getModelMatrix()))));

    glUniformMatrix4fv(shader->getHandle("uModel"), 1, GL_FALSE,
                        glm::value_ptr(object->getModelMatrix()));

    glUniform3f(shader->getHandle("uDiffuseColor"),
                object->getMaterial->getDiffuseColor().x,
                object->getMaterial->getDiffuseColor().y,
                object->getMaterial->getDiffuseColor().z);
    glUniform3f(shader->getHandle("uSpecularColor"),
                object->getMaterial->getSpecularColor().x,
                object->getMaterial->getSpecularColor().y,
                object->getMaterial->getSpecularColor().z);
    glUniform3f(shader->getHandle("uAmbientColor"),
                object->getMaterial->getAmbientColor().x,
                object->getMaterial->getAmbientColor().y,
                object->getMaterial->getAmbientColor().z);
    glUniform3f(shader->getHandle("uEmissionColor"),
                object->getMaterial->getEmissionColor().x,
                object->getMaterial->getEmissionColor().y,
                object->getMaterial->getEmissionColor().z);
    glUniform1f(shader->getHandle("uShininess"), object->getMaterial->getShininess());

    glDrawElements(GL_TRIANGLES, (int) mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
}
