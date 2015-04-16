#ifndef COLLISION_OBJECT_HPP
#define COLLISION_OBJECT_HPP

#include "bounding_box.hpp"

class CollisionObject{

public:
    CollisionObject();
    CollisionObject(BoundingBox box);

    bool canCollide();
    BoundingBox getBoundingBox();
    void setBoundingBox(BoundingBox box);

    int getCollisionID();
    int getCollideWithID();

    void setCollisionID(int ID);
    void setCollideWithID(int ID);

    virtual void collided(CollisionObject * collidedWith) = 0;

protected:
    BoundingBox boundingBox;

private:
    bool _canCollide;
    int collisionID, collideWithID;

};

#endif
