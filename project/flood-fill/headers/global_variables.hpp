#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "GLIncludes.h"

class Global{

public:
    static const int ScreenWidth;
    static const int ScreenHeight;
    static int FbWidth; // twice ScreenWidth for higher pixel density monitors
    static int FbHeight;
    static GLFWwindow * window;
};

#endif
