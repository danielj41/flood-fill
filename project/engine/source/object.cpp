#include "object.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "GLIncludes.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "director.hpp"

Object::Object() {
    DEBUG("Empty Object!");
}

Object::Object(Mesh * _mesh, Material * _material)
    : mesh(_mesh), material(_material){

    loadIdentity();
    ASSERT(getModelMatrix() == glm::mat4(1.0f),
        "Idetenty matrix did't load for this object");
}

void Object::draw(Shader * shader){

    glUniformMatrix4fv(shader->getHandle("uNormalMatrix"), 1, GL_FALSE,
        glm::value_ptr(
        glm::transpose(
        glm::inverse(
        getModelMatrix()))));

    glUniformMatrix4fv(shader->getHandle("uModel"), 1, GL_FALSE,
                        glm::value_ptr(getModelMatrix()));

    glUniform3f(shader->getHandle("uDiffuseColor"),
                material->getDiffuseColor().x,
                material->getDiffuseColor().y,
                material->getDiffuseColor().z);
    glUniform3f(shader->getHandle("uSpecularColor"),
                material->getSpecularColor().x,
                material->getSpecularColor().y,
                material->getSpecularColor().z);
    glUniform3f(shader->getHandle("uAmbientColor"),
                material->getAmbientColor().x,
                material->getAmbientColor().y,
                material->getAmbientColor().z);
    glUniform3f(shader->getHandle("uEmissionColor"),
                material->getEmissionColor().x,
                material->getEmissionColor().y,
                material->getEmissionColor().z);
    glUniform1f(shader->getHandle("uShininess"), material->getShininess());

    drawElements();
}

void Object::drawElements(){
    glDrawElements(GL_TRIANGLES, (int) mesh->getIndices().size(),
                   GL_UNSIGNED_INT, 0);
}

Mesh * Object::getMesh(){
    return mesh;
}

Material * Object::getMaterial(){
    return material;
}

glm::mat4 Object::getModelMatrix(){
    return modelMatrix;
}

void Object::scale(glm::vec3 scalingVector){
    modelMatrix = glm::scale(glm::mat4(1.0f), scalingVector)*modelMatrix;
}

void Object::rotate(float angle, glm::vec3 direction){
    modelMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(angle),
                                direction)*modelMatrix;
}

void Object::translate(glm::vec3 translationVector){
    modelMatrix = glm::translate(glm::mat4(1.0f), translationVector)*modelMatrix;
}

void Object::loadIdentity(){
    modelMatrix = glm::mat4(1.0f);
}

void Object::setMatrix(glm::mat4 matrix){
    modelMatrix = matrix;
}
