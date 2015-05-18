#ifndef COLLISION_MANAGER_HPP
#define COLLISION_MANAGER_HPP

#include <list>

#include "collision_object.hpp"
#include "uniform_3d_grid.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "pointer_macros.h"
DEF_PTR(CollisionManager, CollisionManagerPtr);

class CollisionManager{

public:
    static void detectCollisions();
    static void addCollisionObjectToList(CollisionObjectPtr object);
    static void addCollisionObjectToGrid(CollisionObjectPtr object);
    static void removeCollisionObject(CollisionObjectPtr object);
    static void removeCollisionObjectFromGrid(CollisionObjectPtr object);
    static void removeCollisionObject(CollisionObject* object);
    static void removeCollisionObjectFromGrid(CollisionObject* object);

    static void checkCollision(CollisionObjectPtr a);
    static void checkCollisionWithOffset(CollisionObjectPtr aObject, int x, int y, int z);
    static void initGrid(int x, int y, int z, glm::vec3 min, glm::vec3 max);
    static Uniform3DGrid<CollisionObjectPtr>* getGrid();

private:
    static std::list<CollisionObjectPtr> collisionObjects;
    static Uniform3DGrid<CollisionObjectPtr> grid;
};

#endif
