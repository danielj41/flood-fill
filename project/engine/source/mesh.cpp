#include "mesh.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "glm/gtc/matrix_transform.hpp"

glm::vec3 Mesh::INVALID_LIMIT = glm::vec3(0);

Mesh::Mesh() {
    DEBUG("Empty Mesh!");
}

Mesh::Mesh(std::string _objfile)
    : objfile(_objfile), loaded(false), normalsFlag(false),
         max(INVALID_LIMIT), min(INVALID_LIMIT) {}

void Mesh::resize(){
    ASSERT(isLoaded(), "OBJ " << objfile << "not loaded");

    INFO("Resizing OBJ " << objfile << "...");

    float minX, minY, minZ;
    float maxX, maxY, maxZ;
    float scaleX, scaleY, scaleZ;
    float shiftX, shiftY, shiftZ;
    float epsilon = 0.001;

    minX = minY = minZ = 1.1754E+38F;
    maxX = maxY = maxZ = -1.1754E+38F;

    //Go through all vertices to determine min and max of each dimension
    for (size_t i = 0; i < shape.size(); i++) {
        for (size_t v = 0; v < shape[i].mesh.positions.size() / 3; v++) {
            if (shape[i].mesh.positions[3 * v + 0] < minX) minX = shape[i].mesh.positions[3 * v + 0];
            if (shape[i].mesh.positions[3 * v + 0] > maxX) maxX = shape[i].mesh.positions[3 * v + 0];

            if (shape[i].mesh.positions[3 * v + 1] < minY) minY = shape[i].mesh.positions[3 * v + 1];
            if (shape[i].mesh.positions[3 * v + 1] > maxY) maxY = shape[i].mesh.positions[3 * v + 1];

            if (shape[i].mesh.positions[3 * v + 2] < minZ) minZ = shape[i].mesh.positions[3 * v + 2];
            if (shape[i].mesh.positions[3 * v + 2] > maxZ) maxZ = shape[i].mesh.positions[3 * v + 2];
        }
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

    //Go through all verticies shift and scale them
    for (size_t i = 0; i < shape.size(); i++) {
        for (size_t v = 0; v < shape[i].mesh.positions.size() / 3; v++) {
            shape[i].mesh.positions[3 * v + 0] = (shape[i].mesh.positions[3 * v + 0] - shiftX) * scaleX;
            shape[i].mesh.positions[3 * v + 1] = (shape[i].mesh.positions[3 * v + 1] - shiftY) * scaleY;
            shape[i].mesh.positions[3 * v + 2] = (shape[i].mesh.positions[3 * v + 2] - shiftZ) * scaleZ;
        }
    }
}

void Mesh::load(){
    if(!isLoaded()){
        INFO("Loading OBJ " << objfile << "...");

        std::vector<tinyobj::material_t> materials;
        std::string err = tinyobj::LoadObj(shape, materials, objfile.c_str());

        ASSERT(err.empty(), err);
    }

    INFO("OBJ " << objfile << " loaded correctly!");
    loaded = true;
}

void Mesh::calculateNormals(){
    ASSERT(isLoaded(), "OBJ " << objfile << "not loaded");

    INFO("Calculating " << objfile << " normals...");

    normals = std::vector<float>(shape[0].mesh.positions.size(), 0);

    int idx1, idx2, idx3;
    glm::vec3 v1, v2, v3;

    for (int i = 0; i < shape[0].mesh.indices.size() / 3; i++) {
        idx1 = shape[0].mesh.indices[3 * i + 0];
        idx2 = shape[0].mesh.indices[3 * i + 1];
        idx3 = shape[0].mesh.indices[3 * i + 2];
        v1 = glm::vec3(shape[0].mesh.positions[3 * idx1 + 0],
                       shape[0].mesh.positions[3 * idx1 + 1],
                       shape[0].mesh.positions[3 * idx1 + 2]);
        v2 = glm::vec3(shape[0].mesh.positions[3 * idx2 + 0],
                       shape[0].mesh.positions[3 * idx2 + 1],
                       shape[0].mesh.positions[3 * idx2 + 2]);
        v3 = glm::vec3(shape[0].mesh.positions[3 * idx3 + 0],
                       shape[0].mesh.positions[3 * idx3 + 1],
                       shape[0].mesh.positions[3 * idx3 + 2]);

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

    for (int i = 0; i < normals.size() / 3; i++) {
        glm::vec3 norm = glm::normalize(glm::vec3(normals[3 * i + 0],
                                                  normals[3 * i + 1],
                                                  normals[3 * i + 2]));
        normals[3 * i + 0] += norm.x;
        normals[3 * i + 1] += norm.y;
        normals[3 * i + 2] += norm.z;
    }

    normalsFlag = true;
}

void Mesh::calculateLimits(){
    INFO("Calculating mesh " << objfile << " limits...");

    float minX, minY, minZ;
    float maxX, maxY, maxZ;

    minX = minY = minZ = 1.1754E+38F;
    maxX = maxY = maxZ = -1.1754E+38F;

    for (size_t v = 0; v < shape[0].mesh.positions.size() / 3; v++) {
        if (shape[0].mesh.positions[3 * v + 0] < minX) minX = shape[0].mesh.positions[3 * v + 0];
        if (shape[0].mesh.positions[3 * v + 0] > maxX) maxX = shape[0].mesh.positions[3 * v + 0];

        if (shape[0].mesh.positions[3 * v + 1] < minY) minY = shape[0].mesh.positions[3 * v + 1];
        if (shape[0].mesh.positions[3 * v + 1] > maxY) maxY = shape[0].mesh.positions[3 * v + 1];

        if (shape[0].mesh.positions[3 * v + 2] < minZ) minZ = shape[0].mesh.positions[3 * v + 2];
        if (shape[0].mesh.positions[3 * v + 2] > maxZ) maxZ = shape[0].mesh.positions[3 * v + 2];
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

    if(!hasNormals()){
        INFO(objfile << " normals were not calculated yet!");
    }
    return normals;
}

std::vector<float> Mesh::getVertices(){
    ASSERT(isLoaded(), "OBJ " << objfile << "not loaded");

    return shape[0].mesh.positions;
}

std::vector<unsigned int> Mesh::getIndices(){
    ASSERT(isLoaded(), "OBJ " << objfile << "not loaded");

    return shape[0].mesh.indices;
}

std::vector<float> Mesh::getTextureCoordinates(){
    ASSERT(isLoaded(), "OBJ " << objfile << "not loaded");

    // TODO: Check if there is tex coord available

    return shape[0].mesh.texcoords;
}

bool Mesh::hasNormals(){
    return normalsFlag;
}

bool Mesh::isLoaded(){
    return loaded;
}

void Mesh::generateVertexBuffer(){
    // TODO: Check if the mesh has vertices, normals, etc before the binding
    // TODO: Check GLError after binding
    INFO("Generating vertex buffer of mesh " << objfile << "...");

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, getVertices().size()*sizeof(float),
                 &getVertices()[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::generateNormalBuffer(){
    INFO("Generating normal buffer of mesh " << objfile << "...");

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, getNormals().size()*sizeof(float),
                 &getNormals()[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Mesh::generateIndexBuffer(){
    INFO("Generating index buffer of mesh " << objfile << "...");

    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, getIndices().size()*sizeof(unsigned int),
                 &getIndices()[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::generateTextureCoordinateBuffer(){
    INFO("Generating texture coordinates buffer of mesh " << objfile << "...");

    glGenBuffers(1, &textureCoordinateBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureCoordinateBuffer);
    glBufferData(GL_ARRAY_BUFFER, getTextureCoordinates().size()*sizeof(float),
                 &getTextureCoordinates()[0], GL_STATIC_DRAW);
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
