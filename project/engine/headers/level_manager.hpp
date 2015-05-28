#ifndef LEVEL_MANAGER_HPP
#define LEVEL_MANAGER_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "pointer_macros.h"

class LevelManager{

public:
    static void resetLevel();
    static void nextLevel();
    static bool levelFinished;

private:
    static int curlevel; 
    static float timeStamp;
    static void setupFirstLevel();
    static void setupSecondLevel();
    static void removeFirstLevel();
    static void removeSecondLevel();
};

#endif