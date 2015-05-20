#ifndef TEST_LEVEL_HPP
#define TEST_LEVEL_HPP

#include "level_template.hpp"

#include "pointer_macros.h"
DEF_PTR(TestLevel, TestLevelPtr);

class TestLevel : public LevelTemplate{

public:
    TestLevel();

    void setup();
    void update();

private:
    void createRenders();
    LightPtr l1;
    float timer;
    glm::vec3 gridCenter;
};

#endif
