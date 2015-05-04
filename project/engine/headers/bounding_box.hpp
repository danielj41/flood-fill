#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

/**

Bounding Box Vertex Mapping:
   6--------5
  /|       /|
 / |      / |
2--------1  |
|  |     |  |
|  7-----|--4
| /      | /
|/       |/
3--------0
*/

class BoundingBox{

public:
    BoundingBox();
    BoundingBox(glm::vec3 _max, glm::vec3 _min);

    glm::vec3 getMax();
    glm::vec3 getMin();
    glm::vec3 getVertex(int index);

    //TODO: [Maybe] Extract the behavior of having a model matrix to a different
    //    class? It happens here and in the Object Class
    glm::mat4 getModelMatrix();
    void scale(glm::vec3 scalingVector);
    void rotate(float angle, glm::vec3 direction);
    void translate(glm::vec3 translationVector);
    void loadIdentity();
    void setModelMatrix(glm::mat4 matrix);
    void setPosition(glm::vec3 _position);
    glm::vec3 getPosition();

    //TODO: Remove the GL code inside this function in the creation of the
    //      Render Engine
    void draw();

private:
    static const int NUMBER_VERTICES;

    glm::vec3 max, min;
    glm::vec3 vertices[8];
    glm::mat4 modelMatrix;
    glm::vec3 position;
};

#endif
