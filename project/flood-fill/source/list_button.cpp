#include "list_button.hpp"

#include "menu.hpp"
#include "page.hpp"

#include <string>
#include "glm/glm.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"




ListButton::ListButton(std::string _page, std::string _name, int _direction, std::string _msg, glm::vec4 _color, glm::vec4 _hoverColor, glm::vec3 _position, std::string _fontName, int _fontSize) : MenuItem(_msg, _color, _hoverColor, _position, _fontName, _fontSize), page(_page), direction(_direction) {}

void ListButton::action() {
    Menu::getPage(page)->changeLevel(direction);
}
