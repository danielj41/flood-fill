#include "texture.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

Texture::Texture() : created(false){}

void Texture::createTextureFromImage(Image * image, bool mipmap){
    INFO("Creating Texture From Image...");
    if(created) DEBUG("Overriding Texture!");

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    GLint mode;
    if(image->isPNG()) mode = GL_RGBA;
    else if(image->isBMP()) mode = GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, mode, image->getWidth(),
        image->getHeight(), 0, mode, GL_UNSIGNED_BYTE,
        image->getImage());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if(mipmap){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    created = true;

    INFO("Texture loaded!");
}

void Texture::createTexture(GLsizei width, GLsizei height, GLint internalformat,
                            GLenum format, GLenum type, bool mipmap = false){
    INFO("Creating Texture...");
    if(created) DEBUG("Overriding Texture!");

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);


    glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    if(mipmap){
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    glBindTexture(GL_TEXTURE_2D, 0);

    created = true;

    INFO("Texture loaded!");


}

GLuint Texture::getTexture(){
    ASSERT(created, "You didn't load the Texture");

    return texture;
}

