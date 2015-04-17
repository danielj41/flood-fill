#include "material_manager.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

std::map<std::string, Material *> MaterialManager::materials;

void MaterialManager::addMaterial(std::string name, Material * material){
    materials[name] = material;

    INFO("Material " << name << " added!");
}

Material * MaterialManager::getMaterial(std::string name){
    ASSERT(materials.find(name) != materials.end(),
           "The material " << name << " does not exist!");

    return materials[name];
}

void MaterialManager::removeMaterial(std::string name){
    ASSERT(materials.find(name) != materials.end(),
           "The material " << name << " does not exist!");

    materials.erase(materials.find(name));
}
