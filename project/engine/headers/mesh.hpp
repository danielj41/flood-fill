#ifndef MESH_H
#define MESH_H

#include "vector"
#include "string"

#include "GLIncludes.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"
#include "tiny_obj_loader.h"

class Mesh {

public:
    Mesh();
    Mesh(std::string _objfile);

    void load();
    void resize();
    void calculateNormals();

    std::vector<float> getNormals();
    std::vector<float> getVertices();
    std::vector<float> getTextureCoordinates();
    std::vector<unsigned int> getIndices();
    std::string getFileName();

    bool hasNormals();
    bool isLoaded();

    void generateVertexBuffer();
    void generateNormalBuffer();
    void generateIndexBuffer();
    void generateTextureCoordinateBuffer();

    GLuint getVertexBuffer();
    GLuint getNormalBuffer();
    GLuint getIndexBuffer();
    GLuint getTextureCoordinateBuffer();

private:
    std::string objfile;
    bool normalsFlag, loaded;
    std::vector<float> normals;
    std::vector<tinyobj::shape_t> shape;

    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint indexBuffer;
    GLuint textureCoordinateBuffer;
};

#endif
