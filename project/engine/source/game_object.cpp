#include "game_object.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

GameObject::GameObject() : _canCollide(false){}

bool GameObject::canCollide(){
    return _canCollide;
}

void GameObject::setBoundingBox(BoundingBox _boundingBox){
    boundingBox = _boundingBox;
    _canCollide = true;
}

BoundingBox GameObject::getBoundingBox(){
    ASSERT(_canCollide == true, "There is no Bounding Box for this object!");
    return boundingBox;
}

void GameObject::collided(GameObject * collidedWith){
    DEBUG("HIT: Empty collided method! You should implement your own method on"
            << " the child class!");
}
