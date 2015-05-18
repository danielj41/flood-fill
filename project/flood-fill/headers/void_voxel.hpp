#ifndef VOID_VOXEL_HPP
#define VOID_VOXEL_HPP

#include "game_object.hpp"

#include "pointer_macros.h"
DEF_PTR(VoidVoxel, VoidVoxelPtr);

class VoidVoxel : public GameObject{

public:
    VoidVoxel();

    void setup();
    void update();
};

#endif
