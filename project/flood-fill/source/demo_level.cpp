#include "demo_level.hpp"

#include <sstream>

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "global_variables.hpp"
#include "ground_plane.hpp"
#include "obstacle.hpp"
#include "debug_player.hpp"
#include "collision_object.hpp"
#include "collision_manager.hpp"
#include "time_manager.hpp"
#include "light.hpp"
#include "uniform_3d_grid.hpp"
#include "box.hpp"
#include "level_design.hpp"
#include "border.hpp"

DemoLevel::DemoLevel() : Scene("DemoLevel"){}

void DemoLevel::setup(){
    INFO("Setting up the Demo Level...");
	
    Camera * cam1 = new Camera(glm::vec3(6, 8, -4), glm::vec3(6, 0, -10),
                             glm::vec3(0, 1, 0));
    cam1->setProjectionMatrix(
        glm::perspective(glm::radians(90.0f),
                        (float) Global::ScreenWidth/Global::ScreenHeight,
                        0.1f, 100.f));

    addCamera("Camera1", cam1);
    setMainCamera("Camera1");

    Camera * cam2 = new Camera(glm::vec3(0, 1, 0), glm::vec3(0, 0, -5),
							   glm::vec3(0, 1, 0));
    cam2->setProjectionMatrix(
        glm::perspective(glm::radians(90.0f),
                        (float) Global::ScreenWidth/Global::ScreenHeight,
                        0.1f, 100.f));
    addCamera("DebugCamera", cam2);

    player = new Player(cam1);
    player->setup();
    addGameObject("player" , player);
    CollisionManager::addCollisionObjectToList(player);

    debugPlayer = new DebugPlayer(cam2);
    debugPlayer->setup();
    addGameObject("debugPlayer" , debugPlayer);

    LightPtr l1 = new Light(glm::vec3(1), 5.0f, glm::vec3(0, -1, 0));
    addLight("Sun", l1);

	// Init Grid and Map
	CollisionManager::initGrid(6, 6, 16, glm::vec3(0,0,-32), glm::vec3(12,12,0));
	Border::createBorder();
	
	for (int i = 0; i < LevelDesign::getLenX(); i++) {
	  for (int j = 0; j < LevelDesign::getLenY(); j++) {
		for (int k = 0; k < LevelDesign::getLenZ(); k++) { 
		  if ( LevelDesign::getVal(i, j, k) == 1 ) {
			
			glm::vec3 pos = glm::vec3(2*i+3,2*j+1,-2*k-3);
			BoxPtr box = new Box( pos, 
								glm::vec3(0,0,0), 0.0f);
			box->setup();
			CollisionManager::addCollisionObjectToGrid(box);
			addGameObject("box", box);
		  }
		}
	  }
	}
}

void DemoLevel::update(){

    if(debugPlayer->isActive()){
        ASSERT(getCamera("Camera1") != getCamera("DebugCamera"), "Equal camera");
        setMainCamera("DebugCamera");
        getCamera("Camera1")->fix();
    }
    else{
        setMainCamera("Camera1");
        getCamera("Camera1")->fix(false, true, false);
    }

}
