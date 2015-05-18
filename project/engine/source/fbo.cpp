#include "fbo.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

//TODO: Check if the HW support FBO and check if the FBO EXT is supported

FBO::FBO() : depth(false), _hasDepthTexture(false), loaded(false) {}

void FBO::addTexture(TexturePtr texture){
    INFO("Adding texture to FBO...");
    if(loaded) DEBUG("You are trying to add a texture to a FBO that was already laoded!");

    textures.push_back(texture);
}

void FBO::addDepthRenderBuffer(unsigned int width, unsigned int height){
    INFO("Adding depth render buffer to FBO...");
    if(loaded) DEBUG("You are trying to add a depth buffer to a FBO that was already laoded!");

    _width = width;
    _height = height;
    depth = true;
}

void FBO::addDepthTexture(unsigned int width, unsigned int height){
    INFO("Adding depth texture to FBO...");
    if(loaded) DEBUG("You are trying to add a depth buffer to a FBO that was already laoded!");

    depthTexture = TexturePtr(new Texture());
    depthTexture->createTexture(width, height, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_FLOAT);
    depthBuf = depthTexture->getTexture();

    _width = width;
    _height = height;
    depth = true;
    _hasDepthTexture = true;
}

void FBO::load(){
    if(loaded) DEBUG("Overriding FBO! This FBO was already loaded once!");

    INFO("Creating FBO...");
    glGenFramebuffersEXT(1, &id);
    ASSERT(id != 0, "Could not create FBO");

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);

    //Color Buffers
    INFO("Attaching Colors to FBO...");
    if(textures.size() > 0){
        GLenum * attchs = new GLenum[textures.size()];

        for (unsigned int i = 0; i < textures.size(); i++){
            glBindTexture(GL_TEXTURE_2D, textures[i]->getTexture());

            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
                                      GL_COLOR_ATTACHMENT0_EXT + i,
                                      GL_TEXTURE_2D, textures[i]->getTexture(),
                                      0);
            attchs[i] = GL_COLOR_ATTACHMENT0_EXT + i;
        }

        glDrawBuffers(textures.size(), attchs);
        delete attchs;

        INFO(textures.size() << " colors were attached!");
    }
    else{
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        INFO("There is no color to attach!");
    }

    //Depth Buffer
    if(depth && _hasDepthTexture){
        INFO("Attaching Depth Texture...");

        glBindTexture(GL_TEXTURE_2D, depthBuf);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                                  GL_TEXTURE_2D, depthBuf, 0);
    }
    else if(depth){
        INFO("Attaching Depth Render Buffer...");

        glGenRenderbuffersEXT(1, &depthBuf);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuf);

        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT,
                                 _width, _height);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,
                                     GL_RENDERBUFFER_EXT, depthBuf);
    }

    GLenum e = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
    ASSERT(e == GL_FRAMEBUFFER_COMPLETE_EXT, "Problem in FBO creation. " << e);
    printInfo();

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    loaded = true;
}

TexturePtr FBO::getTexture(unsigned int texID){
    ASSERT(loaded, "FBO not loaded! Can't get texture");
    ASSERT(texID < textures.size(),
            "Inexisting Texture " << texID << " in the FBO!");
    return textures[texID];
}

TexturePtr FBO::getDepthTexture(){
    ASSERT(loaded, "FBO not loaded! Can't get texture");
    ASSERT(_hasDepthTexture, "FBO does not have depth texture!");

    return depthTexture;
}

void FBO::enable(){
    ASSERT(loaded, "FBO not loaded! Can't get texture");

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void FBO::disable(){
    ASSERT(loaded, "FBO not loaded! Can't get texture");

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void FBO::printInfo() {
    //Code modified from:
    //      http://www.lighthouse3d.com/tutorials/glsl-core-tutorial/fragment-shader/

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);

    int res;

    GLint buffer;
    int i = 0;
    do {
        glGetIntegerv(GL_DRAW_BUFFER0 + i, &buffer);

        if (buffer != GL_NONE) {
            INFO("Shader Output Location " << i << " - color attachment " << buffer - GL_COLOR_ATTACHMENT0_EXT);

            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER_EXT, buffer, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &res);
            INFO("\tAttachment Type: " << (res == GL_TEXTURE ? "Texture" : "Render Buffer"));
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER_EXT, buffer, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &res);
            INFO("\tAttachment object name: " << res);
        }
        ++i;

    } while (buffer != GL_NONE);
}
