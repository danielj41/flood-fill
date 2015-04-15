#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

/**

Bounding Box Mapping:
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

    void update(glm::mat4 _modelMatrix);

    glm::vec3 getMax();
    glm::vec3 getMin();
    glm::vec3 getVertex(int index);

    int getCollisionID();
    int getCollideWithID();

    void setCollisionID(int ID);
    void setCollideWithID(int ID);

private:
    static const int NUMBER_VERTICES;

    int collisionID, collideWithID;
    glm::vec3 max, min;
    glm::vec3 vertices[8];
    glm::mat4 modelMatrix;
};

#endif
