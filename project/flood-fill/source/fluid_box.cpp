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
#include "uniform_3d_grid.hpp"
#include "level_template.hpp"

FluidBox::FluidBox(glm::vec3 _position)
  : GameObject(), CollisionObject(_position),
  position(_position),
  color("FlatBlue"),
  size(glm::vec3(1)) {}

FluidBox::FluidBox(glm::vec3 _position, std::string _color)
  : GameObject(), CollisionObject(_position),
	position(_position),
  color(_color),
	size(glm::vec3(1)) {}

void FluidBox::setup() {  
  INFO("Creating a box ...");
  
  fluidBox = new Object(
				   LoadManager::getMesh("plane2.obj"),
				   MaterialManager::getMaterial(color));

  fluidBox->scale(glm::vec3(1,1.0f,1));
  fluidBox->translate(position + glm::vec3(0, 1.0f, 0));
  timer = 0.0f;
  visible = false;
  
  setCollisionID(1);
  setCanCollide(true);

  setBoundingBox(BoundingBox(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(-1.0f,-1.0f,-1.0f)));
  getBoundingBox()->setPosition(position - glm::vec3(0, 2.0f, 0));

  CollisionManager::addCollisionObjectToGrid(this);

  
}

void FluidBox::update(){
  
  float dTime = ((float) TimeManager::getDeltaTime());
  if(!visible) {
    timer += dTime;
    getBoundingBox()->setPosition(position - (1.0f - timer/1.55f) * glm::vec3(0, 2.0f, 0));
    if(timer > 1.55f) {
      visible = true;
      RenderEngine::addObject(fluidBox);
      getBoundingBox()->setPosition(position);
    }
  } 
}

void FluidBox::collided(CollisionObject * collidedWith){
  
  switch (collidedWith->getCollisionID()){
  default:
	break;
  } 
}
