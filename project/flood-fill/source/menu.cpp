#include "menu.hpp"

#include "page.hpp"
#include "menu_item.hpp"
#include "switch_page_button.hpp"
#include "level_button.hpp"
#include "list_button.hpp"
#include "text.hpp"
#include "GLIncludes.h"
#include "global_variables.hpp"
#include "render_engine.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "pointer_macros.h"
#include "glm/glm.hpp"

bool Menu::active = true;
bool Menu::newLevel = false;
bool Menu::novice = true;
std::map< std::string, PagePtr > Menu::pages;
std::string Menu::currentPage;
double Menu::lastClick;
double Menu::lastEscape;

void Menu::setup(){
    //////////////////////////////////////////////////////////////////////////
    // HOME PAGE
    
    PagePtr home;

    home = PagePtr(new Page());

    MenuItemPtr title, quitButton;
    
    title = (MenuItemPtr)new SwitchPageButton("levels","FLOOD_FILL", glm::vec4(0,0,0,.7), glm::vec4(0,0,1,1), glm::vec3(0,0,0), "FourPixel", 50 * Global::FbHeight/600.0);

    quitButton = (MenuItemPtr)new SwitchPageButton("", "QUIT", glm::vec4(0,0,0,.7), glm::vec4(1,0,0,1), glm::vec3(0,0,0), "FourPixel", 25 * Global::FbHeight/600.0);

    float pixelDensityX = (float)Global::FbWidth / Global::ScreenWidth;

    title->setPosition(glm::vec3(0 - title->getTextWidth() / 2.0/pixelDensityX, 0, 0));
    quitButton->setPosition(glm::vec3(0 - quitButton->getTextWidth() / 2.0/pixelDensityX, -0.75, 0));

    home->addToPageButton("title", title);
    home->addToPageButton("quit", quitButton);
    
    addPage("home", home);
  
    ////////////////////////////////////////////////////////////////////////////
    // LEVELS PAGE

    PagePtr levels;

    levels = PagePtr(new Page());

    MenuItemPtr backButton, next, previous, level1, level2, level3, level4;

    backButton = (MenuItemPtr)new SwitchPageButton("home","BACK", glm::vec4(0,0,0,.7), glm::vec4(1,0,0,1), glm::vec3(0, 0, 0), "FourPixel", 25 * Global::FbHeight/600.0);
    
    level1 = (MenuItemPtr)new LevelButton(1, "Level1", glm::vec4(0,0,0,.7), glm::vec4(0,1,0,1), glm::vec3(0,0,0), "FourPixel", 37 * Global::FbHeight/600.0);
    
    level2 = (MenuItemPtr)new LevelButton(2, "Level2", glm::vec4(0,0,0,.7), glm::vec4(0,1,0,1), glm::vec3(0,0,0), "FourPixel", 37 * Global::FbHeight/600.0);

    level3 = (MenuItemPtr)new LevelButton(3, "Level3", glm::vec4(0,0,0,.7), glm::vec4(0,1,0,1), glm::vec3(0,0,0), "FourPixel", 37 * Global::FbHeight/600.0);

    //level4 = (MenuItemPtr)new LevelButton(4, "Boss", glm::vec4(0,0,0,.7), glm::vec4(0,1,0,1), glm::vec2(0,0), "FourPixel", 37 * Global::FbHeight/600.0);

    previous = (MenuItemPtr)new ListButton("levels", "previous", -1, "<<", glm::vec4(0,0,0,.2), glm::vec4(0,0,0,1), glm::vec3(0,0,0), "Courier", 37 * Global::FbHeight/600.0);
    
    next = (MenuItemPtr)new ListButton("levels", "next", 1, ">>", glm::vec4(0,0,0,.2), glm::vec4(0,0,0,1), glm::vec3(0,0,0), "Courier", 37 * Global::FbHeight/600.0);
                                          
    

    backButton->setPosition(glm::vec3(0 - backButton->getTextWidth()/2.0/pixelDensityX, -0.75, 0));
    level1->setPosition(glm::vec3(0 - level1->getTextWidth()/2.0/pixelDensityX, 0, 0));
    level2->setPosition(glm::vec3(0 - level2->getTextWidth()/2.0/pixelDensityX, 0, 0));
    level3->setPosition(glm::vec3(0 - level3->getTextWidth()/2.0/pixelDensityX, 0, 0));
    //level4->setPosition(glm::vec3(0 - level3->getTextWidth()/2.0/pixelDensityX, 0, 0));
    previous->setPosition(glm::vec3(0 - level1->getTextWidth()/2.0/pixelDensityX - previous->getTextWidth()/pixelDensityX, 0, 0));
    next->setPosition(glm::vec3(0 + level1->getTextWidth()/2.0/pixelDensityX, 0, 0));

    levels->addToPageButton("back", backButton);
    levels->addToPageLevel(level1);
    levels->addToPageLevel(level2);
    levels->addToPageLevel(level3);
    //levels->addToPageLevel(level4);
    levels->addToPageButton("previous", previous);
    levels->addToPageButton("next", next);
      
    addPage("levels", levels);

    /////////////////////////////////////////////////////////////////////////////////
    // TUTORIAL GUIDE 
    
    PagePtr userGuide;
    
    userGuide = PagePtr(new Page());

    MenuItemPtr comprehend = (MenuItemPtr)new SwitchPageButton("hide","OK!", glm::vec4(0,0,0,.7), glm::vec4(0,0,1,1), glm::vec3(0, 0, 0), "FourPixel", 30 * Global::FbHeight/600.0);
    comprehend->setPosition(glm::vec3(0 - comprehend->getTextWidth()/2.0/pixelDensityX, -0.8, 0));
    
    userGuide->addToPageButton("comprehend", comprehend);

    TextPtr guideTitle = (TextPtr)new Text("User Guide", glm::vec4(0,0,0,1), glm::vec3(0,0,0), "FourPixel", 40 * Global::FbHeight/600.0);
    guideTitle->setPosition(glm::vec3(0 - guideTitle->getTextWidth()/2.0/pixelDensityX, .75, 0));

    userGuide->addToPageInertText("guideTitle", guideTitle);

    TextPtr blue = (TextPtr)new Text("BLUE", glm::vec4(0,0,1,1), glm::vec3(0,0,0), "FourPixel", 25*Global::FbHeight/600.0);
    blue->setPosition(glm::vec3(0 - blue->getTextWidth()/2.0/pixelDensityX - .25, .45, 0));

    userGuide->addToPageInertText("blue", blue);
    
    TextPtr green = (TextPtr)new Text("GREEN", glm::vec4(0,1,0,1), glm::vec3(0,0,0), "FourPixel", 25*Global::FbHeight/600.0);
    green->setPosition(glm::vec3(0 - green->getTextWidth()/2.0/pixelDensityX - .25, .35, 0));
    
    userGuide->addToPageInertText("green", green);
    
    TextPtr red = (TextPtr)new Text("RED", glm::vec4(1,0,0,1), glm::vec3(0,0,0), "FourPixel", 25*Global::FbHeight/600.0);
    red->setPosition(glm::vec3(0 - red->getTextWidth()/2.0/pixelDensityX - .25, .25, 0));

    userGuide->addToPageInertText("red", red);

    TextPtr assignments = (TextPtr)new Text("= JUMP\n= SPEED\n= SPEED + JUMP", glm::vec4(0,0,0,1), glm::vec3(0,0,0), "Courier", 35*Global::FbHeight/600.0);
    assignments->setPosition(glm::vec3(0 - assignments->getTextWidth()/2.0/pixelDensityX + 1.22, .45, 0));

    userGuide->addToPageInertText("assignments", assignments);

    TextPtr keybinds = (TextPtr)new Text("'Q' = TOGGLE COLOR\n'E' = TRIGGER SWITCH", glm::vec4(0,0,0,1), glm::vec3(0,0,0), "Courier", 30*Global::FbHeight/600.0);
    keybinds->setPosition(glm::vec3(0 - keybinds->getTextWidth()/2.0/pixelDensityX + 1, 0, 0));
    userGuide->addToPageInertText("keybinds", keybinds);

    TextPtr info = (TextPtr)new Text("Fill and remove blocks of the color equipped\n\n\nin order to reach the end of the stage (!)", glm::vec4(0,0,0,1), glm::vec3(0,0,0), "Courier", 28*Global::FbHeight/600.0);
    info->setPosition(glm::vec3(0 - info->getTextWidth()/2.0/pixelDensityX + 2.2, -.3, 0));
    userGuide->addToPageInertText("info", info);
    
    TextPtr mouse = (TextPtr)new Text("(LEFT/RIGHT MOUSE BUTTONS)", glm::vec4(0,0,0,1), glm::vec3(0,0,0), "Courier", 30*Global::FbHeight/600.0);
    mouse->setPosition(glm::vec3(0 - mouse->getTextWidth()/2.0/pixelDensityX + .37, -.42, 0));
    userGuide->addToPageInertText("mouse", mouse);   

    addPage("UserGuide", userGuide);
  
    displayMenu(); 
}

