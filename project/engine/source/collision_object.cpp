#include "collision_object.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"
#include "glm/glm.hpp"

CollisionObject::CollisionObject() : _canCollide(false){}

CollisionObject::CollisionObject(glm::vec3 _position)
    : position(_position), _canCollide(true), collisionID(0), collideWithID(0){}

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
