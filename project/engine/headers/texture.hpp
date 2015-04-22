#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <string>
#include <map>

#include "image.hpp"

class Texture{
public:
    Texture();
    Texture(Image * _image);

    void load();

    GLuint getTexture();

private:
    Image * image;
    GLuint texture;

    bool loaded;
};

#endif
