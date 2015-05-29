#ifndef TEXT_HPP
#define TEXT_HPP

#include <string>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "pointer_macros.h"
DEF_PTR(Text, TextPtr);

class Text {
public:
    Text(std::string _msg, glm::vec4 _color, glm::vec2 _position,
         std::string _fontName, int _fontSize);

    void setText(std::string _msg);
    void setColor(glm::vec4 _color);
    void setPosition(glm::vec2 _postion);
    void setFont(std::string _fontName);
    void setFontSize(int size);

    std::string getText();
    glm::vec4 getColor();
    glm::vec2 getPosition();
    std::string getFont();
    int getFontSize();

private:
    std::string msg;
    glm::vec4 color;
    glm::vec2 position;
    std::string fontName;
    int fontSize;
};

#endif
