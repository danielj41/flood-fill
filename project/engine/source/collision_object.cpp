#include "collision_object.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

CollisionObject::CollisionObject() : _canCollide(false){}

CollisionObject::CollisionObject(BoundingBox box)
    : boundingBox(box), _canCollide(true){}

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

