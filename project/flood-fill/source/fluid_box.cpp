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
	size(glm::vec3(1)) {}

void FluidBox::setup() {  
  INFO("Creating a box ...");
  
  fluidBox = new Object(
				   LoadManager::getMesh("cube.obj"),
				   MaterialManager::getMaterial("FlatBlue"));

  fluidBox->translate(position);
  timer = 0.0f;
  visible = false;
  
  setCollisionID(1);
  setCanCollide(true);

  setBoundingBox(BoundingBox(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(-1.0f,-1.0f,-1.0f)));
  getBoundingBox()->setPosition(position);

  CollisionManager::addCollisionObjectToGrid(this);
  ((LevelTemplate *)Director::getScene())->getTypeGrid()->setValue(position.x, position.y, position.z, LevelTemplate::SOLID_CUBE);

  Uniform3DGrid<CollisionObject *> *grid = CollisionManager::getGrid();
  createNew(position + glm::vec3(grid->getEdgeSizeX(), 0.0f, 0.0f));
  createNew(position + glm::vec3(-grid->getEdgeSizeX(), 0.0f, 0.0f));
  createNew(position + glm::vec3(0.0f, 0.0f, grid->getEdgeSizeZ()));
  createNew(position + glm::vec3(0.0f, 0.0f, -grid->getEdgeSizeZ()));
}

void FluidBox::update(){
  float dTime = ((float) TimeManager::getDeltaTime());
  if(!visible) {
    timer += dTime;
    if(timer > 2.0f) {
      visible = true;
      RenderEngine::addObject(fluidBox);
    }
  } 
}

void FluidBox::createNew(glm::vec3 newPos) {
  Uniform3DGrid<CollisionObject *> *grid = CollisionManager::getGrid();
  if(grid->inGrid(newPos.x, newPos.y, newPos.z) && grid->getValue(newPos.x, newPos.y, newPos.z) == NULL) {
    FluidBox *box = new FluidBox(newPos);
    box->setup();
    Director::getScene()->addGameObject(box);
  }
}

void FluidBox::collided(CollisionObject * collidedWith){
  
  switch (collidedWith->getCollisionID()){
  default:
	break;
  } 
}
