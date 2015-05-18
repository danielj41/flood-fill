#ifndef LOAD_MANAGER_H
#define LOAD_MANAGER_H

#include <string>
#include <map>

#include "shader.hpp"
#include "mesh.hpp"
#include "image.hpp"
#include "texture.hpp"
#include "render_texture.hpp"
#include "sound.hpp"
// TODO: create an unload operation to delete the pointers created

#include "pointer_macros.h"
DEF_PTR(LoadManager, LoadManagerPtr);

class LoadManager {
public:
    static void loadShader(std::string vertex, std::string fragment);
    static void loadMesh(std::string filename);
    static void loadImage(std::string filename);
    static void loadTexture(std::string name, Image * image);
    static void loadRenderTexture(std::string name);
    static void loadSound(std::string name);

    static Shader * getShader(std::string vertex, std::string fragment);
    static Mesh * getMesh(std::string name);
    static Image * getImage(std::string name);
    static Texture * getTexture(std::string name);
    static RenderTexture * getRenderTexture(std::string name);
    static Sound * getSound(std::string name);

private:
    static std::map<std::string, Shader *> shaders;
    static std::map<std::string, Mesh *> meshes;
    static std::map<std::string, Image *> images;
    static std::map<std::string, Texture *> textures;
    static std::map<std::string, RenderTexture *> renderTextures;
    static std::map<std::string, Sound *> sounds;

    static const std::string shaderFolder;
    static const std::string meshFolder;
    static const std::string imageFolder;
    static const std::string soundFolder;

    static std::string buildPath(std::string folder, std::string filename);
};

#endif
