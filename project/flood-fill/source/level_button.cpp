#include "level_button.hpp"

#include "menu.hpp"
#include "level_manager.hpp"

#include <string>
#include "glm/glm.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"


LevelButton::LevelButton(int _level, std::string _msg, glm::vec4 _color, glm::vec4 _hoverColor, glm::vec2 _position, std::string _fontName, int _fontSize) : MenuItem(_msg, _color, _hoverColor, _position, _fontName, _fontSize), level(_level) {}

void LevelButton::action() {
    
    Menu::setNewLevel(true);
    LevelManager::removeLevel(LevelManager::getCurrentLevel());
    LevelManager::setupLevel(level);
    Menu::hideMenu();
}
