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
#define GREEN   2
#define RED     4
#define GREY    8

ColorChange::ColorChange(glm::vec3 _position, int _colorMask)
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
  
  colorChange = ObjectPtr(new Object(
                   LoadManager::getMesh("cube.obj"),
                   MaterialManager::getMaterial(color)));

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
  colorChange->scale(glm::vec3(.25, .25, .25) * (float)((cos(timer * 5.0) + 3.0) / 2.0));
  colorChange->rotate(45.0 * timer, glm::vec3(0, 1.0, 0));
  colorChange->translate(position);

}

int ColorChange::getColor() {
  return colorMask;
}

void ColorChange::collided(CollisionObjectPtr collidedWith){
  
  switch (collidedWith->getCollisionID()){
  case 2:
    break;
  } 
}