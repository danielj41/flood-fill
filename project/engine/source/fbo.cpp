#include "fbo.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

//TODO: Check if the HW support FBO and check if the FBO EXT is supported

#define FBO21 // OpenGL 2.1 FBO

FBO::FBO() : depth(false), _hasDepthTexture(false), loaded(false) {}

void FBO::addTexture(Texture * texture){
    if(loaded) DEBUG("You are trying to add a texture to a FBO that was already laoded!");

    textures.push_back(texture);
}

void FBO::addDepthRenderBuffer(unsigned int width, unsigned int height){
    if(loaded) DEBUG("You are trying to add a depth buffer to a FBO that was already laoded!");

#ifdef FBO21
    glGenRenderbuffersEXT(1, &depthBuf);
    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, depthBuf);

    glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, depthBuf);

    glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
#else
    // OpenGL 3.1+ FBO
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
#endif

    depth = true;
}

void FBO::addDepthTexture(unsigned int width, unsigned int height){
    if(loaded) DEBUG("You are trying to add a depth buffer to a FBO that was already laoded!");

    depthTexture = new Texture();
    depthTexture->createTexture(width, height, GL_DEPTH_COMPONENT16, GL_DEPTH_COMPONENT, GL_FLOAT);
    depthBuf = depthTexture->getTexture();

    glBindTexture(GL_TEXTURE_2D, depthBuf);

#ifdef FBO21
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, depthBuf, 0);
#else
    // OpenGL 3.1+ FBO
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthBuf, 0);
#endif

    glBindTexture(GL_TEXTURE_2D, 0);
    INFO("1");

    depth = true;
    _hasDepthTexture = true;
    INFO("Depth Texture added to the FBO");
}

void FBO::load(){
    if(loaded) DEBUG("Overriding FBO! This FBO was already loaded once!");

#ifdef FBO21
    glGenFramebuffersEXT(1, &id);
    ASSERT(id != 0, "Could not create FBO");

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);


    if(textures.size() > 0){
        GLenum * attchs = new GLenum[textures.size()];

        for (unsigned int i = 0; i < textures.size(); i++){
            glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT,
                                      GL_COLOR_ATTACHMENT0_EXT + i,
                                      GL_TEXTURE_2D, textures[i]->getTexture(),
                                      0);
            attchs[i] = GL_COLOR_ATTACHMENT0_EXT + i;
        }

        glDrawBuffers(textures.size(), attchs);
        delete attchs;
    }
    else{
        glDrawBuffer(GL_NONE);
    }

    GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);
    ASSERT(e != GL_FRAMEBUFFER_COMPLETE_EXT, "Problem in FBO creation");
    printInfo();

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#else
    // OpenGL 3.1+ FBO
    glGenFramebuffers(1, &id);
    ASSERT(id != 0, "Could not create FBO");

    glBindFramebuffer(GL_FRAMEBUFFER, id);


    if(textures.size() > 0){
        GLenum * attchs = new GLenum[textures.size()];

        for (unsigned int i = 0; i < textures.size(); i++){
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i,
                                 textures[i]->getTexture(), 0);
            attchs[i] = GL_COLOR_ATTACHMENT0 + i;
        }

        glDrawBuffers(textures.size(), attchs);
        delete attchs;
    }
    else{
        glDrawBuffer(GL_NONE);
    }

    GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    ASSERT(e != GL_FRAMEBUFFER_COMPLETE, "Problem in FBO creation");
    printInfo();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif

    loaded = true;
}

Texture * FBO::getTexture(unsigned int texID){
    ASSERT(loaded, "FBO not loaded! Can't get texture");
    ASSERT(texID < textures.size(),
            "Inexisting Texture " << texID << " in the FBO!");
    return textures[texID];
}

Texture * FBO::getDepthTexture(){
    ASSERT(loaded, "FBO not loaded! Can't get texture");
    ASSERT(_hasDepthTexture, "FBO does not have depth texture!");

    return depthTexture;
}

void FBO::enable(){
    ASSERT(loaded, "FBO not loaded! Can't get texture");

    glBindTexture(GL_TEXTURE_2D, 0);

#ifdef FBO21
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, id);
#else
    glBindFramebuffer(GL_FRAMEBUFFER, id);
#endif
}

void FBO::disable(){
    ASSERT(loaded, "FBO not loaded! Can't get texture");

#ifdef FBO21
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
#else
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
#endif
}

void FBO::printInfo() {
    //Code modified from:
    //      http://www.lighthouse3d.com/tutorials/glsl-core-tutorial/fragment-shader/

    glBindFramebuffer(GL_FRAMEBUFFER, id);

    int res;

    GLint buffer;
    int i = 0;
    do {
        glGetIntegerv(GL_DRAW_BUFFER0 + i, &buffer);

        if (buffer != GL_NONE) {
            INFO("Shader Output Location " << i << " - color attachment " << buffer - GL_COLOR_ATTACHMENT0);

            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, buffer, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &res);
            INFO("\tAttachment Type: " << (res == GL_TEXTURE ? "Texture" : "Render Buffer"));
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, buffer, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &res);
            INFO("\tAttachment object name: " << res);
        }
        ++i;

    } while (buffer != GL_NONE);
}
