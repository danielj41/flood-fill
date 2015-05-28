#include "menu.hpp"

#include <iostream>
#include <cstdlib>

#include "debug_macros.h"

#include "FontEngine.h"

Menu::Menu(){}

bool Menu::setup() { 
    INFO("Initializing menu fontEngine");
    if (!fontEngine.init()) {
        return 0;
    }

    INFO("Adding Courier Font");
    if (!fontEngine.addFont(courierHandle, "flood-fill/fonts/Courier.ttf")) {
        return 0;
    };
      
    fontEngine.updateWindowSize(1600, 1200);

    return 1;
}

void Menu::display() {
    glClear(GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthMask(GL_TRUE);

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);

    if (fontEngine.useFont(courierHandle, 24)) {
        fontEngine.setColor(1, 0, 0, 1.0);
        fontEngine.renderText("The Quick Brown Fox Jumps Over The Lazy Dog", -1, .5);
    }
}
