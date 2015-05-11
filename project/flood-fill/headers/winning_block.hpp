#ifndef WINNING_BLOCK_BLOCK_HPP
#define WINNING_BLOCK_BLOCK_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "action_invisible_block.hpp"

class WinningBlock : public ActionInvisibleBlock {

public:
    WinningBlock(glm::vec3 _position);

    void setCollisionIDs();

    void doAction();
};

#endif
