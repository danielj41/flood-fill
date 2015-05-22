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
    static void loadTexture(std::string name, ImagePtr image);
    static void loadRenderTexture(std::string name);
    static void loadSound(std::string name);
    static void clearSounds();

    static ShaderPtr getShader(std::string vertex, std::string fragment);
    static MeshPtr getMesh(std::string name);
    static ImagePtr getImage(std::string name);
    static TexturePtr getTexture(std::string name);
    static RenderTexturePtr getRenderTexture(std::string name);
    static SoundPtr getSound(std::string name);

private:
    static std::map<std::string, ShaderPtr> shaders;
    static std::map<std::string, MeshPtr> meshes;
    static std::map<std::string, ImagePtr> images;
    static std::map<std::string, TexturePtr> textures;
    static std::map<std::string, RenderTexturePtr> renderTextures;
    static std::map<std::string, SoundPtr> sounds;

    static const std::string shaderFolder;
    static const std::string meshFolder;
    static const std::string imageFolder;
    static const std::string soundFolder;

    static std::string buildPath(std::string folder, std::string filename);
};

#endif
