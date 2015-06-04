#ifndef MENU_ITEM_H
#define MENU_ITEM_H

#include <string>
#include "glm/glm.hpp"
#include "text.hpp"
#include "pointer_macros.h" 
DEF_PTR(MenuItem, MenuItemPtr);

class MenuItem : public Text{
    
public:

    MenuItem(std::string _msg, glm::vec4 _color, glm::vec4 _hoverColor, glm::vec2 _position, std::string _fontName, int _fontSize);

    virtual void action() = 0;
   
    bool isMouseOver(glm::vec2 mousePos);
    glm::vec4 getHoverColor();
    glm::vec4 getNormalColor();

private:
    glm::vec4 hoverColor;
    glm::vec4 normalColor;
    
};
    
#endif
