#ifndef TEMP_LEVEL_HPP
#define TEMP_LEVEL_HPP

#include "level_template.hpp"

#include "pointer_macros.h"
DEF_PTR(TempLevel, TempLevelPtr);

class TempLevel : public LevelTemplate{

public:
    TempLevel();

    void setup();
    void update();
    void reset();

private:
    void createRenders();
    LightPtr l1;
    float timer;
    glm::vec3 gridCenter;
};

#endif
