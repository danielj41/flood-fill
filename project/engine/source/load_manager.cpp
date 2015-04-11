#include "load_manager.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

std::map<std::string, Shader *> LoadManager::shaders;
std::map<std::string, Mesh *>   LoadManager::meshes;

const std::string LoadManager::shaderFolder = "flood-fill/shaders";
const std::string LoadManager::meshFolder = "flood-fill/objects";


void LoadManager::loadShader(std::string vertex, std::string fragment){
    INFO("LoadManager: Loading shader " << vertex << " " << fragment << "...");

    std::string vertexPath = buildPath(shaderFolder, vertex);
    std::string fragmentPath = buildPath(shaderFolder, fragment);

    std::string key = vertex + fragment;
    if(shaders.find(key) == shaders.end()){
        shaders[key] = new Shader(vertexPath, fragmentPath);
        shaders[key]->load();
    }

    INFO("LoadManager: Shader " << vertex << " " << fragment << " loaded!");
}

void LoadManager::loadMesh(std::string filename){
    INFO("LoadManager: Loading mesh " << filename << "...");

    std::string path = buildPath(meshFolder, filename);
    if(meshes.find(filename) == meshes.end()){
        meshes[filename] = new Mesh(path);
        meshes[filename]->load();
    }

    INFO("LoadManager: Mesh " << filename << " loaded!");
}

Shader * LoadManager::getShader(std::string vertex, std::string fragment){
    ASSERT(shaders.find(vertex + fragment) != shaders.end(),
        "The shader " << vertex << " " << fragment << " is not loaded!");
    return shaders[vertex + fragment];
}

Mesh * LoadManager::getMesh(std::string name){
    ASSERT(meshes.find(name) != meshes.end(),
        "The mesh " << name << " is not loaded!");
    return meshes[name];
}

/** Private Methods **/

std::string LoadManager::buildPath(std::string folder, std::string filename){
    std::string path = folder;
    path.append("/");
    path.append(filename);
    return path;
}
