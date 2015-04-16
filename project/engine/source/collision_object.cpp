#include "collision_object.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

CollisionObject::CollisionObject() : _canCollide(false){}

CollisionObject::CollisionObject(BoundingBox box)
    : boundingBox(box), _canCollide(true), collisionID(0), collideWithID(0){}

bool CollisionObject::canCollide(){
    return _canCollide;
}

void CollisionObject::setBoundingBox(BoundingBox box){
    boundingBox = box;
    _canCollide = true;
}

BoundingBox CollisionObject::getBoundingBox(){
    ASSERT(_canCollide == true, "There is no Bounding Box for this object!");

    return boundingBox;
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
