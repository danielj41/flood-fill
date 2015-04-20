#include "box.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#include "load_manager.hpp"
#include "time_manager.hpp"
#include "collision_manager.hpp"
#include "director.hpp"
#include "material_manager.hpp"
#include "render_engine.hpp"

Box::Box(glm::vec3 _position, glm::vec3 _movementDirection, float _speed)
  : GameObject(), CollisionObject(_position),
	position(_position), movementDirection(_movementDirection),
	size(glm::vec3(1)), speed(_speed){}

void Box::setup() {  
  INFO("Creating a box ...");
  
  box = new Object(
				   LoadManager::getMesh("cube.obj"),
				   MaterialManager::getMaterial("FlatGrey"));

  box->translate(position);
  
  RenderEngine::addObject(box);
  
  setCollisionID(1);
  setCanCollide(true);
}

void Box::update(){

  float dTime = ((float) TimeManager::getDeltaTime());
  
  position += movementDirection*speed*dTime;
  
  box->loadIdentity();
  box->translate(position);  
}

void Box::collided(CollisionObject * collidedWith){
  
  switch (collidedWith->getCollisionID()){
  default:
	break;
  } 
}
