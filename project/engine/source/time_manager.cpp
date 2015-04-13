#include "time_manager.hpp"

#include "GLIncludes.h"

double TimeManager::timeStamp;

double TimeManager::getDeltaTime(){
    return getTimeStamp() - timeStamp;
}

double TimeManager::getTimeStamp(){
    return glfwGetTime();
}

void TimeManager::setTimeStamp(){
    timeStamp = getTimeStamp();
}

