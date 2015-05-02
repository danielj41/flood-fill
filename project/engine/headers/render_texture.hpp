#ifndef RENDER_TEXTURE_HPP
#define RENDER_TEXTURE_HPP

#include <string>
#include <map>

#include "image.hpp"

class RenderTexture {
public:
    RenderTexture();

    void load();
    void render();

    GLuint getTexture();

private:
    GLuint texture[2];
    GLuint framebuffer[2];
    GLuint renderbuffer[2];

    int currentTexture;
    bool loaded;
    bool first;
};

#endif
