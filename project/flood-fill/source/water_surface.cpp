#include "water_surface.hpp"

#include <cstdlib>
#include <iostream>
#include <set>
#include "debug_macros.h"

#include "load_manager.hpp"
#include "time_manager.hpp"
#include "collision_manager.hpp"
#include "director.hpp"
#include "material_manager.hpp"
#include "render_engine.hpp"
#include "fluid_box.hpp"

#define BLUE    1
#define GREEN   2
#define RED     4
#define GREY    8

WaterSurface::WaterSurface(glm::vec3 _position, int _colorMask)
  : GameObject(), CollisionObject(_position), position(_position),
    colorMask(_colorMask){}

void WaterSurface::setup() {
  
  if(colorMask & BLUE)
    color = "FlatBlue";
  else if(colorMask & GREEN)
    color = "FlatGreen";
  else if(colorMask & RED)
    color = "FlatRed";
  else if(colorMask & GREY)
    color = "FlatGrey";

  level = PTR_CAST(LevelTemplate, Director::getScene());
  typeGrid = level->getTypeGrid();

  // these are intentionally backwards - similar to setting "currentMax" to MIN_INT
  minX = typeGrid->getMaxX();
  maxX = typeGrid->getMinX();
  minY = typeGrid->getMaxY();
  maxY = typeGrid->getMinY();
  minZ = typeGrid->getMaxZ();
  maxZ = typeGrid->getMinZ();

  grid = Uniform3DGridPtr<int>(new Uniform3DGrid<int>(*typeGrid));
  targetGrid = Uniform3DGridPtr<int>(new Uniform3DGrid<int>(*typeGrid));
  grid->initialize(0);
  targetGrid->initialize(0);

  hitDrain = false;

  timer = 0.0f;
  checkAdjacent(position, grid->getMaxY());

  if(hitDrain) {
    //minY -= 1.0f * grid->getEdgeSizeY();
    //lowestPosition.y -= 1.0f * grid->getEdgeSizeY();
  }

  startPosition = position;
  waterSurface = ObjectPtr(new Object(
                   LoadManager::getMesh("grid.obj"),
                   MaterialManager::getMaterial(color)));

  waterDataTexture = LoadManager::getRenderTexture("waterData");
  waterColorTexture = LoadManager::getRenderTexture("waterColor");
  waterBlockTexture = LoadManager::getRenderTexture("waterBlock");

  waterDataTexture->clear();
  waterColorTexture->clear();
  waterBlockTexture->clear();

  waterBlockTexture->renderBlock(grid, minX, maxX, minY, maxY, minZ, maxZ, true, glm::vec3(1.0f, 0.0f, 0.0f)); 
  //waterBlockTexture->renderBlock(targetGrid, minX, maxX, minY, maxY, minZ, maxZ, true, glm::vec3(0.0f, 1.0f, 0.0f)); 
  waterSurface->applyWaterBlock(waterBlockTexture->getTexture());

  size = glm::vec3((maxX - minX + 2.0f) / 2.0f, (maxY - minY + 2.0f) / 2.0f, (maxZ - minZ + 2.0f) / 2.0f);

  waterColorTexture->render(LoadManager::getShader("render-texture-vertex-color-initial.glsl", "render-texture-fragment-color-initial.glsl"),
                            waterBlockTexture->getTexture(), glm::vec2(0, 0), startPosition - position, size);
  waterSurface->applyWaterColor(waterColorTexture->getTexture());
  waterColorTexture->swapTextures();

  waterDataTexture->render(LoadManager::getShader("render-texture-vertex-data-initial.glsl", "render-texture-fragment-data-initial.glsl"),
                            waterBlockTexture->getTexture(), glm::vec2(0, 0), startPosition - position, size);
  waterSurface->applyWaterData(waterDataTexture->getTexture());
  waterDataTexture->swapTextures();

  waterSurface->scale(size);
  waterSurface->enableWater();
  waterSurface->setDTime(glm::vec2(0));
  position = glm::vec3((maxX + minX) / 2.0f, (maxY + minY) / 2.0f, (maxZ + minZ) / 2.0f);
  waterSurface->translate(position);
  
  RenderEngine::getRenderElement("water")->addObject(waterSurface);

  grid->initialize(0);
  //createFluidBox(startPosition);
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

/*void WaterSurface::createFluidBox(glm::vec3 newPos) {
  
    std::set<int>* fillTypes = level->getFillTypes();

    if(grid->inGrid(newPos.x, newPos.y, newPos.z) &&
     grid->getValue(newPos.x, newPos.y, newPos.z) == 0 &&
       fillTypes->find(typeGrid->getValue(newPos.x, newPos.y, newPos.z)) != fillTypes->end()) {
        
      grid->setValue(newPos.x, newPos.y, newPos.z, 1);
    
    if(newPos.y < lowestPosition.y + grid->getEdgeSizeY() / 2.0f) {
      if(colorMask & BLUE)
          typeGrid->setValue(newPos.x, newPos.y, newPos.z, LevelTemplate::FLUID_BLUE);
      else if(colorMask & GREEN)
          typeGrid->setValue(newPos.x, newPos.y, newPos.z, LevelTemplate::FLUID_GREEN);
      else if(colorMask & RED)
          typeGrid->setValue(newPos.x, newPos.y, newPos.z, LevelTemplate::FLUID_RED);
      else
          ASSERT(false, "There is no fluid type avaible");
      FluidBoxPtr fluidBox(new FluidBox(newPos, colorMask));
      fluidBox->setup();
      level->setGridValue(fluidBox->getPosition(), fluidBox);
      Director::getScene()->addGameObject(fluidBox);
      CollisionManager::addCollisionObjectToGrid(fluidBox);
    }
    
    createFluidBox(newPos + glm::vec3(grid->getEdgeSizeX(), 0, 0));
    createFluidBox(newPos - glm::vec3(grid->getEdgeSizeX(), 0, 0));
    createFluidBox(newPos - glm::vec3(0, grid->getEdgeSizeY(), 0));
    createFluidBox(newPos + glm::vec3(0, 0, grid->getEdgeSizeZ()));
    createFluidBox(newPos - glm::vec3(0, 0, grid->getEdgeSizeZ()));
  }
}*/

float WaterSurface::checkAdjacent(glm::vec3 newPos, float lowestY) {
  
    std::set<int>* fillTypes = level->getFillTypes();

    if(grid->inGrid(newPos.x, newPos.y, newPos.z) &&
       grid->getValue(newPos.x, newPos.y, newPos.z) == 0) {
        if(fillTypes->find(typeGrid->getValue(newPos.x, newPos.y, newPos.z)) != fillTypes->end()) { 
          
          grid->setValue(newPos.x, newPos.y, newPos.z, 1);
        
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

          lowestY = fmin(newPos.y, lowestY);
          lowestY = fmin(lowestY, checkAdjacent(newPos + glm::vec3(grid->getEdgeSizeX(), 0, 0), lowestY));
          lowestY = fmin(lowestY, checkAdjacent(newPos - glm::vec3(grid->getEdgeSizeX(), 0, 0), lowestY));
          // don't pass height information downward
          lowestY = fmin(lowestY, checkAdjacent(newPos - glm::vec3(0, grid->getEdgeSizeY(), 0), newPos.y));
          lowestY = fmin(lowestY, checkAdjacent(newPos + glm::vec3(0, 0, grid->getEdgeSizeZ()), lowestY));
          lowestY = fmin(lowestY, checkAdjacent(newPos - glm::vec3(0, 0, grid->getEdgeSizeZ()), lowestY));

          if(newPos.y < lowestY + grid->getEdgeSizeY() * 0.5f &&
             typeGrid->getValue(newPos.x, newPos.y, newPos.z) != LevelTemplate::FLUID_DRAIN) {
              targetGrid->setValue(newPos.x, newPos.y, newPos.z, 1);
              if(colorMask & BLUE)
                  typeGrid->setValue(newPos.x, newPos.y, newPos.z, LevelTemplate::FLUID_BLUE);
              else if(colorMask & GREEN)
                  typeGrid->setValue(newPos.x, newPos.y, newPos.z, LevelTemplate::FLUID_GREEN);
              else if(colorMask & RED)
                  typeGrid->setValue(newPos.x, newPos.y, newPos.z, LevelTemplate::FLUID_RED);
              else
                  ASSERT(false, "There is no fluid type avaible");
              FluidBoxPtr fluidBox(new FluidBox(newPos, colorMask));
              fluidBox->setup();
              level->setGridValue(fluidBox->getPosition(), fluidBox);
              Director::getScene()->addGameObject(fluidBox);
              CollisionManager::addCollisionObjectToGrid(fluidBox);
          }

          return lowestY;
      }
  }
  return grid->getMaxY();
}

void WaterSurface::collided(CollisionObjectPtr collidedWith){
  
  switch (collidedWith->getCollisionID()){
  default:
    break;
  } 
}


void WaterSurface::loadShaders() {
    LoadManager::loadShader("render-texture-vertex-data-initial.glsl", "render-texture-fragment-data-initial.glsl");
    ShaderPtr shader = LoadManager::getShader("render-texture-vertex-data-initial.glsl", "render-texture-fragment-data-initial.glsl");
    shader->loadHandle("aPosition", 'a');
    shader->loadHandle("uPrevTexture", 'u');
    shader->loadHandle("uDataTexture", 'u');
    shader->loadHandle("uSize", 'u');
    shader->loadHandle("uDTime", 'u');
    shader->loadHandle("uStartPosition", 'u');

    LoadManager::loadShader("render-texture-vertex-color-initial.glsl", "render-texture-fragment-color-initial.glsl");
    shader = LoadManager::getShader("render-texture-vertex-color-initial.glsl", "render-texture-fragment-color-initial.glsl");
    shader->loadHandle("aPosition", 'a');
    shader->loadHandle("uPrevTexture", 'u');
    shader->loadHandle("uDataTexture", 'u');
    shader->loadHandle("uSize", 'u');
    shader->loadHandle("uDTime", 'u');
    shader->loadHandle("uStartPosition", 'u');

    LoadManager::loadShader("render-texture-vertex-data-update.glsl", "render-texture-fragment-data-update.glsl");
    shader = LoadManager::getShader("render-texture-vertex-data-update.glsl", "render-texture-fragment-data-update.glsl");
    shader->loadHandle("aPosition", 'a');
    shader->loadHandle("uPrevTexture", 'u');
    shader->loadHandle("uDataTexture", 'u');
    shader->loadHandle("uSize", 'u');
    shader->loadHandle("uDTime", 'u');
    shader->loadHandle("uStartPosition", 'u');

    LoadManager::loadShader("render-texture-vertex-color-update.glsl", "render-texture-fragment-color-update.glsl");
    shader = LoadManager::getShader("render-texture-vertex-color-update.glsl", "render-texture-fragment-color-update.glsl");
    shader->loadHandle("aPosition", 'a');
    shader->loadHandle("uPrevTexture", 'u');
    shader->loadHandle("uDataTexture", 'u');
    shader->loadHandle("uSize", 'u');
    shader->loadHandle("uDTime", 'u');
    shader->loadHandle("uStartPosition", 'u');

    LoadManager::loadShader("render-texture-vertex-block.glsl", "render-texture-fragment-block.glsl");
    shader = LoadManager::getShader("render-texture-vertex-block.glsl", "render-texture-fragment-block.glsl");
    shader->loadHandle("aPosition", 'a');
    shader->loadHandle("uModelMatrix", 'u');
    shader->loadHandle("uPrevTexture", 'u');
    shader->loadHandle("uColor", 'u');
}
