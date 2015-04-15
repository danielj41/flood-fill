#include "bounding_box.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "glm/gtc/matrix_transform.hpp"

#include "director.hpp"

#define degreesToRadians(x) (x*(3.141592f/180.0f))

//Draw method dependencies
#include "mesh.hpp"
#include "shader.hpp"
#include "load_manager.hpp"
#include "glm/gtc/type_ptr.hpp"

const int BoundingBox::NUMBER_VERTICES = 8;

BoundingBox::BoundingBox(){}

BoundingBox::BoundingBox(glm::vec3 _max, glm::vec3 _min)
    : max(_max), min(_min), collisionID(1), collideWithID(1) {
    INFO("Creating a bounding box...");

    vertices[0] = glm::vec3(max.x, min.y, max.z);
    vertices[1] = glm::vec3(max.x, max.y, max.z);
    vertices[2] = glm::vec3(min.x, max.y, max.z);
    vertices[3] = glm::vec3(min.x, min.y, max.z);

    vertices[4] = glm::vec3(max.x, min.y, min.z);
    vertices[5] = glm::vec3(max.x, max.y, min.z);
    vertices[6] = glm::vec3(min.x, max.y, min.z);
    vertices[7] = glm::vec3(min.x, min.y, min.z);
}

glm::vec3 BoundingBox::getMax(){
    return max;
}

glm::vec3 BoundingBox::getMin(){
    return min;
}

glm::vec3 BoundingBox::getVertex(int index){
    ASSERT(index < NUMBER_VERTICES && index >= 0,
            "Index " << index << " out of range: Bounding Box getVertex");
    return glm::vec3(Director::getScene()->getCamera()->getProjectionMatrix()*Director::getScene()->getCamera()->getViewMatrix()*modelMatrix*glm::vec4(vertices[index], 1));
}

int BoundingBox::getCollisionID(){
    return collisionID;
}

int BoundingBox::getCollideWithID(){
    return collideWithID;
}

void BoundingBox::setCollisionID(int ID){
    collisionID = ID;
}

void BoundingBox::setCollideWithID(int ID){
    collideWithID = ID;
}

glm::mat4 BoundingBox::getModelMatrix(){
    return modelMatrix;
}

void BoundingBox::scale(glm::vec3 scalingVector){
    modelMatrix = glm::scale(glm::mat4(1.0f), scalingVector)*modelMatrix;
}

void BoundingBox::rotate(float angle, glm::vec3 direction){
    modelMatrix = glm::rotate(glm::mat4(1.0f), degreesToRadians(angle),
                                direction)*modelMatrix;
}

void BoundingBox::translate(glm::vec3 translationVector){
    modelMatrix = glm::translate(glm::mat4(1.0f), translationVector)*modelMatrix;
}

void BoundingBox::loadIdentity(){
    modelMatrix = glm::mat4(1.0f);
}

void BoundingBox::setModelMatrix(glm::mat4 matrix){
    modelMatrix = matrix;
}

void BoundingBox::draw(){
    INFO("Drawing a Bounding Box...");

    // TODO: Change to a more lightweight shader
    Shader * shader = LoadManager::getShader("bounding_box_vertex.glsl",
                                             "bounding_box_fragment.glsl");
    // TODO: Change to a more lightweight mesh if it is possible
    Mesh * mesh = LoadManager::getMesh("cube.obj");

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glUseProgram(shader->getID());
    glEnableVertexAttribArray(shader->getHandle("aPosition"));
    glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
    glVertexAttribPointer(shader->getHandle("aPosition"), 3, GL_FLOAT,
                          GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());

    glUniformMatrix4fv(shader->getHandle("uModel"), 1, GL_FALSE,
                        glm::value_ptr(getModelMatrix()));
    glUniformMatrix4fv(shader->getHandle("uView"), 1, GL_FALSE,
      glm::value_ptr(Director::getScene()->getCamera()->getViewMatrix()));
    glUniformMatrix4fv(shader->getHandle("uProjection"), 1, GL_FALSE,
      glm::value_ptr(Director::getScene()->getCamera()->getProjectionMatrix()));

    glDrawElements(GL_TRIANGLES, (int) mesh->getIndices().size(),
                   GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(shader->getHandle("aPosition"));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glUseProgram(0);
}
