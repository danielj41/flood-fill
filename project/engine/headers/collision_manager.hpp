#ifndef COLLISION_MANAGER_HPP
#define COLLISION_MANAGER_HPP

#include <list>

#include "collision_object.hpp"

class CollisionManager{

public:
    static void detectCollisions();
    static void addCollisionObject(CollisionObject * object);
    static void removeCollisionObject(CollisionObject * object);

    static bool checkCollision(CollisionObject * a, CollisionObject * b);

private:
    static std::list<CollisionObject *> collisionObjects;
};

#endif
