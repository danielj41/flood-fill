#include "bounding_box.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

const int BoundingBox::NUMBER_VERTICES = 8;

BoundingBox::BoundingBox(){}

BoundingBox::BoundingBox(glm::vec3 _max, glm::vec3 _min)
    : max(_max), min(_min), collisionID(1), collideWithID(1) {

    vertices[0] = glm::vec3(max.x, min.y, max.z);
    vertices[1] = glm::vec3(max.x, max.y, max.z);
    vertices[2] = glm::vec3(min.x, max.y, max.z);
    vertices[3] = glm::vec3(min.x, min.y, max.z);

    vertices[4] = glm::vec3(max.x, min.y, min.z);
    vertices[5] = glm::vec3(max.x, max.y, min.z);
    vertices[6] = glm::vec3(min.x, max.y, min.z);
    vertices[7] = glm::vec3(min.x, min.y, min.z);
}

glm::vec3 BoundingBox::getMax(){
    return max;
}

glm::vec3 BoundingBox::getMin(){
    return min;
}

glm::vec3 BoundingBox::getVertex(int index){
    ASSERT(index >= NUMBER_VERTICES || index < 0,
            "Index " << index << " out of range: Bounding Box getVertex");
    return vertices[index];
}

int BoundingBox::getCollisionID(){
    return collisionID;
}

int BoundingBox::getCollideWithID(){
    return collideWithID;
}

void BoundingBox::setCollisionID(int ID){
    collisionID = ID;
}

void BoundingBox::setCollideWithID(int ID){
    collideWithID = ID;
}
