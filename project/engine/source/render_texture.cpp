#include "render_texture.hpp"
#include "debug_macros.h"
#include "load_manager.hpp"
#include "shader.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

RenderTexture::RenderTexture() : loaded(false){}

void RenderTexture::load(){
    LoadManager::loadShader("render-texture-vertex.glsl", "render-texture-fragment.glsl");
    Shader *shader = LoadManager::getShader("render-texture-vertex.glsl", "render-texture-fragment.glsl");
    shader->loadHandle("aNormal", 'a');
    shader->loadHandle("aPosition", 'a');
    shader->loadHandle("aTexCoord", 'a');
    shader->loadHandle("uPrevTexture", 'u');
    shader->loadHandle("test", 'u');

    currentTexture = 0;
    int i = 0;

    for(i = 0; i < 2; i++) {
        glGenTextures(1, &texture[i]);
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 256, 256, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

        glGenFramebuffersEXT(1, &framebuffer[i]);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer[i]);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture[i], 0);

        glGenRenderbuffersEXT(1, &renderbuffer[i]);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderbuffer[i]);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 256, 256);

        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, renderbuffer[i]);

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer[currentTexture]);
        glClearColor(0.0, 0.5, 0.0, 0.0);
        //glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    }
    
    first = true;
    loaded = true;
}

void RenderTexture::render() {
    ASSERT(loaded, "You didn't load the Texture");

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer[currentTexture]);
    glClearColor(0.5, 0.5, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, 256, 256);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);



    Shader *shader = LoadManager::getShader("render-texture-vertex.glsl", "render-texture-fragment.glsl");
    glUseProgram(shader->getID());

    Mesh *mesh = LoadManager::getMesh("sphere.obj");

    glEnableVertexAttribArray(shader->getHandle("aPosition"));
    glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
    glVertexAttribPointer(shader->getHandle("aPosition"), 3,
                          GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(shader->getHandle("aNormal"));
    glBindBuffer(GL_ARRAY_BUFFER, mesh->getNormalBuffer());
    glVertexAttribPointer(shader->getHandle("aNormal"), 3,
                          GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[(currentTexture + 1) % 2]);
    glUniform1i(shader->getHandle("uPrevTexture"), 0);

    glUniform1i(shader->getHandle("test"), 0);
    if(first) {
        // HACKY TEST CODE, REMOVE THIS
        glUniform1i(shader->getHandle("test"), 10);
        first = false;
    }
    


    glDrawElements(GL_TRIANGLES, (int) mesh->getIndices().size(),
                   GL_UNSIGNED_INT, 0);

    glDisableVertexAttribArray(shader->getHandle("aPosition"));
    glDisableVertexAttribArray(shader->getHandle("aNormal"));
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    currentTexture = (currentTexture + 1) % 2;
}

GLuint RenderTexture::getTexture(){
    ASSERT(loaded, "You didn't load the Texture");

    return texture[(currentTexture + 1) % 2];
}

