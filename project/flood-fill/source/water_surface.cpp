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
#include "level_template.hpp"


#define BLUE    1
#define GREEN   2
#define RED     4
#define GREY    8



WaterSurface::WaterSurface(glm::vec3 _position, int _colorMask)
  : GameObject(), CollisionObject(_position), position(_position),
    colorMask(_colorMask), speed(1.0f) {}

void WaterSurface::setup() {
  if(colorMask & BLUE)
    color = "FlatBlue";
  else if(colorMask & GREEN)
    color = "FlatGreen";
  else if(colorMask & RED)
    color = "FlatRed";
  else if(colorMask & GREY)
    color = "FlatGrey";

  LevelTemplatePtr level = PTR_CAST(LevelTemplate, Director::getScene());
  typeGrid = level->getTypeGrid();

  // these are intentionally backwards - similar to setting "currentMax" to MIN_INT
  minX = typeGrid->getMaxX();
  maxX = typeGrid->getMinX();
  minY = typeGrid->getMaxY();
  maxY = typeGrid->getMinY();
  minZ = typeGrid->getMaxZ();
  maxZ = typeGrid->getMinZ();

  visitedGrid = Uniform3DGridPtr<int>(new Uniform3DGrid<int>(
    typeGrid->getSizeX(), typeGrid->getSizeY(), typeGrid->getSizeZ(),
    typeGrid->getMinX(), typeGrid->getMaxX(),
    typeGrid->getMinY(), typeGrid->getMaxY(),
    typeGrid->getMinZ(), typeGrid->getMaxZ()));
  targetGrid = Uniform3DGridPtr<int>(new Uniform3DGrid<int>(
    typeGrid->getSizeX(), typeGrid->getSizeY(), typeGrid->getSizeZ(),
    typeGrid->getMinX(), typeGrid->getMaxX(),
    typeGrid->getMinY(), typeGrid->getMaxY(),
    typeGrid->getMinZ(), typeGrid->getMaxZ()));
  lowestGrid = Uniform3DGridPtr<float>(new Uniform3DGrid<float>(
    typeGrid->getSizeX(), typeGrid->getSizeY(), typeGrid->getSizeZ(),
    typeGrid->getMinX(), typeGrid->getMaxX(),
    typeGrid->getMinY(), typeGrid->getMaxY(),
    typeGrid->getMinZ(), typeGrid->getMaxZ()));
  visitedGrid->initialize(0);
  targetGrid->initialize(1);
  lowestGrid->initialize(typeGrid->getMaxY());

  floodFillVisit(position);
  floodFillTarget(position, typeGrid->getMaxY());

  timer = 0.0f;

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

  waterBlockTexture->renderBlock(visitedGrid, minX, maxX, minY, maxY, minZ, maxZ, true, glm::vec3(1.0f, 0.0f, 0.0f)); 
  waterBlockTexture->swapTextures();
  waterBlockTexture->renderBlock(targetGrid, minX, maxX, minY, maxY, minZ, maxZ, false, glm::vec3(0.0f, 1.0f, 0.0f)); 
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
  
  waterSurface->setGridScale(glm::vec2(size.x, size.z));
  waterSurface->applyTexture(LoadManager::getTexture("PureWhiteTexture"));
  waterSurface->enableTexture();
  waterSurface->applyNormalMap(LoadManager::getTexture("RegularNormalMap"));
  RenderEngine::getRenderElement("water")->addObject(waterSurface);
}

void WaterSurface::update() {}

bool WaterSurface::manualUpdate(){
  float dTime = ((float) TimeManager::getDeltaTime());

  timer += dTime * speed;

  waterSurface->applyWaterData(waterDataTexture->getTexture());
  waterDataTexture->render(LoadManager::getShader("render-texture-vertex-data-update.glsl", "render-texture-fragment-data-update.glsl"),
                           waterBlockTexture->getTexture(), glm::vec2(dTime, timer), startPosition - position, size);
  waterDataTexture->swapTextures();

  waterSurface->applyWaterColor(waterColorTexture->getTexture());
  waterColorTexture->render(LoadManager::getShader("render-texture-vertex-color-update.glsl", "render-texture-fragment-color-update.glsl"),
                           waterBlockTexture->getTexture(), glm::vec2(dTime, timer), startPosition - position, size);
  waterColorTexture->swapTextures();

  waterSurface->setDTime(glm::vec2(dTime, timer));

  if(timer > 1.55f) {
    end();
    return false;
  }

  return true;
}

void WaterSurface::end() {
  Director::getScene()->removeGameObject(this);
  RenderEngine::getRenderElement("water")->removeObject(waterSurface);
  waterDataTexture->release();
  waterColorTexture->release();
  waterBlockTexture->release();
}

float WaterSurface::getSpeed() {
  return speed;
}

void WaterSurface::setSpeed(float speed) {
  this->speed = speed;
  for(std::list<FluidBoxPtr>::iterator it = fluidBoxes.begin(); it != fluidBoxes.end(); it++){
    (*it)->setSpeed(speed);
  }
}

