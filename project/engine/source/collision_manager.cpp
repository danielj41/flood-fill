#include "collision_manager.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

std::list<CollisionObject *> CollisionManager::collisionObjects;

void CollisionManager::detectCollisions(){
    INFO("Detecting Collisions...");

    std::list<CollisionObject *> tempList = collisionObjects;

    for(std::list<CollisionObject *>::iterator a = tempList.begin(),
        c = tempList.begin(); a != tempList.end(); a++){
        for(std::list<CollisionObject *>::iterator b = ++c;
                b != tempList.end(); b++){
            if(!checkCollision(*a, *b))
                checkCollision(*b, *a);
        }
    }
}

void CollisionManager::addCollisionObject(CollisionObject * object){
    if(object->canCollide()){
        INFO("Adding object to the Collision list");
        collisionObjects.push_front(object);
    }
    else {
        DEBUG("You can't add this object to the collision list. " <<
                "This object does not have a Bounding Box");
    }
}

void CollisionManager::removeCollisionObject(CollisionObject * object){
    INFO("Removing object from the collision list...");
    for(std::list<CollisionObject *>::iterator it = collisionObjects.begin();
            it != collisionObjects.end(); it++){
        if(*it == object){
            collisionObjects.erase(it);
            INFO("Object Removed from collision list!");
            return;
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

//TODO: Improve the performace of this method
//TODO: Refactor the magic numbers
bool CollisionManager::checkCollision(CollisionObject * aObject,
                                      CollisionObject * bObject){
    /*This funcition checks if any point of B's bounding box is inside the
      A's boudning box*/

    //Check the collision bit mask
    if((aObject->getCollideWithID() & bObject->getCollisionID()) == 0){
        return false;
    }

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
            return true;
        }
    }

    return false;
}
