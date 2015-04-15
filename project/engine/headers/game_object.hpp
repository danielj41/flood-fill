#ifndef GAME_OBJECT_HPP
#define GAME_OBJECT_HPP

class GameObject{
public:
    GameObject();

    virtual void setup() = 0;
    virtual void update() = 0;
    virtual void draw() = 0;
};

#endif