float WaterSurface::floodFillVisit(glm::vec3 newPos) {
  if(visitedGrid->inGrid(newPos.x, newPos.y, newPos.z) &&
     visitedGrid->getValue(newPos.x, newPos.y, newPos.z) == 0 &&
     (typeGrid->getValue(newPos.x, newPos.y, newPos.z) == LevelTemplate::AVAILABLE_FILL_SPACE ||
      typeGrid->getValue(newPos.x, newPos.y, newPos.z) == LevelTemplate::FLUID_DRAIN)) { 
    
    visitedGrid->setValue(newPos.x, newPos.y, newPos.z, 1);
    
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

    float lowestY = newPos.y;
    lowestY = fmin(lowestY, floodFillVisit(newPos + glm::vec3(typeGrid->getEdgeSizeX(), 0, 0)));
    lowestY = fmin(lowestY, floodFillVisit(newPos - glm::vec3(typeGrid->getEdgeSizeX(), 0, 0)));
    lowestY = fmin(lowestY, floodFillVisit(newPos + glm::vec3(0, 0, typeGrid->getEdgeSizeZ())));
    lowestY = fmin(lowestY, floodFillVisit(newPos - glm::vec3(0, 0, typeGrid->getEdgeSizeZ())));
    lowestY = fmin(lowestY, floodFillVisit(newPos - glm::vec3(0, typeGrid->getEdgeSizeY(), 0)));
    lowestGrid->setValue(newPos.x, newPos.y, newPos.z, lowestY);
    // store height information for second pass on finding the lowest pockets.
    return lowestY;
  }

  if(lowestGrid->inGrid(newPos.x, newPos.y, newPos.z)) {
    return lowestGrid->getValue(newPos.x, newPos.y, newPos.z);
  }
  return visitedGrid->getMaxY();
}

float WaterSurface::floodFillTarget(glm::vec3 newPos, float lowestY) {
  if(visitedGrid->inGrid(newPos.x, newPos.y, newPos.z) &&
     visitedGrid->getValue(newPos.x, newPos.y, newPos.z) == 1) { 
    
    // Increment so we don't visit again
    visitedGrid->setValue(newPos.x, newPos.y, newPos.z, 2);
    
    lowestY = fmin(lowestY, lowestGrid->getValue(newPos.x, newPos.y, newPos.z));
    
    if(newPos.y < lowestY + lowestGrid->getEdgeSizeY() * 0.5f) {
      targetGrid->setValue(newPos.x, newPos.y, newPos.z, 0);
      FluidBoxPtr fluidBox(new FluidBox(newPos, colorMask));
      fluidBox->setup();
      fluidBoxes.push_back(fluidBox);
      PTR_CAST(LevelTemplate, Director::getScene())->setGridValue(fluidBox->getPosition(), fluidBox);
      Director::getScene()->addGameObject(fluidBox);
      CollisionManager::addCollisionObjectToGrid(fluidBox);

      if(typeGrid->getValue(newPos.x, newPos.y, newPos.z) != LevelTemplate::FLUID_DRAIN) {
        if(colorMask & BLUE)
            typeGrid->setValue(newPos.x, newPos.y, newPos.z, LevelTemplate::FLUID_BLUE);
        else if(colorMask & GREEN)
            typeGrid->setValue(newPos.x, newPos.y, newPos.z, LevelTemplate::FLUID_GREEN);
        else if(colorMask & RED)
            typeGrid->setValue(newPos.x, newPos.y, newPos.z, LevelTemplate::FLUID_RED);
        else
            ASSERT(false, "There is no fluid type avaible");
      } else {
        fluidBox->removeAtAdd();
      }
    }

    floodFillTarget(newPos + glm::vec3(typeGrid->getEdgeSizeX(), 0, 0), lowestY);
    floodFillTarget(newPos - glm::vec3(typeGrid->getEdgeSizeX(), 0, 0), lowestY);
    floodFillTarget(newPos + glm::vec3(0, 0, typeGrid->getEdgeSizeZ()), lowestY);
    floodFillTarget(newPos - glm::vec3(0, 0, typeGrid->getEdgeSizeZ()), lowestY);
    // don't pass height information downward, so it will fill different height pockets that are separated.
    floodFillTarget(newPos - glm::vec3(0, typeGrid->getEdgeSizeY(), 0), lowestGrid->getMaxY());

    return lowestY;
  }

  return lowestGrid->getMaxY();
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

    // TODO - move this to engine with render_texture
    LoadManager::loadShader("render-texture-vertex-block.glsl", "render-texture-fragment-block.glsl");
    shader = LoadManager::getShader("render-texture-vertex-block.glsl", "render-texture-fragment-block.glsl");
    shader->loadHandle("aPosition", 'a');
    shader->loadHandle("uModelMatrix", 'u');
    shader->loadHandle("uPrevTexture", 'u');
    shader->loadHandle("uColor", 'u');
}
