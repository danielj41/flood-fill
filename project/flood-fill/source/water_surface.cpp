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
#include "fluid_box.hpp"

WaterSurface::WaterSurface(glm::vec3 _position)
  : GameObject(), CollisionObject(_position), position(_position) {}
//
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

  timer = 0.0f;
  if(!checkAdjacent(position)) {
    timer = 2.0f; // remove next frame
  };

  startPosition = position;
  waterSurface = new Object(
                   LoadManager::getMesh("grid.obj"),
                   MaterialManager::getMaterial("FlatBlue"));

  waterDataTexture = LoadManager::getRenderTexture("waterData");
  waterColorTexture = LoadManager::getRenderTexture("waterColor");
  waterBlockTexture = LoadManager::getRenderTexture("waterBlock");

  waterDataTexture->clear();
  waterColorTexture->clear();
  waterBlockTexture->clear();

  waterBlockTexture->renderBlock(&grid, minX, maxX, minY, maxY, minZ, maxZ, 1.05f); 
  waterSurface->applyWaterBlock(waterBlockTexture->getTexture());

  waterColorTexture->render(LoadManager::getShader("render-texture-vertex-color-initial.glsl", "render-texture-fragment-color-initial.glsl"),
                            waterBlockTexture->getTexture(), glm::vec2(0), glm::vec3(0), glm::vec3(0));
  waterSurface->applyWaterColor(waterColorTexture->getTexture());
  waterColorTexture->swapTextures();

  size = glm::vec3((maxX - minX + 2.0f) / 2.0f, (maxY - minY + 2.0f) / 2.0f, (maxZ - minZ + 2.0f) / 2.0f);

  waterDataTexture->render(LoadManager::getShader("render-texture-vertex-data-initial.glsl", "render-texture-fragment-data-initial.glsl"),
                            waterBlockTexture->getTexture(), glm::vec2(0), glm::vec3(0), size);
  waterSurface->applyWaterData(waterDataTexture->getTexture());
  waterDataTexture->swapTextures();

  waterSurface->scale(size);
  waterSurface->enableWater();
  waterSurface->setDTime(glm::vec2(0));
  position = glm::vec3((maxX + minX) / 2.0f, (maxY + minY) / 2.0f, (maxZ + minZ) / 2.0f);
  waterSurface->translate(position);
  
  RenderEngine::getRenderElement("water")->addObject(waterSurface);

  grid.initialize(0);
  createFluidBox(startPosition);
}

void WaterSurface::update(){
  float dTime = ((float) TimeManager::getDeltaTime());

  waterSurface->applyWaterData(waterDataTexture->getTexture());
  waterDataTexture->render(LoadManager::getShader("render-texture-vertex-data-update.glsl", "render-texture-fragment-data-update.glsl"),
                           waterBlockTexture->getTexture(), glm::vec2(dTime, timer), startPosition - position, size);
  waterDataTexture->swapTextures();

  waterSurface->applyWaterColor(waterColorTexture->getTexture());
  waterColorTexture->render(LoadManager::getShader("render-texture-vertex-color-update.glsl", "render-texture-fragment-color-update.glsl"),
                           waterBlockTexture->getTexture(), glm::vec2(dTime, timer), startPosition - position, size);
  waterColorTexture->swapTextures();

  waterSurface->setDTime(glm::vec2(dTime, timer));

  timer += dTime;
  if(timer > 1.55f) {
    Director::getScene()->removeGameObject(this);
    RenderEngine::getRenderElement("water")->removeObject(waterSurface);
    waterDataTexture->release();
    waterColorTexture->release();
    waterBlockTexture->release();
  }
}

void WaterSurface::createFluidBox(glm::vec3 newPos) {
  if(grid.inGrid(newPos.x, newPos.y, newPos.z) &&
     grid.getValue(newPos.x, newPos.y, newPos.z) == 0 &&
     typeGrid->getValue(newPos.x, newPos.y, newPos.z) == LevelTemplate::AVAILABLE_FILL_SPACE) {

    grid.setValue(newPos.x, newPos.y, newPos.z, 1);

    if(newPos.y < lowestPosition.y + grid.getEdgeSizeY() / 2.0f) {
      typeGrid->setValue(newPos.x, newPos.y, newPos.z, LevelTemplate::SOLID_CUBE);
      FluidBox *fluidBox = new FluidBox(newPos);
      fluidBox->setup();
      Director::getScene()->addGameObject(fluidBox);
    }
    
    createFluidBox(newPos + glm::vec3(grid.getEdgeSizeX(), 0, 0));
    createFluidBox(newPos - glm::vec3(grid.getEdgeSizeX(), 0, 0));
    createFluidBox(newPos - glm::vec3(0, grid.getEdgeSizeY(), 0));
    createFluidBox(newPos + glm::vec3(0, 0, grid.getEdgeSizeZ()));
    createFluidBox(newPos - glm::vec3(0, 0, grid.getEdgeSizeZ()));
  }
}

bool WaterSurface::checkAdjacent(glm::vec3 newPos) {
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
      lowestPosition = newPos;
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

    return true;
  }
  return false;
}

void WaterSurface::collided(CollisionObject * collidedWith){
  
  switch (collidedWith->getCollisionID()){
  default:
    break;
  } 
}
