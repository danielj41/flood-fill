#include "collision_object.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"
#include "glm/glm.hpp"

CollisionObject::CollisionObject() : _canCollide(false), 
boundingBox(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, -1.0f)) {}

CollisionObject::CollisionObject(glm::vec3 _position)
    : position(_position), _canCollide(true), collisionID(0), collideWithID(0),
      boundingBox(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(-1.0f, -1.0f, -1.0f)){}

bool CollisionObject::canCollide(){
    return _canCollide;
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

void CollisionObject::setBoundingBox(BoundingBox _boundingBox) {
    boundingBox = _boundingBox;
}

BoundingBox* CollisionObject::getBoundingBox() {
    return &boundingBox;
}

bool CollisionObject::checkCollision(CollisionObject *other) {
    glm::vec3 min = boundingBox.getMin();
    glm::vec3 oMin = other->boundingBox.getMin();
    glm::vec3 max = boundingBox.getMax();
    glm::vec3 oMax = other->boundingBox.getMax();

    return (max.x >= oMin.x && min.x <= oMax.x)
     && (max.y >= oMin.y && min.y <= oMax.y)
     && (max.z >= oMin.z && min.z <= oMax.z);
}

glm::vec3 CollisionObject::getCollisionNormal(CollisionObject *other) {
    glm::vec3 min = boundingBox.getMin();
    glm::vec3 oMin = other->boundingBox.getMin();
    glm::vec3 max = boundingBox.getMax();
    glm::vec3 oMax = other->boundingBox.getMax();

    glm::vec3 best = glm::vec3(0,0,0);
    float bestDist = 1000.0f; //max float? do something with this.

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


