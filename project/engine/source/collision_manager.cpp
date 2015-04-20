#include "collision_manager.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"
#include "collision_object.hpp"
#include "glm/glm.hpp"

std::list<CollisionObject *> CollisionManager::collisionObjects;
Uniform3DGrid<CollisionObject *> CollisionManager::grid;

void CollisionManager::detectCollisions(){
    INFO("Detecting Collisions...");
		  
    std::list<CollisionObject *> tempList = collisionObjects;

    for(std::list<CollisionObject *>::iterator a = tempList.begin(); a != tempList.end(); a++){
	  // TODO: Check if there is a collision between collision object and grid
	  if (checkCollision(*a)){		  
	  }

    }
}

void CollisionManager::addCollisionObject(CollisionObject * object){
    if(object->canCollide()){
        INFO("Adding collision object to the grid");
	   
		glm::vec3 pos = object->getPosition();
		grid.setValue(pos.x, pos.y, pos.z, object);

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


bool CollisionManager::checkCollision(CollisionObject * aObject){
  //This function checks if a has any conflicts with other collision objects in the grid    
	  
  glm::vec3 pos = aObject->getPosition();
  CollisionObject *bObject = grid.getValue(pos.x, pos.y, pos.z);
  
  
  if (aObject->getCollisionID() & bObject->getCollisionID()){
	aObject->collided(bObject);
	bObject->collided(aObject);
	return true;
  }
	
	return false;
}



void CollisionManager::initGrid(int x, int y, int z, glm::vec3 min, glm::vec3 max){
    INFO("Initializing the Grid");
    grid = Uniform3DGrid<CollisionObject *>(x, y, z, min.x, max.x, min.y, max.y, min.z, max.z);
}

 
