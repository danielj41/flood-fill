#include "invisible_wall.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#include "bounding_box.hpp"

InvisibleWall::InvisibleWall(glm::vec3 _position, glm::vec3 _scaleVector)
    : GameObject(), CollisionObject(), position(_position),
        scaleVector(_scaleVector){}

void InvisibleWall::setup(){
    INFO("Wall Setup...");

    setCollisionID(1);
    setCollideWithID(2 | 4);

    BoundingBox b = BoundingBox(glm::vec3(1), glm::vec3(-1));
    b.scale(scaleVector);
    b.translate(position);

    setBoundingBox(b);
}

void InvisibleWall::update(){
}

void InvisibleWall::collided(CollisionObject * collidedWith){
    INFO("HIT WALL");
}
