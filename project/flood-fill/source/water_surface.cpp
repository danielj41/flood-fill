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
  
  level = (LevelTemplate *)Director::getScene();
  typeGrid = level->getTypeGrid();

  // these are intentionally backwards - similar to setting "currentMax" to MIN_INT
  minX = typeGrid->getMaxX();
  maxX = typeGrid->getMinX();
  minY = typeGrid->getMaxY();
  maxY = typeGrid->getMinY();
  minZ = typeGrid->getMaxZ();
  maxZ = typeGrid->getMinZ();

  grid = Uniform3DGrid<int>(*typeGrid);
  grid.initialize(0);

  checkAdjacent(position);

  waterSurface = new Object(
                   LoadManager::getMesh("grid.obj"),
                   MaterialManager::getMaterial("FlatGrey"));

  waterDataTexture = LoadManager::getRenderTexture("waterData");
  waterColorTexture = LoadManager::getRenderTexture("waterColor");
  waterBlockTexture = LoadManager::getRenderTexture("waterBlock");
  waterDataTexture->clear();
  waterColorTexture->clear();
  waterBlockTexture->clear();
  waterBlockTexture->renderBlock(&grid, minX, maxX, minY, maxY, minZ, maxZ); 
  waterSurface->applyWaterData(waterDataTexture->getTexture());
  waterSurface->applyWaterColor(waterBlockTexture->getTexture());
  waterSurface->scale(glm::vec3((maxX - minX + 2.0f) / 2.0f, (maxY - minY + 2.0f) / 2.0f, (maxZ - minZ + 2.0f) / 2.0f));
  waterSurface->enableWater();
  position = glm::vec3((maxX + minX) / 2.0f, (maxY + minY) / 2.0f, (maxZ + minZ) / 2.0f);
  waterSurface->translate(position);
  
  RenderEngine::addObject(waterSurface);
}

void WaterSurface::update(){
  waterSurface->applyWaterData(waterDataTexture->getTexture());
  //waterSurface->applyWaterColor(waterColorTexture->getTexture());
  waterDataTexture->render(LoadManager::getShader("render-texture-vertex.glsl", "render-texture-fragment.glsl"));
  waterColorTexture->render(LoadManager::getShader("render-texture-vertex.glsl", "render-texture-fragment.glsl"));
  waterDataTexture->swapTextures();
  waterColorTexture->swapTextures();
}

void WaterSurface::checkAdjacent(glm::vec3 newPos) {
  if(grid.inGrid(newPos.x, newPos.y, newPos.z) &&
     grid.getValue(newPos.x, newPos.y, newPos.z) == 0 &&
     typeGrid->getValue(newPos.x, newPos.y, newPos.z) == LevelTemplate::AVAILABLE_FILL_SPACE) {
    grid.setValue(newPos.x, newPos.y, newPos.z, 1);

    if(newPos.x > maxX) {
      maxX = newPos.x;
    }
    if(newPos.x < minX) {
      minX = newPos.x;
    }
    if(newPos.y > maxY) {
      maxY = newPos.y;
    }
    if(newPos.y < minY) {
      minY = newPos.y;
    }
    if(newPos.z > maxZ) {
      maxZ = newPos.z;
    }
    if(newPos.z < minZ) {
      minZ = newPos.z;
    }

    checkAdjacent(newPos + glm::vec3(grid.getEdgeSizeX(), 0, 0));
    checkAdjacent(newPos - glm::vec3(grid.getEdgeSizeX(), 0, 0));
    checkAdjacent(newPos - glm::vec3(0, grid.getEdgeSizeY(), 0));
    checkAdjacent(newPos + glm::vec3(0, 0, grid.getEdgeSizeZ()));
    checkAdjacent(newPos - glm::vec3(0, 0, grid.getEdgeSizeZ()));
  }
}

void WaterSurface::collided(CollisionObject * collidedWith){
  
  switch (collidedWith->getCollisionID()){
  default:
    break;
  } 
}
