#ifndef SWITCH_PAGE_BUTTON_HPP
#define SWITCH_PAGE_BUTTON_HPP

#include "menu_item.hpp"
#include <string>

#include "pointer_macros.h"
DEF_PTR(SwitchPageButton, SwitchPageButtonPtr);

class SwitchPageButton : public MenuItem {
    
public:
    SwitchPageButton(std::string _targetPage, std::string _msg, glm::vec4 _color, glm::vec4 _hoverColor, glm::vec2 _position, std::string _fontName, int _fontSize);

    void action();

private:
    std::string targetPage;
};

#endif
