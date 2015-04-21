#include "fluid_box.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#include "load_manager.hpp"
#include "time_manager.hpp"
#include "collision_manager.hpp"
#include "director.hpp"
#include "material_manager.hpp"
#include "render_engine.hpp"

FluidBox::FluidBox(glm::vec3 _position, glm::vec3 _movementDirection, float _speed)
  : GameObject(), CollisionObject(_position),
	position(_position), movementDirection(_movementDirection),
	size(glm::vec3(1)), speed(_speed){}

void FluidBox::setup() {  
  INFO("Creating a box ...");
  
  fluidBox = new Object(
				   LoadManager::getMesh("cube.obj"),
				   MaterialManager::getMaterial("FlatBlue"));

  fluidBox->translate(position);
  
  RenderEngine::addObject(fluidBox);
  
  setCollisionID(1);
  setCanCollide(true);
}

void FluidBox::update(){

  float dTime = ((float) TimeManager::getDeltaTime());
  
  position += movementDirection*speed*dTime;
  
  fluidBox->loadIdentity();
  fluidBox->translate(position);  
}

void FluidBox::collided(CollisionObject * collidedWith){
  
  switch (collidedWith->getCollisionID()){
  default:
	break;
  } 
}
