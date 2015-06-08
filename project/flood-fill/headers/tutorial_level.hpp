#ifndef TUTORIAL_LEVEL_HPP
#define TUTORIAL_LEVEL_HPP
#include "level_template.hpp"
#include "text.hpp"
#include "pointer_macros.h"
DEF_PTR(TutorialLevel, TutorialLevelPtr);
class TutorialLevel : public LevelTemplate{
public:
TutorialLevel();
TutorialLevel(bool _includeCinema);
void setup();
void update();
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
