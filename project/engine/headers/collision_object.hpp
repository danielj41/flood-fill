#ifndef COLLISION_OBJECT_HPP
#define COLLISION_OBJECT_HPP

#include "glm/glm.hpp"
#include "bounding_box.hpp"

class CollisionObject{

public:
    CollisionObject();
    CollisionObject(glm::vec3 _position);

    bool canCollide();
    
    int getCollisionID();
    int getCollideWithID();

    void setCollisionID(int ID);
    void setCollideWithID(int ID);

    glm::vec3 getPosition();
    void setPosition(glm::vec3 _position);
    void setCanCollide(bool canCollide);

    virtual void collided(CollisionObject * collidedWith) = 0;

    bool checkCollision(CollisionObject *other);
    glm::vec3 getCollisionNormal(CollisionObject *other);
    float getCollisionDistance(CollisionObject *other);
    void setBoundingBox(BoundingBox _boundingBox);
    BoundingBox* getBoundingBox();

private:
    
    glm::vec3 position;
    bool _canCollide;
    int collisionID, collideWithID;
    BoundingBox boundingBox;

};

#endif
