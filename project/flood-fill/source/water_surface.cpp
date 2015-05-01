#include "water_surface.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#include "load_manager.hpp"
#include "time_manager.hpp"
#include "collision_manager.hpp"
#include "director.hpp"
#include "material_manager.hpp"
#include "render_engine.hpp"

WaterSurface::WaterSurface(glm::vec3 _position)
  : GameObject(), CollisionObject(_position), position(_position) {}

void WaterSurface::setup() {  
  
  waterSurface = new Object(
                   LoadManager::getMesh("grid.obj"),
                   MaterialManager::getMaterial("FlatGrey"));

  waterSurface->translate(position);
  waterSurface->enableWater();
  
  RenderEngine::addObject(waterSurface);
}

void WaterSurface::update(){

}

void WaterSurface::collided(CollisionObject * collidedWith){
  
  switch (collidedWith->getCollisionID()){
  default:
    break;
  } 
}
