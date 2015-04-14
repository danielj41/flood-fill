#ifndef COLLISION_MANAGER_HPP
#define COLLISION_MANAGER_HPP

#include <list>

#include "game_object.hpp"

class CollisionManager{

public:
    static void detectCollisions();
    static void removeGameObject(GameObject * object);
    static void addGameObject(GameObject * object);

private:
    static std::list<GameObject *> gameObjects;
};

#endif
