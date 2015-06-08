#ifndef TEST_LEVEL_HPP
#define TEST_LEVEL_HPP

#include "level_template.hpp"
#include "text.hpp"
#include "text_render.hpp"

#include "pointer_macros.h"
DEF_PTR(TestLevel, TestLevelPtr);

class TestLevel : public LevelTemplate{

public:
    TestLevel();
    TestLevel(bool _includeCinema);
    void setup();
    void update();
    void reset();

private:
    void createRenders();
    LightPtr l1;
    float timer;
    bool includeCinema;
    glm::vec3 gridCenter;
    TextPtr levelTitle;
    ObjectPtr sky;
};

#endif
