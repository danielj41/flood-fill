#include "collision_manager.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"
#include "collision_object.hpp"
#include "glm/glm.hpp"


std::list<CollisionObjectPtr> CollisionManager::collisionObjects;
Uniform3DGrid<CollisionObjectPtr> CollisionManager::grid;

void CollisionManager::detectCollisions(){
    INFO("Detecting Collisions...");
		  
    std::list<CollisionObjectPtr> tempList = collisionObjects;

    for(std::list<CollisionObjectPtr>::iterator a = tempList.begin(); a != tempList.end(); a++){
	  checkCollision(*a);
    }
}

void CollisionManager::addCollisionObjectToList(CollisionObjectPtr object){
  if (object->canCollide()) {
	INFO("Adding collision object to collision list");
	collisionObjects.push_back(object);
  } else {
	DEBUG("You cannot add this object to the collision list because its canCollide == false");
  }
}

void CollisionManager::addCollisionObjectToGrid(CollisionObjectPtr object){
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

void CollisionManager::removeCollisionObjectFromGrid(CollisionObjectPtr object){
  glm::vec3 pos = object->getPosition();
  grid.setValue(pos.x, pos.y, pos.z, NULL_PTR);
}

void CollisionManager::removeCollisionObjectFromGrid(CollisionObject* object){
  glm::vec3 pos = object->getPosition();
  grid.setValue(pos.x, pos.y, pos.z, NULL_PTR);
}


void CollisionManager::removeCollisionObject(CollisionObjectPtr object){
    INFO("Removing object from the collision list...");
    for(std::list<CollisionObjectPtr>::iterator it = collisionObjects.begin();
            it != collisionObjects.end(); it++){
        if(*it == object){
            collisionObjects.erase(it);
            INFO("Object Removed from collision list");
            return;
        }
    }
    DEBUG("Could not find the object in the collision list");
}

void CollisionManager::removeCollisionObject(CollisionObject* object){
    INFO("Removing object from the collision list...");
    for(std::list<CollisionObjectPtr>::iterator it = collisionObjects.begin();
            it != collisionObjects.end(); it++){
        if((*it).get() == object){
            collisionObjects.erase(it);
            INFO("Object Removed from collision list");
            return;
        }
    }
    DEBUG("Could not find the object in the collision list");
}

void CollisionManager::checkCollision(CollisionObjectPtr aObject){
// This function checks if a has any conflicts with other collision objects in the grid    

  // fix this later to include all nearby cubes, or calculate it with the bounding box
  // this causes you to be able to jump diagonally through things.

  int xDir, yDir, zDir;
  glm::vec3 pos = aObject->getBoundingBox()->getPosition();
  glm::vec3 roundPos(grid.getRoundX(pos.x), grid.getRoundY(pos.y), grid.getRoundZ(pos.z));
  xDir = pos.x > roundPos.x ? 1 : -1;
  yDir = pos.y > roundPos.y ? 1 : -1;
  zDir = pos.z > roundPos.z ? 1 : -1;

  checkCollisionWithOffset(aObject, 0, 0, 0);
  checkCollisionWithOffset(aObject, 0, yDir, 0);
  checkCollisionWithOffset(aObject, xDir, 0, 0);
  checkCollisionWithOffset(aObject, 0, 0, zDir);
  checkCollisionWithOffset(aObject, xDir, yDir, 0);
  checkCollisionWithOffset(aObject, 0, yDir, zDir);
  checkCollisionWithOffset(aObject, xDir, 0, zDir);
  checkCollisionWithOffset(aObject, xDir, yDir, zDir);
}

void CollisionManager::checkCollisionWithOffset(CollisionObjectPtr aObject, int x, int y, int z) {
  glm::vec3 pos = aObject->getBoundingBox()->getPosition();

  if(grid.inGrid(pos.x + x * grid.getEdgeSizeX(), pos.y + y * grid.getEdgeSizeY(), pos.z + z * grid.getEdgeSizeZ())) {
    CollisionObjectPtr bObject = grid.getValue(pos.x + x * grid.getEdgeSizeX(), pos.y + y * grid.getEdgeSizeY(), pos.z + z * grid.getEdgeSizeZ());

    if ( bObject != NULL_PTR && aObject->getCollideWithID() & bObject->getCollisionID() && aObject->checkCollision(bObject)) {
      aObject->collided(bObject);
      bObject->collided(aObject);
    }
  }
}

void CollisionManager::initGrid(int x, int y, int z, glm::vec3 min, glm::vec3 max){
  INFO("Initializing the Grid");
  grid = Uniform3DGrid<CollisionObjectPtr>(x, y, z, min.x, max.x, min.y, max.y, min.z, max.z); 
  CollisionObjectPtr obj(NULL_PTR);
  grid.initialize(obj);
}

Uniform3DGrid<CollisionObjectPtr>* CollisionManager::getGrid() {
  return &grid;
}
