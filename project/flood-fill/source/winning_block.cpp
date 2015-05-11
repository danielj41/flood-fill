#include "winning_block.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

WinningBlock::WinningBlock(glm::vec3 _position) : ActionInvisibleBlock(_position) {}

void WinningBlock::setCollisionIDs(){
    setCollisionID(128);
    setCollideWithID(2);
}

void WinningBlock::doAction(){
    INFO("#################### You got in the end of this Level!!! ##########################");
}
