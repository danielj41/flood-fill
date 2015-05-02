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

  waterDataTexture = LoadManager::getRenderTexture("waterData");
  waterColorTexture = LoadManager::getRenderTexture("waterColor");
  waterDataTexture->clear();
  waterColorTexture->clear();
  waterSurface->applyWaterData(waterDataTexture->getTexture());
  waterSurface->applyWaterColor(waterColorTexture->getTexture());
  waterSurface->scale(glm::vec3(4.0f, 1.0f, 4.0f));
  waterSurface->enableWater();
  waterSurface->translate(position);
  
  RenderEngine::addObject(waterSurface);
}

void WaterSurface::update(){
  waterSurface->applyWaterData(waterDataTexture->getTexture());
  waterSurface->applyWaterColor(waterColorTexture->getTexture());
  waterDataTexture->render(LoadManager::getShader("render-texture-vertex.glsl", "render-texture-fragment.glsl"));
  waterColorTexture->render(LoadManager::getShader("render-texture-vertex.glsl", "render-texture-fragment.glsl"));
  waterDataTexture->swapTextures();
  waterColorTexture->swapTextures();
}

void WaterSurface::collided(CollisionObject * collidedWith){
  
  switch (collidedWith->getCollisionID()){
  default:
    break;
  } 
}
