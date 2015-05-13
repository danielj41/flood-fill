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
#define GREEN   2
#define RED     4
#define GREY    8

FluidProjectile::FluidProjectile(glm::vec3 _position,
  glm::vec3 _movementDirection, int _colorMask)
  : GameObject(), CollisionObject(_position),
	position(_position), movementDirection(_movementDirection),
	size(glm::vec3(0.1)) {
    colorMask = _colorMask;
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
  
  fluidProjectile = new Object(LoadManager::getMesh("stream.obj"),
			                     	   MaterialManager::getMaterial(color));

  fluidParticles = new Object(LoadManager::getMesh("particles.obj"),
                              MaterialManager::getMaterial(color));

  RenderEngine::getRenderElement("water-stream")->addObject(fluidProjectile);
  RenderEngine::getRenderElement("water-particle")->addObject(fluidParticles);
  
  setCollisionID(0);
  setCollideWithID(1 + 64);
  setCanCollide(true);

  origPosition = position;

  angle = 90.0 - glm::degrees(atan2(movementDirection.z, movementDirection.x));

  setPosition(position);
  
  fluidProjectile->setVelocity(movementDirection);
  fluidProjectile->loadIdentity();
  fluidProjectile->rotate(angle, glm::vec3(0.0, 1.0, 0.0));
  fluidProjectile->translate(position - glm::vec3(0.0f, 0.75f, 0.0f)); 

  fluidParticles->setVelocity(movementDirection);
  fluidParticles->loadIdentity();
  fluidParticles->rotate(angle, glm::vec3(0.0, 1.0, 0.0));
  fluidParticles->translate(position - glm::vec3(0.0f, 0.75f, 0.0f)); 

  setBoundingBox(BoundingBox(glm::vec3(0.5f,0.5f,0.5f), glm::vec3(-0.5,-0.5f,-0.5f)));
  getBoundingBox()->setPosition(position);

  hasCollided = false;
  createdSurface = false;
  timer = 0;
  totalTime = 0;
}

void FluidProjectile::update(){
  float dTime = ((float) TimeManager::getDeltaTime());
  totalTime += dTime;

  position += movementDirection * dTime * 15.0f;
  oldPosition = position - 0.5f * movementDirection;
  setPosition(position);
  movementDirection.y -= 1.5f * dTime;

  fluidProjectile->setDTime(glm::vec2(dTime, totalTime));
  fluidParticles->setDTime(glm::vec2(dTime, totalTime));

  getBoundingBox()->setPosition(position); 

  if(hasCollided) {
    timer += dTime;
    if(timer > 0.5) {
      Director::getScene()->removeGameObject(this);
      CollisionManager::removeCollisionObject(this);
      RenderEngine::getRenderElement("water-stream")->removeObject(fluidProjectile);
      RenderEngine::getRenderElement("water-particle")->removeObject(fluidParticles);
    }
  }
}

void FluidProjectile::createWaterSurfaceAt(Uniform3DGrid<int> *grid, std::set<int>* fillTypes, glm::vec3 newPos) {
  if(createdSurface) {
    return;
  }
  if(grid->inGrid(newPos.x, newPos.y, newPos.z) &&
    fillTypes->find(grid->getValue(newPos.x, newPos.y, newPos.z)) != fillTypes->end()) {
    if(!LoadManager::getRenderTexture("waterData")->isInUse()) {
      createdSurface = true;
      WaterSurface *surface = new WaterSurface(newPos, colorMask);
      surface->setup();
      Director::getScene()->addGameObject(surface);
    }
  }
}

void FluidProjectile::collided(CollisionObject * collidedWith){
  if((collidedWith->getCollisionID() == 1 || collidedWith->getCollisionID() == 64) && !hasCollided) {
    hasCollided = true;
    std::set<int>* fillTypes = ((LevelTemplate *)Director::getScene())->getFillTypes();
    Uniform3DGrid<int> *grid = ((LevelTemplate *)Director::getScene())->getTypeGrid();
    glm::vec3 newPos(grid->getRoundX(oldPosition.x), grid->getRoundY(oldPosition.y), grid->getRoundZ(oldPosition.z));
     createWaterSurfaceAt(grid, fillTypes, newPos + glm::vec3(0.0f, grid->getEdgeSizeY(), 0.0f));
    createWaterSurfaceAt(grid, fillTypes, newPos);
    // if you can't create a water surface there, look at a few adjacent cells, but not too far.
    createWaterSurfaceAt(grid, fillTypes, newPos + glm::vec3(0.0f, -grid->getEdgeSizeY(), 0.0f));
    createWaterSurfaceAt(grid, fillTypes, newPos + glm::vec3(grid->getEdgeSizeX(), -grid->getEdgeSizeY(), 0.0f));
    createWaterSurfaceAt(grid, fillTypes, newPos + glm::vec3(-grid->getEdgeSizeX(), -grid->getEdgeSizeY(), 0.0f));
    createWaterSurfaceAt(grid, fillTypes, newPos + glm::vec3(0.0f, -grid->getEdgeSizeY(), grid->getEdgeSizeZ()));
    createWaterSurfaceAt(grid, fillTypes, newPos + glm::vec3(0.0f, -grid->getEdgeSizeY(), -grid->getEdgeSizeZ()));
    createWaterSurfaceAt(grid, fillTypes, newPos + glm::vec3(grid->getEdgeSizeX(), -grid->getEdgeSizeY(), grid->getEdgeSizeZ()));
    createWaterSurfaceAt(grid, fillTypes, newPos + glm::vec3(-grid->getEdgeSizeX(), -grid->getEdgeSizeY(), grid->getEdgeSizeZ()));
    createWaterSurfaceAt(grid, fillTypes, newPos + glm::vec3(grid->getEdgeSizeX(), -grid->getEdgeSizeY(), -grid->getEdgeSizeZ()));
    createWaterSurfaceAt(grid, fillTypes, newPos + glm::vec3(-grid->getEdgeSizeX(), -grid->getEdgeSizeY(), -grid->getEdgeSizeZ()));
  } 
}
