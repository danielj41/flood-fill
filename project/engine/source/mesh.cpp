#include "mesh.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "glm/gtc/matrix_transform.hpp"
#include "tiny_obj_loader.h"

glm::vec3 Mesh::INVALID_LIMIT = glm::vec3(0);
std::string Mesh::MANUAL_GENERATED_MESH_NAME = "MANUAL_GENERATED_MESH_NAME";

Mesh::Mesh() {
    DEBUG("Empty Mesh!");
}

Mesh::Mesh(std::string _objfile)
    : objfile(_objfile), loaded(false), normalsFlag(false),
         max(INVALID_LIMIT), min(INVALID_LIMIT) {
    ASSERT(_objfile != MANUAL_GENERATED_MESH_NAME, "This filename is reserved!");
}

Mesh::Mesh(std::vector<unsigned int> _indices, std::vector<float> _vertices){
    objfile = MANUAL_GENERATED_MESH_NAME;
    indices = _indices;
    vertices = _vertices;
    textureCoordinates.clear();
    tangents.clear();
    loaded = true;
}

Mesh::Mesh(std::vector<unsigned int> _indices, std::vector<float> _vertices,
           std::vector<float> _texcoords){
    objfile = MANUAL_GENERATED_MESH_NAME;
    indices = _indices;
    vertices = _vertices;
    textureCoordinates = _texcoords;
    tangents.clear();
    loaded = true;
}

void Mesh::resize(){
    ASSERT(isLoaded(), "OBJ " << objfile << "not loaded");

    INFO("Resizing OBJ " << objfile << "...");

    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    float scaleX, scaleY, scaleZ;
    float shiftX, shiftY, shiftZ;

    minX = minY = minZ = 1.1754E+38F;
    maxX = maxY = maxZ = -1.1754E+38F;

    //Go through all vertices to determine min and max of each dimension
    for (size_t v = 0; v < vertices.size() / 3; v++) {
        if (vertices[3 * v + 0] < minX) minX = vertices[3 * v + 0];
        if (vertices[3 * v + 0] > maxX) maxX = vertices[3 * v + 0];

        if (vertices[3 * v + 1] < minY) minY = vertices[3 * v + 1];
        if (vertices[3 * v + 1] > maxY) maxY = vertices[3 * v + 1];

        if (vertices[3 * v + 2] < minZ) minZ = vertices[3 * v + 2];
        if (vertices[3 * v + 2] > maxZ) maxZ = vertices[3 * v + 2];
    }

    //From min and max compute necessary scale and shift for each dimension
    float maxExtent, xExtent, yExtent, zExtent;
    xExtent = maxX - minX;
    yExtent = maxY - minY;
    zExtent = maxZ - minZ;
    if (xExtent >= yExtent && xExtent >= zExtent) {
        maxExtent = xExtent;
    }
    if (yExtent >= xExtent && yExtent >= zExtent) {
        maxExtent = yExtent;
    }
    if (zExtent >= xExtent && zExtent >= yExtent) {
        maxExtent = zExtent;
    }
    scaleX = 2.0 / maxExtent;
    shiftX = minX + (xExtent / 2.0);
    scaleY = 2.0 / maxExtent;
    shiftY = minY + (yExtent / 2.0);
    scaleZ = 2.0 / maxExtent;
    shiftZ = minZ + (zExtent) / 2.0;

    //Go through all vertices shift and scale them
    for (size_t v = 0; v < vertices.size() / 3; v++) {
        vertices[3 * v + 0] = (vertices[3 * v + 0] - shiftX) * scaleX;
        vertices[3 * v + 1] = (vertices[3 * v + 1] - shiftY) * scaleY;
        vertices[3 * v + 2] = (vertices[3 * v + 2] - shiftZ) * scaleZ;
    }
}

void Mesh::load(){
    ASSERT(objfile != MANUAL_GENERATED_MESH_NAME,
           "This mesh can't be loaded! It is manual generated!");
    if(!isLoaded()){
        INFO("Loading OBJ " << objfile << "...");

        std::vector<tinyobj::material_t> materials;
        std::vector<tinyobj::shape_t> shape;
        std::string err = tinyobj::LoadObj(shape, materials, objfile.c_str());

        vertices = shape[0].mesh.positions;
        indices = shape[0].mesh.indices;
        textureCoordinates = shape[0].mesh.texcoords;
        objNormals = shape[0].mesh.normals;

        ASSERT(err.empty(), err);
    }

    INFO("OBJ " << objfile << " loaded correctly!");
    loaded = true;
}

