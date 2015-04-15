#include "main_level.hpp"

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
#include "player.hpp"
#include "debug_player.hpp"
#include "collision_manager.hpp"

MainLevel::MainLevel() : Scene("MainLevel"){}

void MainLevel::setup(){
    INFO("Setting up the Main Level...");

    Camera * cam1 = new Camera(glm::vec3(0, 1, 0), glm::vec3(0, 0, -5),
                             glm::vec3(0, 1, 0));
    cam1->setProjectionMatrix(
        glm::perspective(90.0f,
                        (float) Global::ScreenWidth/Global::ScreenHeight,
                        0.1f, 100.f));

    addCamera("Camera1", cam1);
    setMainCamera("Camera1");

    Camera * cam2 = new Camera(glm::vec3(0, 1, 0), glm::vec3(0, 0, -5),
                             glm::vec3(0, 1, 0));
    cam2->setProjectionMatrix(
        glm::perspective(90.0f,
                        (float) Global::ScreenWidth/Global::ScreenHeight,
                        0.1f, 100.f));

    addCamera("DebugCamera", cam2);

    GroundPlane * ground = new GroundPlane();
    ground->setup();
    addGameObject("Ground" , ground);

    Obstacle * obs = new Obstacle(glm::vec3(0, 0, 0), 0.05);
    obs->setup();
    addGameObject("obstacle" , obs);
    CollisionManager::addCollisionObject(obs);

    Player * p1 = new Player(cam1);
    p1->setup();
    addGameObject("player" , p1);
    CollisionManager::addCollisionObject(p1);

    debugPlayer = new DebugPlayer(cam2);
    debugPlayer->setup();
    addGameObject("debugPlayer" , debugPlayer);
}

void MainLevel::update(){
    if(debugPlayer->isActive()){
        setMainCamera("DebugCamera");
    }
    else{
        setMainCamera("Camera1");
    }
}
