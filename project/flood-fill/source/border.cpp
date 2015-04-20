#include "border.hpp"

#include "glm/glm.hpp"
#include <iostream>
#include "debug_macros.h"

#include "box.hpp"
#include "collision_manager.hpp"
#include "collision_object.hpp"

void Border::createBorder(){ 
  INFO("Creating Border ...");

  glm::vec3 pos;
  Box * obj;

  // Back & Front Wall
  for (int i = 0; i < NUMBLKX; i++) {
	for (int j = 0; j < NUMBLKY; j++) {
	  pos = glm::vec3(MINX + E_LEN/2 + i * E_LEN, 
					  MINY + E_LEN/2 + j * E_LEN, 
					  MAXZ - E_LEN/2);
	  obj = new Box(pos, glm::vec3(0), 0);
	  obj->setup();
	  obj->setCollisionID(1);
	  CollisionManager::addCollisionObjectToGrid(obj);	 	  

	  pos = glm::vec3(MINX + E_LEN/2 + i * E_LEN, 
					  MINY + E_LEN/2 + j * E_LEN, 
					  MINZ + E_LEN/2);
	  obj = new Box(pos, glm::vec3(0), 0);
	  obj->setup(); 
	  obj->setCollisionID(1);
	  CollisionManager::addCollisionObjectToGrid(obj);	  
	}
  }

  // Left & Right Wall
  for (int i = 1; i < NUMBLKZ - 1; i++) {
	for (int j = 0; j < NUMBLKY; j++) {
	  pos = glm::vec3(MINX + E_LEN/2, 
					  MINY + E_LEN/2 + j * E_LEN, 
					  MINZ + E_LEN/2 + i*E_LEN);
	  obj = new Box(pos, glm::vec3(0), 0);
	  obj->setup(); 
	  obj->setCollisionID(1);
	  CollisionManager::addCollisionObjectToGrid(obj);
	  
	  pos = glm::vec3(MAXX - E_LEN/2, 
					  MINY + E_LEN/2 + j * E_LEN, 
					  MINZ + E_LEN/2 + i*E_LEN);
	  obj = new Box(pos, glm::vec3(0), 0);
	  obj->setup(); 
	  obj->setCollisionID(1);
	  CollisionManager::addCollisionObjectToGrid(obj);
	  
	}
  }
}
	  
		


