#ifndef MATERIAL_MANAGER_HPP
#define MATERIAL_MANAGER_HPP

#include <map>
#include <string>

#include "material.hpp"

class MaterialManager{

public:
    static void addMaterial(std::string name, Material * material);
    static Material * getMaterial(std::string name);
    static void removeMaterial(std::string name);

private:
    static std::map<std::string, Material *> materials;
};

#endif
