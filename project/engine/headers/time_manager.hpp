#ifndef TIME_MANAGER_HPP
#define TIME_MANAGER_HPP

class TimeManager{

public:
    static double getDeltaTime();
    static double getTimeStamp();

    static void setTimeStamp();

private:
    static double timeStamp;
};

#endif
