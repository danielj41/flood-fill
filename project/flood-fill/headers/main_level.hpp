#ifndef MAIN_LEVEL_HPP
#define MAIN_LEVEL_HPP

#include "scene.hpp"
#include "debug_player.hpp"
#include "player.hpp"

#include "pointer_macros.h"
DEF_PTR(MainLevel, MainLevelPtr);

class MainLevel : public Scene{

public:
    MainLevel();

    void setup();
    void update();

private:
    DebugPlayerPtr debugPlayer;
    PlayerPtr player;

    int  maxNumObstacles, numObstaclesLeft;
    double lastSpawnTime, timeToSpawn;

    void placeNewObstacle();
};

#endif
