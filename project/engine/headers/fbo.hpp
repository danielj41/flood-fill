#ifndef FBO_HPP
#define FBO_HPP

#include <vector>

#include "GLIncludes.h"
#include "texture.hpp"

class FBO{
public:
    FBO();

    // Add a 2D texture to the FBO. Must be done before the load()
    void addTexture(Texture * texture);

    // Add a depth buffer as a Renderer Buffer
    void addDepthBuffer(unsigned int width, unsigned int height);

    // Load the FBO with all textures attached in the same order that they were
    // added to it.
    void load();

    Texture * getTexture(unsigned int texID);

    void enable();
    void disable();

private:
    bool depth, loaded;

    std::vector<Texture *> textures;
    GLuint id, depthBuf;

    void printInfo();
};

#endif
