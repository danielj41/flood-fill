#ifndef MENU_H
#define MENU_H

#include "FontEngine.h"

class Menu { 

public:
    
    Menu();
    
    bool setup();
    void display();

private:

    FontEngine fontEngine; 

    const std::string courierHandle = "Courier";
};

#endif
