#ifndef RENDER_TEXTURE_HPP
#define RENDER_TEXTURE_HPP

#include <string>
#include <map>

#include "image.hpp"
#include "shader.hpp"
#include "uniform_3d_grid.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "pointer_macros.h"
DEF_PTR(RenderTexture, RenderTexturePtr);

class RenderTexture {
public:
    RenderTexture();

    static void loadShaders();
    void load();
    void render(Shader *shader, GLuint dataTexture,
     glm::vec2 dTime, glm::vec3 startPosition, glm::vec3 size);
    void swapTextures();
    void clear();
    void renderBlock(Uniform3DGrid<int> *grid,
                     float minX, float maxX,
                     float minY, float maxY,
                     float minZ, float maxZ,
                     float extended);

    GLuint getTexture();
    void release();
    bool isInUse();

private:
    GLuint texture[2];
    GLuint framebuffer[2];
    GLuint renderbuffer[2];

    bool inUse;
    int currentTexture;
    bool loaded;
};

#endif
