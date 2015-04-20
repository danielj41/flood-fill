#ifndef COLLISION_OBJECT_HPP
#define COLLISION_OBJECT_HPP

#include "glm/glm.hpp"


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

    virtual void collided(CollisionObject * collidedWith) = 0;

private:
    
    glm::vec3 position;
    bool _canCollide;
    int collisionID, collideWithID;

};

#endif
