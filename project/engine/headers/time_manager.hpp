#ifndef TIME_MANAGER_HPP
#define TIME_MANAGER_HPP

#include "pointer_macros.h"
DEF_PTR(TimeManager, TimeManagerPtr);

class TimeManager{

public:
    static double getTimeStamp();
    static double getDeltaTime();
    static void setDeltaTime();
    static void setTimeStamp();

private:
    static double timeStamp;
    static double deltaTime;
};

#endif
