#ifndef OBJECT_H
#define OBJECT_H

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "mesh.hpp"
#include "shader.hpp"

class Object {

public:
    Object();
    Object(Mesh * _mesh, Shader * _shader);

    void draw();

    Mesh *    getMesh();
    Shader *  getShader();
    glm::mat4 getModelMatrix();

    void scale(glm::vec3 scalingVector);
    void rotate(float angle, glm::vec3 direction);
    void translate(glm::vec3 translationVector);
    void loadIdentity();
    void setMatrix(glm::mat4 matrix);

private:
    Mesh *   mesh;
    Shader * shader;

    glm::mat4 modelMatrix;
};

#endif
