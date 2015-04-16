#include "obstacle.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#include "load_manager.hpp"
#include "bounding_box.hpp"
#include "time_manager.hpp"
#include "collision_manager.hpp"
#include "director.hpp"

Obstacle::Obstacle(glm::vec3 _position, glm::vec3 _movementDirection,
                    float _speed)
    : GameObject(), CollisionObject(),
      position(_position), movementDirection(_movementDirection), speed(_speed),
      shrink(false), shrinkRate(0.5), size(glm::vec3(1)){}

void Obstacle::setup() {
    INFO("Creating an obstacle plane...");

    box = new Object(
                    LoadManager::getMesh("cube.obj"),
                    LoadManager::getShader("vertex.glsl", "fragment.glsl"));
    //box->rotate(45.0f, glm::vec3(0, 1, 0));
    box->translate(position);

    setBoundingBox(BoundingBox(box->getMesh()->getMaxLimits(),
                               box->getMesh()->getMinLimits()));
    boundingBox.setModelMatrix(box->getModelMatrix());

    setCollisionID(4);
    setCollideWithID(1 | 2);
}

void Obstacle::update() {
    float dTime = ((float) TimeManager::getDeltaTime());
    box->loadIdentity();

    if(!shrink){
        position += movementDirection*speed*dTime;
    }
    else{
        size -= shrinkRate*speed*dTime;
        if(size.x <= 0.0f){
            Director::getScene()->removeGameObject(this);
            return;
        }

        box->scale(glm::vec3(size));
    }

    box->translate(position);
    boundingBox.setModelMatrix(box->getModelMatrix());
}

void Obstacle::draw() {
    box->draw();
    boundingBox.draw();
}

void Obstacle::collided(CollisionObject * collidedWith){
    if(collidedWith->getCollisionID() == 1){
        movementDirection = -movementDirection;
    }

    if(collidedWith->getCollisionID() == 2){
        CollisionManager::removeCollisionObject(this);
        shrink = true;
    }
}
