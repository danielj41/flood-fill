#ifndef OBJECT_H
#define OBJECT_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "mesh.hpp"
#include "material.hpp"
#include "shader.hpp"

class Object {

public:
    Object();
    Object(Mesh * _mesh, Material * _material);

    virtual void draw(Shader * shader);

    Mesh * getMesh();
    Material * getMaterial();
    glm::mat4 getModelMatrix();

    void scale(glm::vec3 scalingVector);
    void rotate(float angle, glm::vec3 direction);
    void translate(glm::vec3 translationVector);
    void loadIdentity();
    void setMatrix(glm::mat4 matrix);

protected:
    Mesh *   mesh;
    Material * material;

    glm::mat4 modelMatrix;

    void drawElements();
};

#endif
