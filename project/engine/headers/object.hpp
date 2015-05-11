#ifndef OBJECT_H
#define OBJECT_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "mesh.hpp"
#include "material.hpp"
#include "shader.hpp"
#include "texture.hpp"

class Object {

public:
    Object();
    Object(Mesh * _mesh, Material * _material);

    virtual void draw(Shader * shader);

    Mesh * getMesh();
    Material * getMaterial();
    glm::mat4 getModelMatrix();
    Texture * getTexture();
    Texture * getNormalMap();
    float getAlpha();
    float getNormalMapScale();
    float getNormalMapBias();
    GLuint getWaterData();
    GLuint getWaterColor();
    GLuint getWaterBlock();

    void applyTexture(Texture * _texture);
    void enableTexture();
    void disableTexture();
    bool isTextureEnabled();
    bool hasTexture();

    void applyNormalMap(Texture * _texture);
    bool hasNormalMap();

    bool isWater();
    void enableWater();
    void applyWaterData(GLuint id);
    void applyWaterColor(GLuint id);
    void applyWaterBlock(GLuint id);

    void setAlpha(float _alpha);
    void scale(glm::vec3 scalingVector);
    void rotate(float angle, glm::vec3 direction);
    void translate(glm::vec3 translationVector);
    void loadIdentity();
    void setMatrix(glm::mat4 matrix);

protected:
    Mesh *   mesh;
    Material * material;

    Texture * texture;
    Texture * normalMap;

    GLuint waterData;
    GLuint waterColor;
    GLuint waterBlock;

    glm::mat4 modelMatrix;
    bool _hasTexture, _hasNormalMap, textureEnabled;
    bool water;
    float alpha;
    float normalMapScale;
    float normalMapBias;

    void drawElements();
};

#endif
