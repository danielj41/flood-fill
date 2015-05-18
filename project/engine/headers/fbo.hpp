#ifndef FBO_HPP
#define FBO_HPP

#include <vector>

#include "GLIncludes.h"
#include "texture.hpp"

#include "pointer_macros.h"
DEF_PTR(FBO, FBOPtr);

class FBO{
public:
    FBO();

    // Add a 2D texture to the FBO. Must be done before the load()
    void addTexture(TexturePtr texture);

    // Add a depth buffer as a Render Buffer
    void addDepthRenderBuffer(unsigned int width, unsigned int height);

    // Add a depth buffer as a Texture
    void addDepthTexture(unsigned int width, unsigned int height);

    // Load the FBO with all textures attached in the same order that they were
    // added to it.
    void load();

    TexturePtr getTexture(unsigned int texID);
    TexturePtr getDepthTexture();

    void enable();
    void disable();

private:
    bool depth, _hasDepthTexture, loaded;
    unsigned int _width, _height;

    std::vector<TexturePtr> textures;
    GLuint id, depthBuf;

    TexturePtr depthTexture;

    void printInfo();
};

#endif
