#ifndef FBO_HPP
#define FBO_HPP

#include <vector>

#include "GLIncludes.h"
#include "texture.hpp"

class FBO{
public:
    FBO();
    FBO(int _width, int _height, int _nTextures, bool _depth);

    void load();

    void addTexture(Texture * texture);
    void addDepthBuffer();

    GLuint getTexture(int texID);

    void enable();
    void disable();

private:
    int nTextures, width, height;
    bool depth;

    GLuint id, * textures, depthBuf;

    int addTexture(int textureID);
    void createDepth();
    void printInfo();
};

#endif
