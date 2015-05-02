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

    void applyTexture(Texture * _texture);
    void enableTexture();
    void disableTexture();
    bool isTextureEnabled();
    bool hasTexture();
    bool isWater();
    void enableWater();
    void applyWaterData(Texture * _texture);
    void applyWaterColor(Texture * _texture);

    void scale(glm::vec3 scalingVector);
    void rotate(float angle, glm::vec3 direction);
    void translate(glm::vec3 translationVector);
    void loadIdentity();
    void setMatrix(glm::mat4 matrix);

protected:
    Mesh *   mesh;
    Material * material;
    Texture * texture;

    Texture * waterData;
    Texture * waterColor;

    glm::mat4 modelMatrix;
    bool _hasTexture, textureEnabled;
    bool water;

    void drawElements();
};

#endif
