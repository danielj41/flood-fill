#include "game_object.hpp"

GameObject::GameObject() : _canCollide(false){}

bool GameObject::canCollide(){
    return _canCollide;
}

void GameObject::setBoundingBox(BoundingBox _boundingBox){
    boundingBox = _boundingBox;
    _canCollide = true;
}
