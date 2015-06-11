#include "billboard_text.hpp"
#include "load_manager.hpp"
#include "director.hpp"
#include "material_manager.hpp"
#include "render_engine.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#include <string>

BillboardText::BillBoardText(std::string _msg, glm::vec3 _position, glm::vec2 _rotationLimits, bool _alwaysVisible, float _distance) : GameObject(), msg(_msg), position(_position), rotationLimits(_rotationLimits), alwaysVisible(_alwaysVisible) {}

void BillboardText::setup() {
    INFO("Creating Billboard Text \"" << msg << "\"");
    
    for (int i = 0; msg[i]; i++) {
        character 
