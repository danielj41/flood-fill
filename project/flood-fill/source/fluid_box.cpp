#include "fluid_box.hpp"

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
#include "level_template.hpp"
#include "global_variables.hpp"
#include "regular_polygons_render.hpp"

#define BLUE    1
#define GREEN   2
#define RED     4
#define GREY    8

FluidBox::FluidBox(glm::vec3 _position)
  : GameObject(), CollisionObject(_position),
  position(_position), colorMask(1), size(glm::vec3(1)) {}

FluidBox::FluidBox(glm::vec3 _position, int _colorMask)
  : GameObject(), CollisionObject(_position),
    position(_position), colorMask(_colorMask), size(glm::vec3(1)) {}

void FluidBox::setup() {
  INFO("Creating a box ...");

  if(colorMask & BLUE)
    color = "FlatBlue";
  else if(colorMask & GREEN)
    color = "FlatGreen";
  else if(colorMask & RED)
    color = "FlatRed";
  else if(colorMask & GREY)
    color = "FlatGrey";

  fluidBox = ObjectPtr(new Object(
                        LoadManager::getMesh("cube.obj"),
                        MaterialManager::getMaterial(color)));

  //fluidBox->applyTexture(LoadManager::getTexture("VoxelTexture"));
  //fluidBox->enableTexture();

  fluidBox->loadIdentity();
  fluidBox->translate(position);
  timer = 0.0f;
  visible = false;
  deleting = false;

  setCollisionID(64);
  setCanCollide(true);

  setBoundingBox(BoundingBox(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(-1.0f,-1.0f,-1.0f)));
  getBoundingBox()->setPosition(position - glm::vec3(0, 2.0f, 0));
}

void FluidBox::update(){
    float dTime = ((float) TimeManager::getDeltaTime());

    if(!visible) {
        timer += dTime;
        getBoundingBox()->setPosition(position - (1.0f - timer/1.55f) * glm::vec3(0, 2.0f, 0));
        if(timer > 1.55f) {
            visible = true;
            RenderEngine::getRenderGrid()->addObject(fluidBox, RenderEngine::getRenderElement("regular"));
            getBoundingBox()->setPosition(position);
        }
    }

    if(deleting) {
        remotionAnimation();
    }
}

void FluidBox::remove(){
    INFO("Removing Fluid Box...");

    if(deleting) return;

    deleting = true;
    timer = 0.0f;
    RenderEngine::getRenderGrid()->removeObject(fluidBox);
    RenderEngine::getRenderElement("regular")->addObject(fluidBox);
}

void FluidBox::remotionAnimation(){
    float dTime = ((float) TimeManager::getDeltaTime());

    timer += dTime;
    fluidBox->loadIdentity();
    fluidBox->scale(glm::vec3(1.0 - timer, 1.0 - timer, 1.0 - timer));
    fluidBox->translate(position - glm::vec3(0, timer * 4.0f, 0));
    getBoundingBox()->setPosition(position - glm::vec3(0, timer * 4.0f, 0));
    if(timer > 0.5f) {
        // Moving to the original position, to remove it from the uniform grid
        fluidBox->loadIdentity();
        fluidBox->translate(position);

        // Removing it from the grid
        Director::getScene()->removeGameObject(this);
        PTR_CAST(LevelTemplate, Director::getScene())->setTypeCell(position, LevelTemplate::AVAILABLE_FILL_SPACE);
        RenderEngine::getRenderElement("regular")->removeObject(fluidBox);
        CollisionManager::removeCollisionObjectFromGrid(this);
    }
}

void FluidBox::collided(CollisionObjectPtr){
}

int FluidBox::getColorMask(){
    return colorMask;
}

void FluidBox::highlightForRemotion(){
}

void FluidBox::deselect(){
}
