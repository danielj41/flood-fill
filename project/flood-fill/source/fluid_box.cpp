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

FluidBox::FluidBox(glm::vec3 _position, glm::vec3 _movementDirection, float _speed)
  : GameObject(), CollisionObject(_position),
	position(_position), movementDirection(_movementDirection),
	size(glm::vec3(1)), speed(_speed){}

void FluidBox::setup() {  
  INFO("Creating a box ...");
  
  fluidBox = new Object(
				   LoadManager::getMesh("cube.obj"),
				   MaterialManager::getMaterial("FlatBlue"));

  fluidBox->translate(position - glm::vec3(0.0f, 2.0f, 0.0f));
  
  RenderEngine::addObject(fluidBox);
  
  setCollisionID(1);
  setCanCollide(true);

  height = 0.0;
  generatedAdjacent = false;
}

void FluidBox::update(){

  float dTime = ((float) TimeManager::getDeltaTime());
  
  position += movementDirection*speed*dTime;
  height += dTime / (height * height * height + 0.5) * 5.0;
  if(height > 2.0) {
    height = 2.0;
  }
  if(height > 1.0 && !generatedAdjacent) {
    generatedAdjacent = true;
    Uniform3DGrid<CollisionObject *> *grid = CollisionManager::getGrid();
    createNew(position + glm::vec3(grid->getEdgeSizeX(), 0.0f, 0.0f));
    createNew(position + glm::vec3(-grid->getEdgeSizeX(), 0.0f, 0.0f));
    createNew(position + glm::vec3(0.0f, 0.0f, grid->getEdgeSizeZ()));
    createNew(position + glm::vec3(0.0f, 0.0f, -grid->getEdgeSizeZ()));
  }

  fluidBox->loadIdentity();
  fluidBox->translate(position - glm::vec3(0.0f, 2.0f - height, 0.0f));  
}

void FluidBox::createNew(glm::vec3 newPos) {
  Uniform3DGrid<CollisionObject *> *grid = CollisionManager::getGrid();
  if(grid->inGrid(newPos.x, newPos.y, newPos.z) && grid->getValue(newPos.x, newPos.y, newPos.z) == NULL) {
    FluidBox *box = new FluidBox(newPos, glm::vec3(0,0,0), 0.0f);
    box->setup();
    Director::getScene()->addGameObject(box);
    CollisionManager::addCollisionObjectToGrid(box);
  }
}

void FluidBox::collided(CollisionObject * collidedWith){
  
  switch (collidedWith->getCollisionID()){
  default:
	break;
  } 
}
