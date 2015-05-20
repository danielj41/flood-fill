#include "collision_object.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"
#include "glm/glm.hpp"

CollisionObject::CollisionObject() : _canCollide(false), _applyPhysics(false),
      velocity(0), boundingBox(glm::vec3(1.0f, 1.0f, 1.0f),
      glm::vec3(-1.0f, -1.0f, -1.0f)),
      shearX1(0.0f), shearX2(0.0f), shearZ1(0.0f), shearZ2(0.0f) {}

CollisionObject::CollisionObject(glm::vec3 _position)
    : position(_position), _canCollide(true), _applyPhysics(false),  velocity(0), 
      collisionID(0), collideWithID(0), boundingBox(glm::vec3(1.0f, 1.0f, 1.0f),
      glm::vec3(-1.0f, -1.0f, -1.0f)),
      shearX1(0.0f), shearX2(0.0f), shearZ1(0.0f), shearZ2(0.0f) {}

bool CollisionObject::canCollide(){
    return _canCollide;
}

bool CollisionObject::applyPhysics(){
    return _applyPhysics;
}

int CollisionObject::getCollisionID(){
    return collisionID;
}

int CollisionObject::getCollideWithID(){
    return collideWithID;
}

void CollisionObject::setCollisionID(int ID){
    collisionID = ID;
}

void CollisionObject::setCollideWithID(int ID){
    collideWithID = ID;
}

glm::vec3 CollisionObject::getPosition(){
    return position;
}

void CollisionObject::setPosition(glm::vec3 _position){
  position = _position;
}

void CollisionObject::setCanCollide(bool canCollide){
  _canCollide = canCollide;
}

void CollisionObject::setApplyPhysics(bool applyPhysics){
  _applyPhysics = applyPhysics;
}

void CollisionObject::setBoundingBox(BoundingBox _boundingBox) {
    boundingBox = _boundingBox;
}

BoundingBox* CollisionObject::getBoundingBox() {
    return &boundingBox;
}

bool CollisionObject::checkCollision(CollisionObjectPtr other) {
    glm::vec3 min = boundingBox.getMin();
    glm::vec3 oMin = other->boundingBox.getMin();
    glm::vec3 max = boundingBox.getMax();
    glm::vec3 oMax = other->boundingBox.getMax();

    oMax.y += (position.x - oMin.x) / (oMax.x - oMin.x) * (other->shearX2 - other->shearX1) + other->shearX1;
    oMax.y += (position.z - oMin.z) / (oMax.z - oMin.z) * (other->shearZ2 - other->shearZ1) + other->shearZ1;

    return (max.x >= oMin.x && min.x <= oMax.x)
     && (max.y >= oMin.y && min.y <= oMax.y)
     && (max.z >= oMin.z && min.z <= oMax.z);
}

glm::vec3 CollisionObject::getCollisionNormal(CollisionObjectPtr other) {
    glm::vec3 min = boundingBox.getMin();
    glm::vec3 oMin = other->boundingBox.getMin();
    glm::vec3 max = boundingBox.getMax();
    glm::vec3 oMax = other->boundingBox.getMax();

    oMax.y += (position.x - oMin.x) / (oMax.x - oMin.x) * (other->shearX2 - other->shearX1) + other->shearX1;
    oMax.y += (position.z - oMin.z) / (oMax.z - oMin.z) * (other->shearZ2 - other->shearZ1) + other->shearZ1;

    glm::vec3 best = glm::vec3(0,0,0);
    float bestDist = 10.0f; //max float? do something with this.

    if(max.x - oMin.x < bestDist) {
        bestDist = max.x - oMin.x;
        best = glm::vec3(-1, 0, 0);
    }
    if(oMax.x - min.x < bestDist) {
        bestDist = oMax.x - min.x;
        best = glm::vec3(1, 0, 0);
    }
    if(max.y - oMin.y < bestDist) {
        bestDist = max.y - oMin.y;
        best = glm::vec3(0, -1, 0);
    }
    if(oMax.y - min.y < bestDist) {
        bestDist = oMax.y - min.y;
        best = glm::vec3(0, 1, 0);
    }
    if(max.z - oMin.z < bestDist) {
        bestDist = max.z - oMin.z;
        best = glm::vec3(0, 0, -1);
    }
    if(oMax.z - min.z < bestDist) {
        bestDist = oMax.z - min.z;
        best = glm::vec3(0, 0, 1);
    }

    return best;
}

float CollisionObject::getCollisionDistance(CollisionObjectPtr other) {
    glm::vec3 min = boundingBox.getMin();
    glm::vec3 oMin = other->boundingBox.getMin();
    glm::vec3 max = boundingBox.getMax();
    glm::vec3 oMax = other->boundingBox.getMax();

    oMax.y += (position.x - oMin.x) / (oMax.x - oMin.x) * (other->shearX2 - other->shearX1) + other->shearX1;
    oMax.y += (position.z - oMin.z) / (oMax.z - oMin.z) * (other->shearZ2 - other->shearZ1) + other->shearZ1;

    float bestDist = 10.0f; //max float? do something with this.

    if(max.x - oMin.x < bestDist) {
        bestDist = max.x - oMin.x;
    }
    if(oMax.x - min.x < bestDist) {
        bestDist = oMax.x - min.x;
    }
    if(max.y - oMin.y < bestDist) {
        bestDist = max.y - oMin.y;
    }
    if(oMax.y - min.y < bestDist) {
        bestDist = oMax.y - min.y;
    }
    if(max.z - oMin.z < bestDist) {
        bestDist = max.z - oMin.z;
    }
    if(oMax.z - min.z < bestDist) {
        bestDist = oMax.z - min.z;
    }

    return bestDist;
}

void CollisionObject::setShear(float x1, float x2, float z1, float z2) {
    shearX1 = x1;
    shearX2 = x2;
    shearZ1 = z1;
    shearZ2 = z2;
}

