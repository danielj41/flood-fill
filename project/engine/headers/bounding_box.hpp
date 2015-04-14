#ifndef BOUNDING_BOX_HPP
#define BOUNDING_BOX_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

class BoundingBox{

public:
    BoundingBox();
    BoundingBox(glm::vec3 _max, glm::vec3 _min);

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
};

#endif
