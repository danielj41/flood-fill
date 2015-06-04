#include "menu_item.hpp"

#include "text.hpp"
#include <string>


#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "glm/glm.hpp"
#include "global_variables.hpp"
#include "GLIncludes.h"

MenuItem::MenuItem(std::string _msg, glm::vec4 _color, glm::vec4 _hoverColor, glm::vec2 _position, std::string _fontName, int _fontSize) : Text(_msg, _color, _position, _fontName, _fontSize), hoverColor(_hoverColor), normalColor(_color) {}


bool MenuItem::isMouseOver(glm::vec2 mousePos) {
    glm::vec2 textPos = getPosition();

    INFO("textPos: " << textPos.x << ", " << textPos.y);

    glm::vec2 pos = glm::vec2((mousePos.x / (Global::ScreenWidth/2.0)) - 1.0, 
                              -(mousePos.y / (Global::ScreenHeight/2.0) - 1.0));
    
return pos.x >= textPos.x && pos.x <= textPos.x + getTextWidth() && pos.y >= textPos.y && pos.y <= textPos.y + getTextHeight();
}

glm::vec4 MenuItem::getHoverColor() { return hoverColor; }
glm::vec4 MenuItem::getNormalColor() { return normalColor; }

