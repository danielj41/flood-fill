#ifndef COLLISION_OBJECT_HPP
#define COLLISION_OBJECT_HPP

#include <string>

#include "glm/glm.hpp"
#include "bounding_box.hpp"

#include "pointer_macros.h"
DEF_PTR(CollisionObject, CollisionObjectPtr);

class CollisionObject{

public:
    CollisionObject();
    CollisionObject(glm::vec3 _position);

    bool canCollide();
    bool applyPhysics();
    
    int getCollisionID();
    int getCollideWithID();

    void setCollisionID(int ID);
    void setCollideWithID(int ID);

    glm::vec3 getPosition();
    void setPosition(glm::vec3 _position);
    void setCanCollide(bool canCollide);
    void setApplyPhysics(bool applyPhysics);

    // virtual void updatePhysics();
    virtual void collided(CollisionObjectPtr collidedWith) = 0;

    bool checkCollision(CollisionObjectPtr other);
    glm::vec3 getCollisionNormal(CollisionObjectPtr other);
    float getCollisionDistance(CollisionObjectPtr other);
    void setBoundingBox(BoundingBox _boundingBox);
    BoundingBox* getBoundingBox();

private:
    
    glm::vec3 position;
    bool _canCollide, _applyPhysics;
    int collisionID, collideWithID, velocity;
    BoundingBox boundingBox;

};

#endif