void Menu::update() { pages[currentPage]->update(); }

void Menu::addPage(std::string name, PagePtr page){
    pages[name] = page;
}

void Menu::removePage(std::string name){
    for(auto it = pages.begin(); it != pages.end(); it++){
        if(it->first == name){
            pages.erase(it);
            return;
        }
    }
}

PagePtr Menu::getPage(std::string name){
    return pages[name];
}

void Menu::displayPage(std::string name){ 
    // TODO: Check if Valid Page
    if(name == "") {
      currentPage = "";
    } else if ( name == "hide") {
        glfwSetInputMode(Global::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        RenderEngine::toggleElementsForMenu();
        hidePage(currentPage);
        currentPage = "hide";
    } else if ( name != currentPage) {
        
        hidePage(currentPage);
        pages[name]->display();
        currentPage = name;               
    }
}

void Menu::hidePage(std::string name){ 
    // TODO: Check if Valid Page
    if (name != "hide") {
        pages[name]->hide(); 
    }
}

void Menu::hideMenu(){ 
    pages[currentPage]->hide();
    active = false;
    glfwSetInputMode(Global::window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //RenderEngine::toggleElementsForMenu();
}
    
void Menu::displayMenu(){
    active = true;
    glfwSetInputMode(Global::window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    currentPage = "home";
    pages["home"]->display(); 

    RenderEngine::toggleElementsForMenu();
}

bool Menu::isActive(){ return active; }

std::string Menu::getCurrentPage() { return currentPage; }

void Menu::setLastClick(double timeStamp) { lastClick = timeStamp; }
void Menu::setLastEscape(double timeStamp) { lastEscape = timeStamp; }

double Menu::getLastClick(){ return lastClick; }
double Menu::getLastEscape(){ return lastEscape; }
void Menu::setNewLevel(bool b) { newLevel = b; }
bool Menu::isNewLevel(){ return newLevel; }
bool Menu::isNovice() { return novice; }
void Menu::setNovice(bool b) { novice = b; }
