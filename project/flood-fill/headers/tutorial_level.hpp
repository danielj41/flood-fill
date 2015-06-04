#ifndef TUTORIAL_LEVEL_HPP
#define TUTORIAL_LEVEL_HPP
#include "level_template.hpp"
#include "text.hpp"
#include "pointer_macros.h"
DEF_PTR(TutorialLevel, TutorialLevelPtr);
class TutorialLevel : public LevelTemplate{
public:
TutorialLevel();
void setup();
void update();
private:
void createRenders();
LightPtr l1;
float timer;
glm::vec3 gridCenter;
TextPtr levelTitle;
};
#endif
