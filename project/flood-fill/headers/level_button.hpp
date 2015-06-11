#ifndef LEVEL_BUTTON_HPP
#define LEVEL_BUTTON_HPP

#include "menu_item.hpp"
#include <string>

#include "pointer_macros.h"
DEF_PTR(LevelButton, LevelButtonPtr);

class LevelButton : public MenuItem {
    
public:
    LevelButton(int _level, std::string _msg, glm::vec4 _color, glm::vec4 _hoverColor, glm::vec3 _position, std::string _fontName, int _fontSize);

    void action();

private:
    int level;
};

#endif
