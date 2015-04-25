#ifndef TEST_LEVEL_HPP
#define TEST_LEVEL_HPP

#include "level_template.hpp"

class TestLevel : public LevelTemplate{

public:
    TestLevel();

    void setup();
    void update();
};

#endif
