#ifndef RENDER_TEXTURE_HPP
#define RENDER_TEXTURE_HPP

#include <string>
#include <map>

#include "image.hpp"
#include "shader.hpp"

class RenderTexture {
public:
    RenderTexture();

    static void loadShaders();
    void load();
    void render(Shader *shader);
    void swapTextures();
    void clear();

    GLuint getTexture();

private:
    GLuint texture[2];
    GLuint framebuffer[2];
    GLuint renderbuffer[2];

    int currentTexture;
    bool loaded;
};

#endif
