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
	  checkCollision(*a);
    }
}

void CollisionManager::addCollisionObjectToList(CollisionObject * object){
  if (object->canCollide()) {
	INFO("Adding collision object to collision list");
	collisionObjects.push_back(object);
  } else {
	DEBUG("You cannot add this object to the collision list because its canCollide == false");
  }
}

void CollisionManager::addCollisionObjectToGrid(CollisionObject * object){
  if (object->canCollide()){
        INFO("Adding collision object to the grid");
	   
		glm::vec3 pos = object->getPosition();
		grid.setValue(pos.x, pos.y, pos.z, object);

   }
  else {
	DEBUG("You cannot add this object to the collision grid. " <<
		  "This object canCollide == false");
  }
}

void CollisionManager::removeCollisionObject(CollisionObject * object){
    INFO("Removing object from the collision list...");
    for(std::list<CollisionObject *>::iterator it = collisionObjects.begin();
            it != collisionObjects.end(); it++){
        if(*it == object){
            collisionObjects.erase(it);
            INFO("Object Removed from collision list");
            return;
        }
    }
    DEBUG("Could not find the object in the collision list");
}

void CollisionManager::checkCollision(CollisionObject * aObject){
// This function checks if a has any conflicts with other collision objects in the grid    

  glm::vec3 pos = aObject->getPosition();
  CollisionObject *bObject = grid.getValue(pos.x, pos.y, pos.z);

  if ( bObject != NULL && aObject->getCollideWithID() & bObject->getCollisionID()) {
	aObject->collided(bObject);
	bObject->collided(aObject);	
  }
}

void CollisionManager::initGrid(int x, int y, int z, glm::vec3 min, glm::vec3 max){
  INFO("Initializing the Grid");
  grid = Uniform3DGrid<CollisionObject *>(x, y, z, min.x, max.x, min.y, max.y, min.z, max.z); 
}

Uniform3DGrid<CollisionObject *>* CollisionManager::getGrid() {
  return &grid;
}
