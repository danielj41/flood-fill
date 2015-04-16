#include "main_level.hpp"

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

MainLevel::MainLevel()
    : Scene("MainLevel"), maxNumObstacles(15), numObstaclesLeft(0),
      lastSpawnTime(0.0), timeToSpawn(2.0){}

void MainLevel::setup(){
    INFO("Setting up the Main Level...");

    Camera * cam1 = new Camera(glm::vec3(0, 1, 0), glm::vec3(0, 0, -5),
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

    GroundPlane * ground = new GroundPlane();
    ground->setup();
    addGameObject("Ground" , ground);


    player = new Player(cam1);
    player->setup();
    addGameObject("player" , player);
    CollisionManager::addCollisionObject(player);

    debugPlayer = new DebugPlayer(cam2);
    debugPlayer->setup();
    addGameObject("debugPlayer" , debugPlayer);

    InvisibleWall * wall1 = new InvisibleWall(glm::vec3(-30, 0, 0),
                                              glm::vec3(0.5, 10, 30));
    wall1->setup();
    addGameObject("wall1", wall1);
    CollisionManager::addCollisionObject(wall1);

    InvisibleWall * wall2 = new InvisibleWall(glm::vec3(30, 0, 0),
                                              glm::vec3(0.5, 10, 30));
    wall2->setup();
    addGameObject("wall2", wall2);
    CollisionManager::addCollisionObject(wall2);

    InvisibleWall * wall3 = new InvisibleWall(glm::vec3(0, 0, 30),
                                              glm::vec3(30, 10, 0.5));
    wall3->setup();
    addGameObject("wall3", wall3);
    CollisionManager::addCollisionObject(wall3);

    InvisibleWall * wall4 = new InvisibleWall(glm::vec3(0, 0, -30),
                                              glm::vec3(30, 10, 0.5));
    wall4->setup();
    addGameObject("wall4", wall4);
    CollisionManager::addCollisionObject(wall4);
}

void MainLevel::update(){
    DEBUG("Obstacles in the screen: " <<
            (numObstaclesLeft - player->getPoints()));

    if(debugPlayer->isActive()){
        ASSERT(getCamera("Camera1") != getCamera("DebugCamera"), "Equal camera");
        setMainCamera("DebugCamera");
        getCamera("Camera1")->fix();
    }
    else{
        setMainCamera("Camera1");
        getCamera("Camera1")->fix(false, true, false);
    }

    if(TimeManager::getTimeStamp() - lastSpawnTime >= timeToSpawn){
        lastSpawnTime = TimeManager::getTimeStamp();
        placeNewObstacle();
    }
}

void MainLevel::placeNewObstacle(){
    if((numObstaclesLeft - player->getPoints()) < maxNumObstacles){
        srand(TimeManager::getTimeStamp());

        float x = rand() % 51 - 25;
        float z = rand() % 51 - 25;

        float dx = rand() % 3 - 1;
        float dz = rand() % 3 - 1;
        if(dx == 0.0f && dz == 0.0f) dz = -1;

        Obstacle * obs = new Obstacle(glm::vec3(x, 1, z),
                                      glm::vec3(dx, 0, dz), 3.0f);
        obs->setup();

        std::stringstream ss;
        ss << "obstacle" << rand();

        addGameObject(ss.str() , obs);
        CollisionManager::addCollisionObject(obs);
        numObstaclesLeft++;
    }
}
