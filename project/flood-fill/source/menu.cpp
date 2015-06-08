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
std::map< std::string, PagePtr > Menu::pages;
std::string Menu::currentPage;
double Menu::lastClick;
double Menu::lastEscape;

void Menu::setup(){
   
    PagePtr home;

    home = PagePtr(new Page());

    MenuItemPtr title, quitButton;
    
    title = (MenuItemPtr)new SwitchPageButton("levels","FLOOD_FILL", glm::vec4(0,0,0,.7), glm::vec4(0,0,1,1), glm::vec2(0, 0), "FourPixel", 50 * Global::FbHeight/600.0);

    quitButton = (MenuItemPtr)new SwitchPageButton("", "QUIT", glm::vec4(0,0,0,.7), glm::vec4(1,0,0,1), glm::vec2(0,0), "FourPixel", 25 * Global::FbHeight/600.0);

    float pixelDensityX = (float)Global::FbWidth / Global::ScreenWidth;

    title->setPosition(glm::vec2(0 - title->getTextWidth() / 2.0/pixelDensityX, 0));
    quitButton->setPosition(glm::vec2(0 - quitButton->getTextWidth() / 2.0/pixelDensityX, -0.75));

    home->addToPageButton("title", title);
    home->addToPageButton("quit", quitButton);
    
    addPage("home", home);
  
    ////////////////////////////////////////////////////////////////////////////
    
    PagePtr levels;

    levels = PagePtr(new Page());

    MenuItemPtr backButton, next, previous, level1, level2, level3, level4;

    backButton = (MenuItemPtr)new SwitchPageButton("home","BACK", glm::vec4(0,0,0,.7), glm::vec4(1,0,0,1), glm::vec2(0, 0), "FourPixel", 25 * Global::FbHeight/600.0);
    
    level1 = (MenuItemPtr)new LevelButton(1, "Level1", glm::vec4(0,0,0,.7), glm::vec4(0,1,0,1), glm::vec2(0,0), "FourPixel", 37 * Global::FbHeight/600.0);
    
    level2 = (MenuItemPtr)new LevelButton(2, "Level2", glm::vec4(0,0,0,.7), glm::vec4(0,1,0,1), glm::vec2(0,0), "FourPixel", 37 * Global::FbHeight/600.0);

    level3 = (MenuItemPtr)new LevelButton(3, "Level3", glm::vec4(0,0,0,.7), glm::vec4(0,1,0,1), glm::vec2(0,0), "FourPixel", 37 * Global::FbHeight/600.0);

    level4 = (MenuItemPtr)new LevelButton(4, "Boss", glm::vec4(0,0,0,.7), glm::vec4(0,1,0,1), glm::vec2(0,0), "FourPixel", 37 * Global::FbHeight/600.0);

    previous = (MenuItemPtr)new ListButton("levels", "previous", -1, "<<", glm::vec4(0,0,0,.2), glm::vec4(0,0,0,1), glm::vec2(0,0), "Courier", 37 * Global::FbHeight/600.0);
    
    next = (MenuItemPtr)new ListButton("levels", "next", 1, ">>", glm::vec4(0,0,0,.2), glm::vec4(0,0,0,1), glm::vec2(0,0), "Courier", 37 * Global::FbHeight/600.0);
                                          
    

    backButton->setPosition(glm::vec2(0 - backButton->getTextWidth()/2.0/pixelDensityX, -0.75));
    level1->setPosition(glm::vec2(0 - level1->getTextWidth()/2.0/pixelDensityX, 0));
    level2->setPosition(glm::vec2(0 - level2->getTextWidth()/2.0/pixelDensityX, 0));
    level3->setPosition(glm::vec2(0 - level3->getTextWidth()/2.0/pixelDensityX, 0));
    level4->setPosition(glm::vec2(0 - level3->getTextWidth()/2.0/pixelDensityX, 0));
    previous->setPosition(glm::vec2(0 - level1->getTextWidth()/2.0/pixelDensityX - previous->getTextWidth()/pixelDensityX, 0));
    next->setPosition(glm::vec2(0 + level1->getTextWidth()/2.0/pixelDensityX, 0));

    levels->addToPageButton("back", backButton);
    levels->addToPageLevel(level1);
    levels->addToPageLevel(level2);
    levels->addToPageLevel(level3);
    levels->addToPageLevel(level4);
    levels->addToPageButton("previous", previous);
    levels->addToPageButton("next", next);
      
    addPage("levels", levels);
    
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
    } else if ( name != currentPage) {
        
        hidePage(currentPage);
        pages[name]->display();
        currentPage = name;               
    }


}

void Menu::hidePage(std::string name){ 
    // TODO: Check if Valid Page
    pages[name]->hide(); 
    currentPage = "";   
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
