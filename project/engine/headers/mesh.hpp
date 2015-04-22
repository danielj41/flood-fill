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

    //Calculates the Maximun and Minimun Limits
    void calculateLimits();
    //Returns the (max.x, max.y, max.z) Limits of this mesh
    glm::vec3 getMaxLimits();
    //Returns the (min.x, min.y, min.z) Limits of this mesh
    glm::vec3 getMinLimits();

    std::vector<float> getNormals();
    std::vector<float> getVertices();
    std::vector<float> getTextureCoordinates();
    std::vector<unsigned int> getIndices();
    std::string getFileName();

    bool hasNormals();
    bool hasTextureCoordinates();
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
    static glm::vec3 INVALID_LIMIT;

    std::string objfile;
    bool  loaded, normalsFlag;
    std::vector<float> normals;
    std::vector<tinyobj::shape_t> shape;

    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint indexBuffer;
    GLuint textureCoordinateBuffer;

    glm::vec3 max, min;
};

#endif
