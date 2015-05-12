#include "object.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "GLIncludes.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "director.hpp"

Object::Object() : _hasTexture(false), textureEnabled(false), water(false), alpha(1.0f) {
    _hasNormalMap = false;
    normalMapScale = 0.0f;
    normalMapBias = 0.0f;
}

Object::Object(Mesh * _mesh, Material * _material)
    : mesh(_mesh), material(_material), _hasTexture(false), textureEnabled(false), water(false), alpha(1.0f){

    loadIdentity();
    ASSERT(getModelMatrix() == glm::mat4(1.0f),
        "Idetenty matrix did't load for this object");

    _hasNormalMap = false;
    normalMapScale = 0.0f;
    normalMapBias = 0.0f;
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

    if(alpha < 0.99f) {
        glDepthMask(GL_FALSE);
    }

    if(hasTexture() && isTextureEnabled()){
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture->getTexture());
        glUniform1i(shader->getHandle("uTextureID"), 0);
    }
    else if(isWater()) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, waterData);
        glUniform1i(shader->getHandle("uWaterData"), 0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, waterColor);
        glUniform1i(shader->getHandle("uWaterColor"), 1);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, waterBlock);
        glUniform1i(shader->getHandle("uWaterBlock"), 2);
    } else {
        glUniform1f(shader->getHandle("alpha"), alpha);
    }

    drawElements();

    if(alpha < 0.99f) {
        glDepthMask(GL_TRUE);
    }
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

Texture * Object::getTexture(){
    ASSERT(hasTexture(), "This object does not have texture!");
    return texture;
}

float Object::getAlpha(){
    return alpha;
}

GLuint Object::getWaterData(){
    return waterData;
}

GLuint Object::getWaterColor(){
    return waterColor;
}

GLuint Object::getWaterBlock(){
    return waterBlock;
}

Texture * Object::getNormalMap(){
    ASSERT(hasNormalMap(), "This Object does not have normal map!");
    return normalMap;
}

float Object::getNormalMapScale(){
    return normalMapScale;
}

float Object::getNormalMapBias(){
    return normalMapBias;
}

void Object::setAlpha(float _alpha) {
    alpha = _alpha;
}

void Object::applyTexture(Texture * _texture){
    texture = _texture;
    _hasTexture = true;
}

void Object::applyNormalMap(Texture * _texture){
    normalMap = _texture;
    _hasNormalMap = true;
}

void Object::applyWaterData(GLuint id){
    waterData = id;
}

void Object::applyWaterColor(GLuint id){
    waterColor = id;
}

void Object::applyWaterBlock(GLuint id){
    waterBlock = id;
}

void Object::setDTime(glm::vec2 _dTime) {
    dTime = _dTime;
}

glm::vec2 Object::getDTime() {
    return dTime;
}

void Object::setVelocity(glm::vec3 _velocity) {
    velocity = _velocity;
}

glm::vec3 Object::getVelocity() {
    return velocity;
}

void Object::enableTexture(){
    ASSERT(_hasTexture, "This Object does not have texture!");
    textureEnabled = true;
}

void Object::disableTexture(){
    ASSERT(_hasTexture, "This Object does not have texture!");
    textureEnabled = false;
}

bool Object::isTextureEnabled(){
    ASSERT(_hasTexture, "This Object does not have texture!");
    return textureEnabled;
}

bool Object::hasTexture(){
    return _hasTexture;
}

bool Object::hasNormalMap(){
    return _hasNormalMap;
}

bool Object::isWater() {
    return water;
}

void Object::enableWater() {
    water = true;
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

void Object::setMaterial(Material * _material) {
    material = _material;
}
