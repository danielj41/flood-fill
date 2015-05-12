#include "player_hand.hpp"

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
#include "fluid_box.hpp"

PlayerHand::PlayerHand(glm::vec3 _position)
  : GameObject(), CollisionObject(_position),
	position(_position) {}

void PlayerHand::setup() {    
  setCollisionID(4);
  setCanCollide(true);
  setCollideWithID(64);

  setBoundingBox(BoundingBox(glm::vec3(0.1f,0.1f,0.1f), glm::vec3(-0.1f,-0.1f,-0.1f)));
  getBoundingBox()->setPosition(position);

  CollisionManager::addCollisionObjectToGrid(this);  
}

void PlayerHand::update() {

}

void PlayerHand::setPosition(glm::vec3 _position) {
  position = _position;
  getBoundingBox()->setPosition(position);
}

void PlayerHand::collided(CollisionObject * collidedWith){
  
  switch (collidedWith->getCollisionID()){
  default:
	break;
  } 
}

void PlayerHand::setColorMask(int _colorMask) {
  colorMask = _colorMask;
}

int PlayerHand::getColorMask() {
  return colorMask;
}

