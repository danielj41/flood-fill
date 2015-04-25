#include "test_level.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

TestLevel::TestLevel() : LevelTemplate("testLevel.txt"){}

void TestLevel::setup(){
    createLevel();
}

void TestLevel::update(){
}
