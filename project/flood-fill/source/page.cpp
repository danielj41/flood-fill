#include "page.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "text_render.hpp"
#include "menu_item.hpp"
#include "render_engine.hpp"
#include "global_variables.hpp"
#include "time_manager.hpp"
#include "level_manager.hpp"
#include "menu.hpp"

Page::Page() : lvlndx(0), numLevels(0) {}

void Page::addToPageButton(std::string name, MenuItemPtr menuItem){
    menuItems[name] = menuItem;
}

void Page::addToPageLevel(MenuItemPtr levelButton) {
    levels[numLevels++] = levelButton;
}

void Page::addToPageInertText(std::string name, TextPtr text) {
    inertText[name] = text;
}
    
void Page::removeFromPageButton(std::string name){
    for(auto it = menuItems.begin(); it != menuItems.end(); it++){
        if(it->first == name){
            menuItems.erase(it);
            INFO("MenuItem Removed!");
            return;
        }
    }
}


void Page::changeLevel(int direction) {
    PTR_CAST(TextRender, RenderEngine::getRenderElement("text"))->removeText((TextPtr)levels[lvlndx]);

    lvlndx += direction;
    
    if (lvlndx >= numLevels) lvlndx = 0;
    if (lvlndx < 0 ) lvlndx = numLevels-1;

    PTR_CAST(TextRender, RenderEngine::getRenderElement("text"))->addText((TextPtr)levels[lvlndx]);    
}


void Page::display(){
    for(auto it = menuItems.begin(); it != menuItems.end(); it++){
        PTR_CAST(TextRender, RenderEngine::getRenderElement("text"))->addText((TextPtr)it->second);
    }
    
    for(auto it = inertText.begin(); it != inertText.end(); it++){
        PTR_CAST(TextRender, RenderEngine::getRenderElement("text"))->addText(it->second);
    }

    if ( numLevels ) {
        PTR_CAST(TextRender, RenderEngine::getRenderElement("text"))->addText((TextPtr)levels[0]);
    }
}

void Page::hide(){
    for(auto it = menuItems.begin(); it != menuItems.end(); it++){
        PTR_CAST(TextRender, RenderEngine::getRenderElement("text"))->removeText((TextPtr)it->second);
    }
    
    for(auto it = inertText.begin(); it != inertText.end(); it++){
        PTR_CAST(TextRender, RenderEngine::getRenderElement("text"))->removeText(it->second);
    }
    
    
    if ( numLevels ) {    
        PTR_CAST(TextRender, RenderEngine::getRenderElement("text"))->removeText((TextPtr)levels[lvlndx]);
        lvlndx = 0;
    }
    
}

void Page::update(){
    double xpos, ypos;

    glfwGetCursorPos(Global::window, &xpos, &ypos);
    
    for(auto it = menuItems.begin(); it != menuItems.end(); it++){
        if (it->second->isMouseOver(glm::vec2(xpos, ypos))) {
            it->second->setColor(it->second->getHoverColor());
            
            if(TimeManager::getTimeStamp() - Menu::getLastClick() >= .25 && glfwGetMouseButton(Global::window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS) {
                Menu::setLastClick(TimeManager::getTimeStamp());
                it->second->action();
            }

        } else {
            it->second->setColor(it->second->getNormalColor());
        }
    }
    
    if ( numLevels ) {
        if ( levels[lvlndx]->isMouseOver(glm::vec2(xpos,ypos))) {
            levels[lvlndx]->setColor(levels[lvlndx]->getHoverColor());
            if(TimeManager::getTimeStamp() - Menu::getLastClick() >= .25 && glfwGetMouseButton(Global::window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS ) {
                Menu::setLastClick(TimeManager::getTimeStamp());
                levels[lvlndx]->action();
            }

        } else {
            levels[lvlndx]->setColor(levels[lvlndx]->getNormalColor());
        }
    }
}
