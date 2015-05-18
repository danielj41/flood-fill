#include "obstacle.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#include "load_manager.hpp"
#include "bounding_box.hpp"
#include "time_manager.hpp"
#include "collision_manager.hpp"
#include "director.hpp"
#include "material_manager.hpp"
#include "render_engine.hpp"

Obstacle::Obstacle(glm::vec3 _position, glm::vec3 _movementDirection,
                    float _speed)
    : GameObject(), CollisionObject(),
      position(_position), movementDirection(_movementDirection),
      size(glm::vec3(1)), speed(_speed), shrink(false), shrinkRate(0.5){}

void Obstacle::setup() {
    INFO("Creating an obstacle plane...");

    box = ObjectPtr(new Object(
                    LoadManager::getMesh("cube.obj"),
                    MaterialManager::getMaterial("FlatGrey")));
    //box->rotate(45.0f, glm::vec3(0, 1, 0));
    box->translate(position);

    RenderEngine::addObject(box);

    setBoundingBox(BoundingBox(box->getMesh()->getMaxLimits(),
                               box->getMesh()->getMinLimits()));
    boundingBox.setModelMatrix(box->getModelMatrix());

    setCollisionID(4);
    setCollideWithID(1 | 2 | 4);
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
            RenderEngine::removeObject(box);
            return;
        }

        box->scale(glm::vec3(size));
    }

    box->translate(position);
    boundingBox.setModelMatrix(box->getModelMatrix());
}

void Obstacle::collided(CollisionObjectPtr collidedWith){
    if(collidedWith->getCollisionID() == 1 ||
       collidedWith->getCollisionID() == 4){
        movementDirection = -movementDirection;
    }
    else if(collidedWith->getCollisionID() == 2){
        CollisionManager::removeCollisionObject(this);
        shrink = true;
    }
}
