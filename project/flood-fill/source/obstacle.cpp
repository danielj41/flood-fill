#include "obstacle.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#include "load_manager.hpp"
#include "bounding_box.hpp"

Obstacle::Obstacle(glm::vec3 _movementDirection, float _speed)
    : GameObject(), CollisionObject(),
            movementDirection(_movementDirection), speed(_speed){}

void Obstacle::setup() {
    INFO("Creating an obstacle plane...");

    box = new Object(
                    LoadManager::getMesh("cube.obj"),
                    LoadManager::getShader("vertex.glsl", "fragment.glsl"));
    box->rotate(45.0f, glm::vec3(0, 1, 0));
    box->translate(glm::vec3(-2, 0, -3));

    setBoundingBox(BoundingBox(box->getMesh()->getMaxLimits(),
                               box->getMesh()->getMinLimits()));
    boundingBox.setModelMatrix(box->getModelMatrix());
}

void Obstacle::update() {
    box->translate(movementDirection*speed);
    boundingBox.setModelMatrix(box->getModelMatrix());
}

void Obstacle::draw() {
    box->draw();
    boundingBox.draw();
}

void Obstacle::collided(CollisionObject * collidedWith){
    INFO("HIT OBSTACLE!!!");
}
