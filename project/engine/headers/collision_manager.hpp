#ifndef COLLISION_MANAGER_HPP
#define COLLISION_MANAGER_HPP

#include <list>

#include "collision_object.hpp"
#include "uniform_3d_grid.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"



class CollisionManager{

public:
    static void detectCollisions();
    static void addCollisionObjectToList(CollisionObject * object);
    static void addCollisionObjectToGrid(CollisionObject * object);
    static void removeCollisionObject(CollisionObject * object);

    static void checkCollision(CollisionObject * a);
    static void checkCollisionWithOffset(CollisionObject * aObject, int x, int y, int z);
    static void initGrid(int x, int y, int z, glm::vec3 min, glm::vec3 max);
    static Uniform3DGrid<CollisionObject *>* getGrid();

private:
    static std::list<CollisionObject *> collisionObjects;
    static Uniform3DGrid<CollisionObject *> grid;
};

#endif
