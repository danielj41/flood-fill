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
#include "invisible_wall.hpp"
#include "collision_manager.hpp"
#include "time_manager.hpp"
#include "light.hpp"
#include "uniform_3d_grid.hpp"
#include "box.hpp"
#include "level_design.hpp"

DemoLevel::DemoLevel() : Scene("DemoLevel"){}

int numboxes = 0;

void DemoLevel::setup(){
    INFO("Setting up the Demo Level...");

	Uniform3DGrid<int> map = Uniform3DGrid<int>(6, 6, 16, -1, 1, -1, 1, -1, 1);

    Camera * cam1 = new Camera(glm::vec3(0, 2, 0), glm::vec3(0, 0,-3),
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
	

	Box * box;

	// Init map
	for (int i = 0; i < LevelDesign::getLenX(); i++) {
	  for (int j = 0; j < LevelDesign::getLenY(); j++) {
		for (int k = 0; k < LevelDesign::getLenZ(); k++) { 
		  if ( LevelDesign::getVal(i, j, k) == 1 ) {
			box = new Box(glm::vec3(2*i+1,2*j+1,-2*j-1), 
								glm::vec3(0,0,0), 0.0f);
			box->setup();
			addGameObject("box", box);
		  }
		}
	  }
	}

    player = new Player(cam1);
    player->setup();
    addGameObject("player" , player);
    CollisionManager::addCollisionObject(player);

    debugPlayer = new DebugPlayer(cam2);
    debugPlayer->setup();
    addGameObject("debugPlayer" , debugPlayer);

    Light * l1 = new Light(glm::vec3(1), 5.0f, glm::vec3(0, -1, 0));
    addLight("Sun", l1);
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
