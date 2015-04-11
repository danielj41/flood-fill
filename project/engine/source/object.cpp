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

Object::Object(Mesh * _mesh, Shader * _shader): mesh(_mesh), shader(_shader){
    loadIdentity();
    ASSERT(getMatrix() == glm::mat4(1.0f),
        "Idetenty matrix did't load for this object");
}

void Object::draw(){
    INFO("Drawing mesh " << mesh->getFileName()
        << " with the vertex shader " << shader->getVertexShaderFileName()
        << " and fragment shader " << shader->getFragmentShaderFileName());

    glUseProgram(shader->getID());
    glEnableVertexAttribArray(shader->getHandle("aPosition"));
    glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
    glVertexAttribPointer(shader->getHandle("aPosition"), 3, GL_FLOAT,
                          GL_FALSE, 0, 0);

    glEnableVertexAttribArray(shader->getHandle("aNormal"));
    glBindBuffer(GL_ARRAY_BUFFER, mesh->getNormalBuffer());
    glVertexAttribPointer(shader->getHandle("aNormal"), 3,
                          GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());

    glUniformMatrix4fv(shader->getHandle("uModel"), 1, GL_FALSE,
                        glm::value_ptr(getMatrix()));
    glUniformMatrix4fv(shader->getHandle("uView"), 1, GL_FALSE,
      glm::value_ptr(Director::getScene()->getCamera()->getViewMatrix()));
    glUniformMatrix4fv(shader->getHandle("uProjection"), 1, GL_FALSE,
      glm::value_ptr(Director::getScene()->getCamera()->getProjectionMatrix()));

    glDrawElements(GL_TRIANGLES, (int) mesh->getIndices().size(),
                   GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(shader->getHandle("aPosition"));
    glDisableVertexAttribArray(shader->getHandle("aNormal"));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
}

Mesh * Object::getMesh(){
    return mesh;
}

Shader * Object::getShader(){
    return shader;
}

glm::mat4 Object::getMatrix(){
    return modelMatrix;
}

void Object::scale(glm::vec3 scalingVector){
    modelMatrix = glm::scale(glm::mat4(1.0f), scalingVector)*modelMatrix;
}

void Object::rotate(float angle, glm::vec3 direction){
    modelMatrix = glm::rotate(glm::mat4(1.0f), angle, direction)*modelMatrix;
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
