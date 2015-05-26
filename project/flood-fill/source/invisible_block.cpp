#include "invisible_block.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

InvisibleBlock::InvisibleBlock(glm::vec3 _position) : GameObject(), CollisionObject(_position), position(_position) {}

void InvisibleBlock::setup() {
    /*  setCollisionID(512);
    setCollideWithID(2);
    setCanCollide(true);

    setBoundingBox(BoundingBox(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(-1.0f,-1.0f,-1.0f)));
    getBoundingBox()->setPosition(position);*/
}

void InvisibleBlock::update() {
}

void collided(CollisionObjectPtr collidedWith) {
}
    
