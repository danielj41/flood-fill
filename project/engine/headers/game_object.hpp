#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

#include "bounding_box.hpp"

class GameObject{
public:
    GameObject();

    virtual void setup() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;

    bool canCollide();
    void setBoundingBox(BoundingBox _boundingBox);
    BoundingBox getBoundingBox();

    virtual void collided(GameObject * collidedWith);

protected:
    bool _canCollide;
    BoundingBox boundingBox;
};

#endif
