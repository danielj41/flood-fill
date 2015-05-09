#include "texture.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

Texture::Texture() : image(NULL), loaded(false){}
Texture::Texture(Image * _image) : image(_image), loaded(false){}

void Texture::load(){
    INFO("Loading Texture...");
    ASSERT(image != NULL, "There is no image inside this texture!");

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    GLint mode;
    if(image->isPNG()) mode = GL_RGBA;
    else if(image->isBMP()) mode = GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, mode, image->getWidth(),
        image->getHeight(), 0, mode, GL_UNSIGNED_BYTE,
        image->getImage());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    loaded = true;

    INFO("Texture loaded!");
}

GLuint Texture::getTexture(){
    ASSERT(loaded, "You didn't load the Texture");

    return texture;
}

