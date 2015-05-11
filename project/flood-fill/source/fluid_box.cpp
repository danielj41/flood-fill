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
#include "global_variables.hpp"

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
				   LoadManager::getMesh("cube.obj"),
				   MaterialManager::getMaterial(color));

  fluidBox->translate(position);
  timer = 0.0f;
  visible = false;
  deleting = false;
  
  setCollisionID(64);
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
      RenderEngine::getRenderElement("regular")->addObject(fluidBox);
      getBoundingBox()->setPosition(position);
    }
  } 
  if(deleting) {
    timer += dTime;
    position -= glm::vec3(0, dTime * 4.0f, 0);
    fluidBox->loadIdentity();
    fluidBox->translate(position);
    getBoundingBox()->setPosition(position);
    if(timer > 0.5f) {
      Director::getScene()->removeGameObject(this);
      RenderEngine::getRenderElement("regular")->removeObject(fluidBox);
      CollisionManager::removeCollisionObjectFromGrid(this);
    }
  }
}

void FluidBox::collided(CollisionObject * collidedWith){
  
  switch (collidedWith->getCollisionID()){
  case 4:
    if(glfwGetMouseButton(Global::window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS && !deleting) {
      deleting = true;
      timer = 0.0f;
    }
	break;
  } 
}
