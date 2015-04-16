#include "time_manager.hpp"

#include "GLIncludes.h"

double TimeManager::timeStamp;
double TimeManager::deltaTime;

void TimeManager::setDeltaTime(){
    deltaTime = getTimeStamp() - timeStamp;
}

double TimeManager::getTimeStamp(){
    return glfwGetTime();
}

void TimeManager::setTimeStamp(){
    timeStamp = getTimeStamp();
}

double TimeManager::getDeltaTime(){
    return deltaTime;
}
