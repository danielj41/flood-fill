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

  texture = new RenderTexture();
  texture->load();
  waterSurface->applyWaterData(texture->getTexture());
  waterSurface->applyWaterColor(texture->getTexture());
  waterSurface->scale(glm::vec3(4.0f, 1.0f, 4.0f));
  waterSurface->enableWater();
  waterSurface->translate(position);
  
  RenderEngine::addObject(waterSurface);
}

void WaterSurface::update(){
  waterSurface->applyWaterData(texture->getTexture());
  waterSurface->applyWaterColor(texture->getTexture());
  texture->render();
}

void WaterSurface::collided(CollisionObject * collidedWith){
  
  switch (collidedWith->getCollisionID()){
  default:
    break;
  } 
}