void Mesh::loadObjNormals() {
    ASSERT(isLoaded(), "OBJ " << objfile << "not loaded");
    normals = objNormals;
    normalsFlag = true;
}

void Mesh::calculateNormals(){
    ASSERT(isLoaded(), "OBJ " << objfile << "not loaded");

    INFO("Calculating " << objfile << " normals...");

    normals = std::vector<float>(vertices.size(), 0.0f);

    int idx1, idx2, idx3;
    glm::vec3 v1, v2, v3;

    for (unsigned int i = 0; i < indices.size() / 3; i++) {
        idx1 = indices[3 * i + 0];
        idx2 = indices[3 * i + 1];
        idx3 = indices[3 * i + 2];
        v1 = glm::vec3(vertices[3 * idx1 + 0],
                       vertices[3 * idx1 + 1],
                       vertices[3 * idx1 + 2]);
        v2 = glm::vec3(vertices[3 * idx2 + 0],
                       vertices[3 * idx2 + 1],
                       vertices[3 * idx2 + 2]);
        v3 = glm::vec3(vertices[3 * idx3 + 0],
                       vertices[3 * idx3 + 1],
                       vertices[3 * idx3 + 2]);

        glm::vec3 norm = glm::cross((v2 - v1), (v3 - v1));

        normals[3 * idx1 + 0] += norm.x;
        normals[3 * idx1 + 1] += norm.y;
        normals[3 * idx1 + 2] += norm.z;

        normals[3 * idx2 + 0] += norm.x;
        normals[3 * idx2 + 1] += norm.y;
        normals[3 * idx2 + 2] += norm.z;

        normals[3 * idx3 + 0] += norm.x;
        normals[3 * idx3 + 1] += norm.y;
        normals[3 * idx3 + 2] += norm.z;
    }

    for (unsigned int i = 0; i < normals.size() / 3; i++) {
        glm::vec3 norm = glm::normalize(glm::vec3(normals[3 * i + 0],
                                                  normals[3 * i + 1],
                                                  normals[3 * i + 2]));
        normals[3 * i + 0] = norm.x;
        normals[3 * i + 1] = norm.y;
        normals[3 * i + 2] = norm.z;

    }

    normalsFlag = true;
}

