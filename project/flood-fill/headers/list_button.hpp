#ifndef LIST_BUTTON_HPP
#define LIST_BUTTON_HPP

#include "menu_item.hpp"
#include <string>

#include "pointer_macros.h"
DEF_PTR(ListButton, ListButtonPtr);

class ListButton : public MenuItem {
    
public:
    ListButton(std::string _page, std::string _name, int _direction, std::string _msg, glm::vec4 _color, glm::vec4 _hoverColor, glm::vec3 _position, std::string _fontName, int _fontSize);

    void action();

private:
    std::string page;
    int direction;
};

#endif
