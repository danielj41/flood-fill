#include "fluid_projectile.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#include "load_manager.hpp"
#include "time_manager.hpp"
#include "collision_manager.hpp"
#include "director.hpp"
#include "material_manager.hpp"
#include "render_engine.hpp"

FluidProjectile::FluidProjectile(glm::vec3 _position, glm::vec3 _movementDirection)
  : GameObject(), CollisionObject(_position),
	position(_position), movementDirection(_movementDirection),
	size(glm::vec3(0.1)){}

void FluidProjectile::setup() {  
  INFO("Shooting a fluid projectile ...");
  
  fluidProjectile = new Object(
				   LoadManager::getMesh("cube.obj"),
				   MaterialManager::getMaterial("FlatGrey"));

  fluidProjectile->translate(position);
  
  RenderEngine::addObject(fluidProjectile);
  
  setCollisionID(0);
  setCollideWithID(1);
  setCanCollide(true);

  position += movementDirection;
  setPosition(position);
}

void FluidProjectile::update(){
  float dTime = ((float) TimeManager::getDeltaTime());
  
  position += movementDirection*dTime * 10.0f;
  setPosition(position);
  movementDirection.y -= dTime;
  
  fluidProjectile->loadIdentity();
  fluidProjectile->translate(position);  
}

void FluidProjectile::collided(CollisionObject * collidedWith){
  switch (collidedWith->getCollisionID()) {
  case 1:
    Director::getScene()->removeGameObject(this);
    CollisionManager::removeCollisionObject(this);
    RenderEngine::removeObject(fluidProjectile);
    break;
  default:
	break;
  } 
}