void Mesh::calculateTangents(){
    //Reference: http://www.terathon.com/code/tangent.html

    ASSERT(isLoaded(), "OBJ " << objfile << "not loaded");
    ASSERT(hasNormals(), objfile << " normals were not calculated yet!");
    ASSERT(hasTextureCoordinates(), "this mesh does not have" <<
                                           " texture coordinates");

    tangents = std::vector<float>(4*vertices.size()/3, 0.0f);
    std::vector<float> bitangents(vertices.size(), 0.0f);

    int idx1, idx2, idx3;
    glm::vec3 v1, v2, v3;
    glm::vec2 uv1, uv2, uv3;

    for (unsigned int i = 0; i < indices.size() / 3; i++) {
        idx1 = indices[3 * i + 0];
        idx2 = indices[3 * i + 1];
        idx3 = indices[3 * i + 2];
        v1 = glm::vec3(vertices[3 * idx1 + 0],
                       vertices[3 * idx1 + 1],
                       vertices[3 * idx1 + 2]);
        v2 = glm::vec3(vertices[3 * idx2 + 0],
                       vertices[3 * idx2 + 1],
                       vertices[3 * idx2 + 2]);
        v3 = glm::vec3(vertices[3 * idx3 + 0],
                       vertices[3 * idx3 + 1],
                       vertices[3 * idx3 + 2]);

        uv1 = glm::vec2(textureCoordinates[2 * idx1 + 0],
                        textureCoordinates[2 * idx1 + 1]);
        uv2 = glm::vec2(textureCoordinates[2 * idx2 + 0],
                        textureCoordinates[2 * idx2 + 1]);
        uv3 = glm::vec2(textureCoordinates[2 * idx3 + 0],
                        textureCoordinates[2 * idx3 + 1]);

        glm::vec2 ST1 = uv2 - uv1;
        glm::vec2 ST2 = uv3 - uv1;

        float f = 1.0f/(ST1.x * ST2.y - ST2.x * ST1.y);

        glm::vec3 tangent   = f * (ST2.y * (v2 - v1) - ST1.y * (v3 - v1));
        glm::vec3 bitangent = f * (ST1.x * (v2 - v1) - ST2.x * (v3 - v1));

        tangents[4 * idx1 + 0] += tangent.x;
        tangents[4 * idx1 + 1] += tangent.y;
        tangents[4 * idx1 + 2] += tangent.z;

        tangents[4 * idx2 + 0] += tangent.x;
        tangents[4 * idx2 + 1] += tangent.y;
        tangents[4 * idx2 + 2] += tangent.z;

        tangents[4 * idx3 + 0] += tangent.x;
        tangents[4 * idx3 + 1] += tangent.y;
        tangents[4 * idx3 + 2] += tangent.z;

        bitangents[3 * idx1 + 0] += bitangent.x;
        bitangents[3 * idx1 + 1] += bitangent.y;
        bitangents[3 * idx1 + 2] += bitangent.z;

        bitangents[3 * idx2 + 0] += bitangent.x;
        bitangents[3 * idx2 + 1] += bitangent.y;
        bitangents[3 * idx2 + 2] += bitangent.z;

        bitangents[3 * idx3 + 0] += bitangent.x;
        bitangents[3 * idx3 + 1] += bitangent.y;
        bitangents[3 * idx3 + 2] += bitangent.z;
    }

    for (unsigned int i = 0; i < tangents.size() / 4; i++) {
        glm::vec3 tangent = glm::vec3(tangents[4 * i + 0],
                                      tangents[4 * i + 1],
                                      tangents[4 * i + 2]);
        glm::vec3 bitangent = glm::vec3(bitangents[3 * i + 0],
                                        bitangents[3 * i + 1],
                                        bitangents[3 * i + 2]);
        glm::vec3 normal = glm::vec3(normals[3 * i + 0],
                                     normals[3 * i + 1],
                                     normals[3 * i + 2]);

        // Gram-Schmidt orthogonalization
        glm::vec3 t_ortho = tangent - glm::dot(tangent, normal)*normal;
        glm::vec3 b_ortho = bitangent - glm::dot(normal, bitangent)*normal
                            - glm::dot(t_ortho, bitangent)*t_ortho/(tangent*tangent);

        t_ortho = normalize(t_ortho);
        b_ortho = normalize(b_ortho);

        glm::mat3 TBN_ortho = glm::transpose(glm::mat3(t_ortho, b_ortho, normal));

        float handedness = glm::determinant(TBN_ortho);
        if(handedness < 0) handedness = -1.0f;
        else handedness = 1.0f;

        tangents[4 * i + 0] = t_ortho.x;
        tangents[4 * i + 1] = t_ortho.y;
        tangents[4 * i + 2] = t_ortho.z;
        tangents[4 * i + 3] = handedness;
    }
}

void Mesh::calculateLimits(){
    INFO("Calculating mesh " << objfile << " limits...");

    float minX, minY, minZ;
    float maxX, maxY, maxZ;

    minX = minY = minZ = 1.1754E+38F;
    maxX = maxY = maxZ = -1.1754E+38F;

    for (size_t v = 0; v < vertices.size() / 3; v++) {
        if (vertices[3 * v + 0] < minX) minX = vertices[3 * v + 0];
        if (vertices[3 * v + 0] > maxX) maxX = vertices[3 * v + 0];

        if (vertices[3 * v + 1] < minY) minY = vertices[3 * v + 1];
        if (vertices[3 * v + 1] > maxY) maxY = vertices[3 * v + 1];

        if (vertices[3 * v + 2] < minZ) minZ = vertices[3 * v + 2];
        if (vertices[3 * v + 2] > maxZ) maxZ = vertices[3 * v + 2];
    }

    min = glm::vec3(minX, minY, minZ);
    max = glm::vec3(maxX, maxY, maxZ);

    INFO("Mesh " << objfile << " Limits:");
    INFO("   Max: (" << maxX << ", " << maxY << ", " << maxZ<< ")");
    INFO("   Min: (" << minX << ", " << minY << ", " << minZ<< ")");
}

