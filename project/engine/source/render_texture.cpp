#include "render_texture.hpp"
#include "debug_macros.h"
#include "load_manager.hpp"
#include "shader.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"



RenderTexture::RenderTexture() : loaded(false){}

void RenderTexture::loadShaders() {
    LoadManager::loadShader("render-texture-vertex-data-initial.glsl", "render-texture-fragment-data-initial.glsl");
    Shader *shader = LoadManager::getShader("render-texture-vertex-data-initial.glsl", "render-texture-fragment-data-initial.glsl");
    shader->loadHandle("aNormal", 'a');
    shader->loadHandle("aPosition", 'a');
    shader->loadHandle("aTexCoord", 'a');
    shader->loadHandle("uPrevTexture", 'u');
    shader->loadHandle("uDataTexture", 'u');
    shader->loadHandle("uSize", 'u');
    shader->loadHandle("uDTime", 'u');
    shader->loadHandle("uStartPosition", 'u');

    LoadManager::loadShader("render-texture-vertex-color-initial.glsl", "render-texture-fragment-color-initial.glsl");
    shader = LoadManager::getShader("render-texture-vertex-color-initial.glsl", "render-texture-fragment-color-initial.glsl");
    shader->loadHandle("aNormal", 'a');
    shader->loadHandle("aPosition", 'a');
    shader->loadHandle("aTexCoord", 'a');
    shader->loadHandle("uPrevTexture", 'u');
    shader->loadHandle("uDataTexture", 'u');
    shader->loadHandle("uSize", 'u');
    shader->loadHandle("uDTime", 'u');
    shader->loadHandle("uStartPosition", 'u');

    LoadManager::loadShader("render-texture-vertex-data-update.glsl", "render-texture-fragment-data-update.glsl");
    shader = LoadManager::getShader("render-texture-vertex-data-update.glsl", "render-texture-fragment-data-update.glsl");
    shader->loadHandle("aNormal", 'a');
    shader->loadHandle("aPosition", 'a');
    shader->loadHandle("aTexCoord", 'a');
    shader->loadHandle("uPrevTexture", 'u');
    shader->loadHandle("uDataTexture", 'u');
    shader->loadHandle("uSize", 'u');
    shader->loadHandle("uDTime", 'u');
    shader->loadHandle("uStartPosition", 'u');

    LoadManager::loadShader("render-texture-vertex-block.glsl", "render-texture-fragment-block.glsl");
    shader = LoadManager::getShader("render-texture-vertex-block.glsl", "render-texture-fragment-block.glsl");
    shader->loadHandle("aNormal", 'a');
    shader->loadHandle("aPosition", 'a');
    shader->loadHandle("aTexCoord", 'a');
    shader->loadHandle("uModelMatrix", 'u');
}

void RenderTexture::load(){

    currentTexture = 0;
    int i = 0;

    for(i = 0; i < 2; i++) {
        glGenTextures(1, &texture[i]);
        glBindTexture(GL_TEXTURE_2D, texture[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 512, 512, 0, GL_BGRA, GL_UNSIGNED_BYTE, NULL);

        glGenFramebuffersEXT(1, &framebuffer[i]);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer[i]);
        glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, texture[i], 0);

        glGenRenderbuffersEXT(1, &renderbuffer[i]);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderbuffer[i]);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, 512, 512);

        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, renderbuffer[i]);

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer[i]);
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    }
    
    loaded = true;
    inUse = false;
}

void RenderTexture::clear() {
    int i;
    for(i = 0; i < 2; i++) {
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer[i]);
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    }

    inUse = true;
}

