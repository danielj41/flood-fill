#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <map>

#include "GLIncludes.h"
#include "image.hpp"

#include "pointer_macros.h"
DEF_PTR(Texture, TexturePtr);

class Texture{
public:
    Texture();

    void createTexture(GLsizei width, GLsizei height, GLint internalformat,
                        GLenum format, GLenum type, bool mipmap = false);
    void createTextureFromImage(ImagePtr image, bool mipmap = false);

    GLuint getTexture();

private:
    GLuint texture;

    bool created;
};

#endif
