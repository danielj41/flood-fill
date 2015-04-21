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
#include "uniform_3d_grid.hpp"
#include "box.hpp"
#include "fluid_box.hpp"

FluidProjectile::FluidProjectile(glm::vec3 _position, glm::vec3 _movementDirection)
  : GameObject(), CollisionObject(_position),
	position(_position), movementDirection(_movementDirection),
	size(glm::vec3(0.1)){}

void FluidProjectile::setup() {  
  INFO("Shooting a fluid projectile ...");
  
  fluidProjectile = new Object(
				   LoadManager::getMesh("cube.obj"),
				   MaterialManager::getMaterial("FlatGrey"));

  RenderEngine::addObject(fluidProjectile);
  
  setCollisionID(0);
  setCollideWithID(1);
  setCanCollide(true);

  origPosition = position;

  position += movementDirection;
  setPosition(position);
  fluidProjectile->translate(position);
}

void FluidProjectile::update(){
  float dTime = ((float) TimeManager::getDeltaTime());
  
  oldPosition = position;
  position += movementDirection*dTime * 10.0f;
  setPosition(position);
  movementDirection.y -= dTime;
  
  fluidProjectile->loadIdentity();
  fluidProjectile->translate(position);  
}

void FluidProjectile::collided(CollisionObject * collidedWith){
  if(collidedWith->getCollisionID() == 1) {
    Uniform3DGrid<CollisionObject *> *grid = CollisionManager::getGrid();
    glm::vec3 newPos(grid->getRoundX(oldPosition.x), grid->getRoundY(oldPosition.y), grid->getRoundZ(oldPosition.z));
    if(grid->inGrid(newPos.x, newPos.y, newPos.z) && newPos.y < origPosition.y - 1.0f) {
      FluidBox *box = new FluidBox(newPos, glm::vec3(0,0,0), 0.0f);
      box->setup();
      Director::getScene()->addGameObject(box);
      CollisionManager::addCollisionObjectToGrid(box);
    }
    Director::getScene()->removeGameObject(this);
    CollisionManager::removeCollisionObject(this);
    RenderEngine::removeObject(fluidProjectile);
  } 
}