void RenderTexture::render(Shader *shader, GLuint dataTexture,
 float dTime, glm::vec3 startPosition, glm::vec3 size) {
    ASSERT(loaded, "You didn't load the Texture");

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer[currentTexture]);
    glViewport(0, 0, 512, 512);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable (GL_BLEND);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



    glUseProgram(shader->getID());

    Mesh *mesh = LoadManager::getMesh("plane.obj");

    glEnableVertexAttribArray(shader->getHandle("aPosition"));
    glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
    glVertexAttribPointer(shader->getHandle("aPosition"), 3,
                          GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(shader->getHandle("aNormal"));
    glBindBuffer(GL_ARRAY_BUFFER, mesh->getNormalBuffer());
    glVertexAttribPointer(shader->getHandle("aNormal"), 3,
                          GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP); 

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture[(currentTexture + 1) % 2]);
    glUniform1i(shader->getHandle("uPrevTexture"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, dataTexture);
    glUniform1i(shader->getHandle("uDataTexture"), 1);

    glUniform1f(shader->getHandle("uDTime"), dTime);
    glUniform3f(shader->getHandle("uStartPosition"), startPosition.x, startPosition.y, startPosition.z);
    glUniform3f(shader->getHandle("uSize"), size.x, size.y, size.z);

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
}

void RenderTexture::swapTextures() {
    currentTexture = (currentTexture + 1) % 2;
}

void RenderTexture::renderBlock(Uniform3DGrid<int> *grid,
                                float minX, float maxX,
                                float minY, float maxY,
                                float minZ, float maxZ) {

    ASSERT(loaded, "You didn't load the Texture");

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer[currentTexture]);
    glClear(GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, 512, 512);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    Shader *shader = LoadManager::getShader("render-texture-vertex-block.glsl", "render-texture-fragment-block.glsl");

    glUseProgram(shader->getID());

    Mesh *mesh = LoadManager::getMesh("plane.obj");

    glEnableVertexAttribArray(shader->getHandle("aPosition"));
    glBindBuffer(GL_ARRAY_BUFFER, mesh->getVertexBuffer());
    glVertexAttribPointer(shader->getHandle("aPosition"), 3,
                          GL_FLOAT, GL_FALSE, 0, 0);

    glEnableVertexAttribArray(shader->getHandle("aNormal"));
    glBindBuffer(GL_ARRAY_BUFFER, mesh->getNormalBuffer());
    glVertexAttribPointer(shader->getHandle("aNormal"), 3,
                          GL_FLOAT, GL_FALSE, 0, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->getIndexBuffer());

    // y and z are intentionally flipped, to map the XZ grid into an XY projection to draw on the texture
    glm::mat4 scale = glm::scale(glm::mat4(1.0f),
        glm::vec3(grid->getEdgeSizeX() / (maxX - minX + grid->getEdgeSizeX()),
                  grid->getEdgeSizeZ() / (maxZ - minZ + grid->getEdgeSizeZ()),
                  1.0f));

    float x, y, z;
    for(x = minX; x < maxX + grid->getEdgeSizeX()/2.0f; x += grid->getEdgeSizeX()) {
        for(z = minZ; z < maxZ + grid->getEdgeSizeZ()/2.0f; z += grid->getEdgeSizeZ()) {
            for(y = maxY + grid->getEdgeSizeY(); y > minY + grid->getEdgeSizeY()/2.0f && // stop early, so we can query one block down
                          grid->getValue(x, y - grid->getEdgeSizeY(), z) == 1;
                y -= grid->getEdgeSizeY());

            glm::mat4 model = glm::translate(glm::mat4(1.0f),
                glm::vec3((x - (maxX + minX) / 2.0f) / (maxX - minX + grid->getEdgeSizeX()) * 2.0f,
                          (z - (maxZ + minZ) / 2.0f) / (maxZ - minZ + grid->getEdgeSizeZ()) * 2.0f,
                          (y - grid->getEdgeSizeY() / 2.0f - (maxY + minY) / 2.0f) / (maxY - minY + grid->getEdgeSizeY()) * 2.0f
                          )) * scale;

            glUniformMatrix4fv(shader->getHandle("uModelMatrix"), 1, GL_FALSE,
                        glm::value_ptr(model));

            glDrawElements(GL_TRIANGLES, (int) mesh->getIndices().size(), GL_UNSIGNED_INT, 0);
        }
    }

    glDisableVertexAttribArray(shader->getHandle("aPosition"));
    glDisableVertexAttribArray(shader->getHandle("aNormal"));
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}

void RenderTexture::release() {
    inUse = false;
}

bool RenderTexture::isInUse() {
    return inUse;
}

GLuint RenderTexture::getTexture(){
    ASSERT(loaded, "You didn't load the Texture");

    return texture[currentTexture];
}

