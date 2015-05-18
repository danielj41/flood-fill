#include "load_manager.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

//TODO: Remove this class from the engine, too specific for the flood fill project

std::map<std::string, ShaderPtr> LoadManager::shaders;
std::map<std::string, MeshPtr>   LoadManager::meshes;
std::map<std::string, ImagePtr> LoadManager::images;
std::map<std::string, TexturePtr> LoadManager::textures;
std::map<std::string, RenderTexturePtr> LoadManager::renderTextures;
std::map<std::string, SoundPtr> LoadManager::sounds;

const std::string LoadManager::shaderFolder = "flood-fill/shaders";
const std::string LoadManager::meshFolder = "flood-fill/objects";
const std::string LoadManager::imageFolder = "flood-fill/textures";
const std::string LoadManager::soundFolder = "flood-fill/sounds";

void LoadManager::loadShader(std::string vertex, std::string fragment){
    INFO("LoadManager: Loading shader " << vertex << " " << fragment << "...");

    std::string vertexPath = buildPath(shaderFolder, vertex);
    std::string fragmentPath = buildPath(shaderFolder, fragment);

    std::string key = vertex + fragment;
    if(shaders.find(key) == shaders.end()){
        shaders[key] = ShaderPtr(new Shader(vertexPath, fragmentPath));
        shaders[key]->load();
    }

    INFO("LoadManager: Shader " << vertex << " " << fragment << " loaded!");
}

void LoadManager::loadMesh(std::string filename){
    INFO("LoadManager: Loading mesh " << filename << "...");

    std::string path = buildPath(meshFolder, filename);
    if(meshes.find(filename) == meshes.end()){
        meshes[filename] = MeshPtr(new Mesh(path));
        meshes[filename]->load();
    }

    INFO("LoadManager: Mesh " << filename << " loaded!");
}

void LoadManager::loadImage(std::string filename){
    INFO("LoadManager: Loading image " << filename << "...");

    std::string path = buildPath(imageFolder, filename);
    if(images.find(filename) == images.end()){
        images[filename] = ImagePtr(new Image(path));
        images[filename]->load();
    }

    INFO("LoadManager: Image " << filename << " loaded!");
}

void LoadManager::loadTexture(std::string name, ImagePtr image){
    INFO("LoadManager: Loading texture " << name <<
            "form image " << image->getFileName() << "...");

    if(textures.find(name) == textures.end()){
        textures[name] = TexturePtr(new Texture());
        textures[name]->createTextureFromImage(image, true);
    }

    INFO("LoadManager: Texture " << name << " loaded!");
}

void LoadManager::loadRenderTexture(std::string name){

    if(renderTextures.find(name) == renderTextures.end()){
        renderTextures[name] = RenderTexturePtr(new RenderTexture());
        renderTextures[name]->load();
    }

    INFO("LoadManager: RenderTexture " << name << " loaded!");
}

void LoadManager::loadSound(std::string name){
    INFO("LoadManager: Loading sound " << name << "...");

    std::string path = buildPath(soundFolder, name);
    if(sounds.find(name) == sounds.end()){
        sounds[name] = SoundPtr(new Sound(path));
        sounds[name]->load();
    }

    INFO("LoadManager: Sound " << name << " loaded!");
}

ShaderPtr LoadManager::getShader(std::string vertex, std::string fragment){
    ASSERT(shaders.find(vertex + fragment) != shaders.end(),
        "The shader " << vertex << " " << fragment << " is not loaded!");
    return shaders[vertex + fragment];
}

MeshPtr LoadManager::getMesh(std::string name){
    ASSERT(meshes.find(name) != meshes.end(),
        "The mesh " << name << " is not loaded!");
    return meshes[name];
}

ImagePtr LoadManager::getImage(std::string name){
    ASSERT(images.find(name) != images.end(),
        "The Image " << name << " is not loaded!");
    return images[name];
}

TexturePtr LoadManager::getTexture(std::string name){
    ASSERT(textures.find(name) != textures.end(),
        "The texture " << name << " is not loaded!");
    return textures[name];
}

RenderTexturePtr LoadManager::getRenderTexture(std::string name){
    ASSERT(renderTextures.find(name) != renderTextures.end(),
        "The texture " << name << " is not loaded!");
    return renderTextures[name];
}

SoundPtr LoadManager::getSound(std::string name){
    ASSERT(sounds.find(name) != sounds.end(),
        "The sound " << name << " is not loaded!");
    return sounds[name];
}

/** Private Methods **/

std::string LoadManager::buildPath(std::string folder, std::string filename){
    std::string path = folder;
    path.append("/");
    path.append(filename);
    return path;
}
