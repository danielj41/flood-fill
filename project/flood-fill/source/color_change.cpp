#include <cstdlib>
#include <iostream>
#include "debug_macros.h"
#include "color_change.hpp"

#include "load_manager.hpp"
#include "time_manager.hpp"
#include "collision_manager.hpp"
#include "director.hpp"
#include "material_manager.hpp"
#include "render_engine.hpp"
#include "level_template.hpp"


#define BLUE    1
#define RED     2
#define GREEN   4
#define GREY    8

ColorChange::ColorChange(glm::vec3 _position, uint _colorMask)
  : GameObject(), CollisionObject(_position),
    position(_position), size(glm::vec3(1)),
    colorMask(_colorMask), timer(0){}

void ColorChange::setup() {  
  INFO("Creating a ColorChange ...");

  if(colorMask & BLUE)
      color = "FlatBlue";
  else if(colorMask & GREEN)
    color = "FlatGreen";
  else if(colorMask & RED)
    color = "FlatRed";
  else if(colorMask & GREY)
    color = "FlatGrey";
  
  colorChange = new Object(
                   LoadManager::getMesh("cube.obj"),
                   MaterialManager::getMaterial(color));

  RenderEngine::getRenderElement("regular")->addObject(colorChange);

  colorChange->scale(glm::vec3(.5, .5, .5));
  colorChange->translate(position);

    
  setCollisionID(16);
  setCollideWithID(2);
  setCanCollide(true);

  setBoundingBox(BoundingBox(glm::vec3(.5f,.5f,.5f), glm::vec3(-.5f,-.5f,-.5f)));
  getBoundingBox()->setPosition(position);
}

void ColorChange::update(){  
  timer += TimeManager::getDeltaTime();
  colorChange->loadIdentity();
  colorChange->scale(glm::vec3(.5, .5, .5));
  colorChange->rotate(45.0 * timer, glm::vec3(0, 1.0, 0));
  colorChange->translate(position);

}

uint ColorChange::getColor() {
  return colorMask;
}

void ColorChange::collided(CollisionObject * collidedWith){
  
  switch (collidedWith->getCollisionID()){
  case 2:
    break;
  } 
}