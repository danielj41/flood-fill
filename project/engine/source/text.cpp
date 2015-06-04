#include "text.hpp"
#include "global_variables.hpp"


Text::Text(std::string _msg, glm::vec4 _color, glm::vec2 _position,
            std::string _fontName, int _fontSize)
  : msg(_msg), color(_color), position(_position), fontName(_fontName),
    fontSize(_fontSize) {}

void Text::setText(std::string _msg) { msg = _msg; }
void Text::setColor(glm::vec4 _color) { color = _color; }
void Text::setPosition(glm::vec2 _postion) { position = _postion; }
void Text::setFont(std::string _fontName) { fontName = _fontName; }
void Text::setFontSize(int size) { fontSize = size; }

std::string Text::getText() { return msg; }
glm::vec4 Text::getColor() { return color; }
glm::vec2 Text::getPosition() { return position; }
std::string Text::getFont() { return fontName; }
int Text::getFontSize() { return fontSize; }
float Text::getCharWidth() { return fontSize * 1.0 * (2.0 / Global::ScreenWidth); }
float Text::getCharHeight() { return fontSize * 1.0 * (2.0 / Global::ScreenHeight); }
float Text::getTextHeight() { return getCharHeight(); }

float Text::getTextWidth() {
    int len = 0;
    for ( ; msg[len]; len++);
    return len * getCharWidth();
}
