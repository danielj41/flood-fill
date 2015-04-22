#ifndef LOAD_MANAGER_H
#define LOAD_MANAGER

#include <string>
#include <map>

#include "shader.hpp"
#include "mesh.hpp"
#include "image.hpp"
#include "texture.hpp"

// TODO: create an unload operation to delete the pointers created

class LoadManager {
public:
    static void loadShader(std::string vertex, std::string fragment);
    static void loadMesh(std::string filename);
    static void loadImage(std::string filename);
    static void loadTexture(std::string name, Image * image);

    static Shader * getShader(std::string vertex, std::string fragment);
    static Mesh * getMesh(std::string name);
    static Image * getImage(std::string name);
    static Texture * getTexture(std::string name);

private:
    static std::map<std::string, Shader *> shaders;
    static std::map<std::string, Mesh *> meshes;
    static std::map<std::string, Image *> images;
    static std::map<std::string, Texture *> textures;

    static const std::string shaderFolder;
    static const std::string meshFolder;
    static const std::string imageFolder;

    static std::string buildPath(std::string folder, std::string filename);
};

#endif
