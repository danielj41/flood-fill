#include "fbo.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

FBO::FBO(){}

FBO::FBO(int _width, int _height, int _nTextures, bool _depth)
    : nTextures(_nTextures), depth(_depth), width(_width), height(_height){}

void FBO::init(){
    textures = new GLuint[nTextures];

    glGenFramebuffers(1, &id);
    ASSERT(id != 0, "Could not create FBO");

    glBindFramebuffer(GL_FRAMEBUFFER, id);

    for (int i = 0; i < nTextures; i++){
        addTexture(i);
    }

    if (depth){
        createDepth();
    }

    GLenum * attchs = new GLenum[nTextures];
    for (int i = 0; i < nTextures; i++){
        attchs[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    glDrawBuffers(nTextures, attchs);

    glBindTexture(GL_TEXTURE_2D, 0);

    GLenum e = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    ASSERT(e != GL_FRAMEBUFFER_COMPLETE, "Problem in FBO creation");
    printInfo();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    delete attachs;
}

int FBO::addTexture(int textureID){
    glGenTextures(1, &textures[textureID]);
    glBindTexture(GL_TEXTURE_2D, textures[textureID]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGB, GL_FLOAT, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + textureID, textures[textureID], 0);

    return textureID;
}

void FBO::createDepth(){
    glGenRenderbuffers(1, &depthBuf);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBuf);

    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuf);

    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

GLuint FBO::getTexture(int texID){
    ASSERT(texID < nTextures && texID >= 0, "Inexisting Texture " << texID);
    return textures[texID];
}

void FBO::enable(){
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, id);
}

void FBO::disable(){
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
            INFO("\tAttachment Type: " << res == GL_TEXTURE ? "Texture" : "Render Buffer");
            glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, buffer, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &res);
            INFO("\tAttachment object name: " << res);
        }
        ++i;

    } while (buffer != GL_NONE);
}