glm::vec3 Mesh::getMaxLimits(){
    ASSERT(max != glm::vec3(0) || min != glm::vec3(0),
        "You dind't calculate the limits of mesh " << objfile << ". "
        "Call calclulateLimits() to be able to get Max and Min");
    return max;
}

glm::vec3 Mesh::getMinLimits(){
    ASSERT(max != glm::vec3(0) || min != glm::vec3(0),
        "You dind't calculate the limits of mesh " << objfile << ". "
        "Call calclulateLimits() to be able to get Max and Min");
    return min;
}

std::string Mesh::getFileName(){
    return objfile;
}

std::vector<float> Mesh::getNormals(){
    ASSERT(isLoaded(), "OBJ " << objfile << "not loaded");
    ASSERT(hasNormals(), objfile << " normals were not calculated yet!");

    return normals;
}

std::vector<float> Mesh::getVertices(){
    ASSERT(isLoaded(), "OBJ " << objfile << "not loaded");
    ASSERT(vertices.size(), "This mesh does not have vertices");

    return vertices;
}

std::vector<unsigned int> Mesh::getIndices(){
    ASSERT(isLoaded(), "OBJ " << objfile << "not loaded");
    ASSERT(hasIndices(), "This mesh does not have indeces");

    return indices;
}

std::vector<float> Mesh::getTextureCoordinates(){
    ASSERT(isLoaded(), "OBJ " << objfile << "not loaded");
    ASSERT(hasTextureCoordinates(), "this mesh does not have" <<
                                           " texture coordinates");

    return textureCoordinates;
}

std::vector<float> Mesh::getTangents(){
    ASSERT(isLoaded(), "OBJ " << objfile << "not loaded");
    ASSERT(hasTangents(), "this mesh does not have" <<
                         " tangents calculated");
    return tangents;
}

bool Mesh::hasNormals(){
    return normalsFlag;
}

bool Mesh::hasTextureCoordinates(){
    return textureCoordinates.size() > 0;
}

bool Mesh::hasIndices(){
    return indices.size() > 0;
}

bool Mesh::hasTangents(){
    return tangents.size() > 0;
}

bool Mesh::isLoaded(){
    return loaded;
}

void Mesh::generateVertexBuffer(){
    // TODO: Check GLError after binding
    INFO("Generating vertex buffer of mesh " << objfile << "...");

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, getVertices().size()*sizeof(float),
                 &getVertices()[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::generateNormalBuffer(){
    ASSERT(hasNormals(), objfile << " normals were not calculated yet!");
    INFO("Generating normal buffer of mesh " << objfile << "...");

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, getNormals().size()*sizeof(float),
                 &getNormals()[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::generateIndexBuffer(){
    ASSERT(hasIndices(), "This mesh does not have indeces");
    INFO("Generating index buffer of mesh " << objfile << "...");

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndices().size()*sizeof(unsigned int),
                 &getIndices()[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::generateTextureCoordinateBuffer(){
    ASSERT(hasTextureCoordinates(), "this mesh does not have" <<
                                           " texture coordinates");

    INFO("Generating texture coordinates buffer of mesh " << objfile << "...");

    glGenBuffers(1, &textureCoordinateBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoordinateBuffer);
    glBufferData(GL_ARRAY_BUFFER, getTextureCoordinates().size()*sizeof(float),
                 &getTextureCoordinates()[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::generateTangentBuffer(){
    ASSERT(hasTangents(), "this mesh does not have" <<
                          " tangents calculated");

    INFO("Generating tangent buffer of mesh " << objfile << "...");

    glGenBuffers(1, &tangentBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, tangentBuffer);
    glBufferData(GL_ARRAY_BUFFER, getTangents().size()*sizeof(float),
                 &getTangents()[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

GLuint Mesh::getVertexBuffer(){
    return vertexBuffer;
}

GLuint Mesh::getNormalBuffer(){
    return normalBuffer;
}

GLuint Mesh::getIndexBuffer(){
    return indexBuffer;
}

GLuint Mesh::getTextureCoordinateBuffer(){
    return textureCoordinateBuffer;
}

GLuint Mesh::getTangentBuffer(){
    return tangentBuffer;
}
