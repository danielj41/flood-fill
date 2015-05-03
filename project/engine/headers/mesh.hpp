#ifndef MESH_H
#define MESH_H

#include "vector"
#include "string"

#include "GLIncludes.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

class Mesh {

public:
    Mesh();
    Mesh(std::string _objfile);
    Mesh(std::vector<unsigned int> _indices, std::vector<float> _vertices);
    Mesh(std::vector<unsigned int> _indices, std::vector<float> _vertices, std::vector<float> _texcoords);

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
    bool hasIndices();
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
    static std::string MANUAL_GENERATED_MESH_NAME;

    std::string objfile;
    bool  loaded, normalsFlag;
    std::vector<float> normals;
    std::vector<float> vertices;
    std::vector<float> textureCoordinates;
    std::vector<unsigned int> indices;

    GLuint vertexBuffer;
    GLuint normalBuffer;
    GLuint indexBuffer;
    GLuint textureCoordinateBuffer;

    glm::vec3 max, min;
};

#endif
