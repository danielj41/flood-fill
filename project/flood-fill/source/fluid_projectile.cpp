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
#include "water_surface.hpp"
#include "level_template.hpp"

#define BLUE    1
#define RED     2
#define GREEN   4
#define GREY    8

FluidProjectile::FluidProjectile(glm::vec3 _position,
  glm::vec3 _movementDirection, uint _colorMask)
  : GameObject(), CollisionObject(_position),
	position(_position), movementDirection(_movementDirection),
	size(glm::vec3(0.1)) {
    if(_colorMask & BLUE)
      color = "FlatBlue";
    else if(_colorMask & GREEN)
      color = "FlatGreen";
    else if(_colorMask & RED)
      color = "FlatRed";
    else if(_colorMask & GREY)
      color = "FlatGrey";
  }

void FluidProjectile::setup() {  
  INFO("Shooting a fluid projectile ...");
  
  fluidProjectile = new Object(
				   LoadManager::getMesh("sphere.obj"),
				   MaterialManager::getMaterial(color));

  RenderEngine::addObject(fluidProjectile);
  
  setCollisionID(0);
  setCollideWithID(1);
  setCanCollide(true);

  origPosition = position;


  position += movementDirection;
  setPosition(position);
  fluidProjectile->translate(position);

  setBoundingBox(BoundingBox(glm::vec3(0.5f,0.5f,0.5f), glm::vec3(-0.5,-0.5f,-0.5f)));
  getBoundingBox()->setPosition(position);

  hasCollided = false;
  timer = 0;
}

void FluidProjectile::update(){
  float dTime = ((float) TimeManager::getDeltaTime());
  
  position += movementDirection * dTime * 10.0f;
  oldPosition = position - 0.5f * movementDirection;
  setPosition(position);
  movementDirection.y -= dTime;
  
  fluidProjectile->loadIdentity();
  fluidProjectile->scale(glm::vec3(0.5, 0.5, 0.5));
  fluidProjectile->translate(position); 
  getBoundingBox()->setPosition(position); 

  if(hasCollided) {
    timer += dTime;
    if(timer > 0.5) {
      Director::getScene()->removeGameObject(this);
      CollisionManager::removeCollisionObject(this);
      RenderEngine::removeObject(fluidProjectile);
    }
  }
}

void FluidProjectile::collided(CollisionObject * collidedWith){
  if(collidedWith->getCollisionID() == 1 && !hasCollided) {
    hasCollided = true;
    Uniform3DGrid<int> *grid = ((LevelTemplate *)Director::getScene())->getTypeGrid();
    glm::vec3 newPos(grid->getRoundX(oldPosition.x), grid->getRoundY(oldPosition.y), grid->getRoundZ(oldPosition.z));
    if(grid->inGrid(newPos.x, newPos.y, newPos.z) && grid->getValue(newPos.x, newPos.y, newPos.z) == LevelTemplate::AVAILABLE_FILL_SPACE) {
      if(!LoadManager::getRenderTexture("waterData")->isInUse()) {
        WaterSurface *surface = new WaterSurface(newPos, color);
        surface->setup();
        Director::getScene()->addGameObject(surface);
      }
    }
  } 
}
