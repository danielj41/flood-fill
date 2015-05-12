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

PlayerHand::PlayerHand(glm::vec3 _position, Object * _gun)
  : GameObject(), CollisionObject(_position),
	position(_position), gun(_gun),
    numColors(1), clrndx(0) {}

void PlayerHand::setup() {    
  setCollisionID(4);
  setCanCollide(true);
  setCollideWithID(64);

  colors[clrndx] = 1;

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

void PlayerHand::setColorMask(int color) {
   
    if (numColors == 1 && color != colors[0]) {
        numColors++;
        clrndx++;
    } else if ( numColors == 2 && colors[(clrndx+1)%2] == color) {
        clrndx =  (clrndx+1) % 2;
    }

    colors[clrndx] = color;
    
    switch(color) {
    case 1:  
        gun->setMaterial(MaterialManager::getMaterial("FlatBlue"));
        break;
    case 2:
        gun->setMaterial(MaterialManager::getMaterial("FlatGreen"));
        break;
    case 4:
        gun->setMaterial(MaterialManager::getMaterial("FlatRed"));
        break;
     case 8:
         gun->setMaterial(MaterialManager::getMaterial("FlatGrey"));
         break;
    default:
        break;
    }
}

int PlayerHand::getColorMask() {
    return colors[clrndx];
}

void PlayerHand::changeColorMask() {
    if (numColors > 1) {
        clrndx = (clrndx+1) % 2;
        
        switch(colors[clrndx]) {
        case 1:  
            gun->setMaterial(MaterialManager::getMaterial("FlatBlue"));
            break;
        case 2:
            gun->setMaterial(MaterialManager::getMaterial("FlatGreen"));
            break;
        case 4:
            gun->setMaterial(MaterialManager::getMaterial("FlatRed"));
            break;
        case 8:
            gun->setMaterial(MaterialManager::getMaterial("FlatGrey"));
            break;
        default:
            break;
        }
    
        toggleTime = TimeManager::getTimeStamp();
    }     
}

double PlayerHand::getToggleTime() {
    return toggleTime;
}
