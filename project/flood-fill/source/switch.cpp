#include "switch.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#include "load_manager.hpp"
#include "time_manager.hpp"
#include "collision_manager.hpp"
#include "director.hpp"
#include "material_manager.hpp"
#include "render_engine.hpp"
#include "player.hpp"

Switch::Switch(glm::vec3 _scale,glm::vec3 _position, glm::vec3 _rotation, float _offsetAngle, int _direction) 
  : GameObject(), CollisionObject(_position),
	scale(_scale),
    position(_position),  
    rotation(_rotation), 
    offsetAngle(_offsetAngle),
    direction(_direction),   
    triggered(false), on(false),
    theta(-40.0f), 
    amplitude(20.0f), speed(50.0)
    {}

void Switch::setup() {  
  INFO("Creating a switch ...");
  
  handle = new Object(LoadManager::getMesh("cube.obj"), MaterialManager::getMaterial("Red"));
  shaft = new Object(LoadManager::getMesh("cube.obj"), MaterialManager::getMaterial("FlatGrey"));
  
  shaft->loadIdentity();
  shaft->scale(scale);
  shaft->translate(glm::vec3(-0.1f, 0.0f, 0.0f));
  shaft->rotate(theta, rotation);
  shaft->translate(position);
          
  handle->loadIdentity();
  handle->scale(glm::vec3(0.17f, 0.17f, 0.17f));
  handle->translate(glm::vec3(-1.1f, 0.0f, 0.0f));
  handle->rotate(theta, rotation);
  handle->translate(position);

  RenderEngine::getRenderElement("regular")->addObject(handle);
  RenderEngine::getRenderElement("regular")->addObject(shaft);
  
  setCollisionID(32);
  setCanCollide(true);

  setBoundingBox(BoundingBox(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(-1.0f,-1.0f,-1.0f)));
  getBoundingBox()->setPosition(position);
}

void Switch::update(){

  if (triggered) {
      if (theta < -amplitude + offsetAngle || theta > offsetAngle + amplitude) {
          theta = amplitude * direction + offsetAngle;
          triggered = !triggered;
          direction = -direction;
      } else {
          theta += speed * direction * TimeManager::getDeltaTime() ;
      }
  }    
  
  shaft->loadIdentity();
  shaft->scale(glm::vec3(0.7f, 0.1f, 0.1f));
  shaft->translate(glm::vec3(-0.1f, 0.0f, 0.0f));
  shaft->rotate(theta, rotation);
  shaft->translate(position);
          
  handle->loadIdentity();
  handle->scale(glm::vec3(0.17f, 0.17f, 0.17f));
  handle->translate(glm::vec3(-0.9f, 0.0f, 0.0f));
  handle->rotate(theta, rotation);
  handle->translate(position);
}

void Switch::collided(CollisionObject * collidedWith){

    switch (collidedWith->getCollisionID()){
    case 2: //Player
        INFO("COLLISION DETECTED WITH SWITCH");
        if (((Player*)collidedWith)->isKeyPressed(GLFW_KEY_E) && !triggered){
            triggered = true;
            on = !on;
        }
        break;
    default:
        break;
    }
}

bool Switch::isOn(){
    return on;
}
