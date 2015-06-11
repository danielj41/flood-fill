#include "switch_page_button.hpp"

#include "menu.hpp"

#include <string>
#include "glm/glm.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"


SwitchPageButton::SwitchPageButton(std::string _targetPage, std::string _msg, glm::vec4 _color, glm::vec4 _hoverColor, glm::vec3 _position, std::string _fontName, int _fontSize) : MenuItem(_msg, _color, _hoverColor, _position, _fontName, _fontSize), targetPage(_targetPage) {}

void SwitchPageButton::action() {
    Menu::displayPage(targetPage);
}
