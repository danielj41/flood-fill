#include "void_voxel.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"


VoidVoxel::VoidVoxel() : GameObject() {}

void VoidVoxel::setup(){
    DEBUG("Setup a void voxel.!");
}

void VoidVoxel::update(){
    DEBUG("Updating a void voxel.!");
}
