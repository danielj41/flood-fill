#ifndef MENU_HPP
#define MENU_HPP

#include <map>
#include <string>

#include "page.hpp"

#include "pointer_macros.h"
DEF_PTR(Menu, MenuPtr);

class Menu {

public:
    
    static void setup();    
    static void update();
    static void addPage(std::string name, PagePtr page);
    static void removePage(std::string name);
    static PagePtr getPage(std::string name);
    static void displayPage(std::string name);
    static void hidePage(std::string name);
    static void hideMenu();
    static void displayMenu();
    static bool isActive();  
    static std::string getCurrentPage();
    static void setLastClick(double timeStamp);
    static void setLastEscape(double timeStamp);
    static double getLastClick();
    static double getLastEscape();
    static void setNewLevel(bool b);
    static bool isNewLevel();

private:
    
    static std::map< std::string, PagePtr > pages;
    static bool active;
    static std::string currentPage;
    static double lastClick;
    static double lastEscape;
    static bool newLevel;

};

#endif
