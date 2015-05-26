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
    
    /* Mimicking Render Engine Setup */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
  
    
 
    fontEngine.updateWindowSize(800, 600);
  
    return 1;
}

void Menu::display() {

    glClearColor(1, 1, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_NONE);
    glEnable(GL_TEXTURE_2D);

    if (fontEngine.useFont(courierHandle, 24)) {
        fontEngine.setColor(0, 0, 0, 1.0);
        fontEngine.renderText("The Quick Brown Fox Jumps Over The Lazy Dog", -1, .5);
    }
   
}
    

