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
    gridScale = glm::vec2(1.0f, 1.0f);
    shearX = 0;
    shearZ = 0;
    texturePack = 0;
}

Object::Object(MeshPtr _mesh, MaterialPtr _material)
    : mesh(_mesh), material(_material), _hasTexture(false), textureEnabled(false), water(false), alpha(1.0f){

    loadIdentity();
    ASSERT(getModelMatrix() == glm::mat4(1.0f),
        "Idetenty matrix did't load for this object");

    _hasNormalMap = false;
    normalMapScale = 0.0f;
    normalMapBias = 0.0f;
    gridScale = glm::vec2(1.0f, 1.0f);
    shearX = 0;
    shearZ = 0;
    texturePack = 0;
}

MeshPtr Object::getMesh(){
    return mesh;
}

MaterialPtr Object::getMaterial(){
    return material;
}

glm::mat4 Object::getModelMatrix(){
    return modelMatrix;
}

TexturePtr Object::getTexture(){
    ASSERT(hasTexture(), "This object does not have texture!");
    return this->getTexture(texturePack);
}

TexturePtr Object::getTexture(unsigned int i){
    ASSERT(hasTexture(), "This object does not have texture!");
    ASSERT(i < texture.size(), "The texture #" << i << " is not avaible!");
    return texture[i];
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

TexturePtr Object::getNormalMap(){
    ASSERT(hasNormalMap(), "This Object does not have normal map!");
    return this->getNormalMap(texturePack);
}

TexturePtr Object::getNormalMap(unsigned int i){
    ASSERT(hasNormalMap(), "This Object does not have normal map!");
    ASSERT(i < normalMap.size(), "The texture #" << i << " is not avaible!");
    return normalMap[i];
}

float Object::getNormalMapScale(){
    return normalMapScale;
}

float Object::getNormalMapBias(){
    return normalMapBias;
}

std::vector<TexturePtr> Object::getTextures() {
    return texture;
}

std::vector<TexturePtr> Object::getNormalMaps() {
    return normalMap;
}

void Object::setAlpha(float _alpha) {
    alpha = _alpha;
}

void Object::applyTexture(TexturePtr _texture){
    texture.push_back(_texture);
    _hasTexture = true;
}

void Object::applyNormalMap(TexturePtr _texture){
    normalMap.push_back(_texture);
    _hasNormalMap = true;
}

void Object::applyTextureIndex(TexturePtr _texture, int index){
    texture[index] = _texture;
    _hasTexture = true;
}

void Object::applyNormalMapIndex(TexturePtr _texture, int index){
    normalMap[index] = _texture;
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

void Object::setGridScale(glm::vec2 _gridScale) {
    gridScale = _gridScale;
}

glm::vec2 Object::getGridScale() {
    return gridScale;
}

void Object::setShear(int x, int z) {
    shearX = x;
    shearZ = z;
}

int Object::getShearX() {
    return shearX;
}

int Object::getShearZ() {
    return shearZ;
}

unsigned int Object::getTexturePack() {
    return texturePack;
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

void Object::setMaterial(MaterialPtr _material) {
    material = _material;
}

void Object::setTextureAndNormalMapPack(unsigned int i){
    texturePack = i;
}

void Object::setTextures(std::vector<TexturePtr> tex){
    texture = tex;
}

void Object::setNormalMaps(std::vector<TexturePtr> norms){
    normalMap = norms;
}

void Object::setTexturePack(unsigned int texturePck){
    texturePack = texturePck;
}
