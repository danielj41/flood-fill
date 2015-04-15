#include "collision_manager.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

std::list<GameObject *> CollisionManager::gameObjects;

void CollisionManager::detectCollisions(){
    INFO("Detecting Collisions...");

    std::list<GameObject *> tempList = gameObjects;

    for(std::list<GameObject *>::iterator a = tempList.begin(),
        c = tempList.begin(); a != tempList.end(); a++){
        for(std::list<GameObject *>::iterator b = ++c;
                b != tempList.end(); b++){
            checkCollision(*a, *b);
        }
    }
}

void CollisionManager::addGameObject(GameObject * object){
    if(object->canCollide()){
        INFO("Adding object to the Collision list");
        gameObjects.push_front(object);
    }
    else {
        DEBUG("You can't add this object to the collision list. " <<
                "This object does not have a Bounding Box");
    }
}

void CollisionManager::removeGameObject(GameObject * object){
    INFO("Removing object from the collision list...");
    for(std::list<GameObject *>::iterator it = gameObjects.begin();
            it != gameObjects.end(); it++){
        if(*it == object){
            gameObjects.erase(it);
            INFO("Object Removed from collision list!");
            break;
        }
    }
    DEBUG("Could not find the object in the collision list");
}

float getPlaneSide(glm::vec3 normal, float d, glm::vec3 point){
    float value = glm::dot(normal, point) + d;
    if(value < 0)
        return -1;
    else
        return 1;
}

float getDFromPlaneEquation(glm::vec3 normal, glm::vec3 point){
    return -1*(glm::dot(normal, point));
}

//TODO: Refactor the magic numbers
void CollisionManager::checkCollision(GameObject * aObject, GameObject * bObject){
    //Checking collision of B in A
    BoundingBox a = aObject->getBoundingBox();
    BoundingBox b = bObject->getBoundingBox();


    //Bounding Box A Normals (Pointing to inside the Box)
    glm::vec3 normal[6];

    /*Top Plane*/
    normal[0] = glm::cross(a.getVertex(2) - a.getVertex(1),
                           a.getVertex(5) - a.getVertex(1));
    /*Bottom Plane*/
    normal[1] = glm::cross(a.getVertex(4) - a.getVertex(0),
                           a.getVertex(3) - a.getVertex(0));
    /*Left Plane*/
    normal[2] = glm::cross(a.getVertex(7) - a.getVertex(3),
                           a.getVertex(2) - a.getVertex(3));
    /*Right Plane*/
    normal[3] = glm::cross(a.getVertex(5) - a.getVertex(1),
                           a.getVertex(0) - a.getVertex(1));
    /*Front Plane*/
    normal[4] = glm::cross(a.getVertex(1) - a.getVertex(2),
                           a.getVertex(3) - a.getVertex(2));
    /*Back Plane*/
    normal[5] = glm::cross(a.getVertex(6) - a.getVertex(5),
                           a.getVertex(4) - a.getVertex(5));

    // D from the plane equation
    float d[6];

    /*Top Plane*/
    d[0] = getDFromPlaneEquation(normal[0], a.getVertex(2));
    /*Bottom Plane*/
    d[1] = getDFromPlaneEquation(normal[1], a.getVertex(4));
    /*Left Plane*/
    d[2] = getDFromPlaneEquation(normal[2], a.getVertex(7));
    /*Right Plane*/
    d[3] = getDFromPlaneEquation(normal[3], a.getVertex(5));
    /*Front Plane*/
    d[4] = getDFromPlaneEquation(normal[4], a.getVertex(1));
    /*Back Plane*/
    d[5] = getDFromPlaneEquation(normal[5], a.getVertex(6));


    for(int i = 0; i < 8; i++){
        int pointsInside = 0;
        for(int j = 0; j < 6; j+=2){
            if(getPlaneSide(normal[j + 0], d[j + 0], b.getVertex(i)) ==
               getPlaneSide(normal[j + 1], d[j + 1], b.getVertex(i)))
               pointsInside++;
            else
                break;
        }
        if(pointsInside == 3){
            //TODO: Fix when two objects collide, and each one removes the other
            // i.e. A removes B and B removes A
            aObject->collided(bObject);
            bObject->collided(aObject);
            break;
        }
    }
}
