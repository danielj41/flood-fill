#ifndef BOSS_LEVEL_HPP
#define BOSS_LEVEL_HPP
#include "level_template.hpp"
#include "text.hpp"
#include "pointer_macros.h"
DEF_PTR(BossLevel, BossLevelPtr);
class BossLevel : public LevelTemplate{
public:
BossLevel();
void setup();
void update();
private:
void createRenders();
LightPtr l1;
float timer;
glm::vec3 gridCenter;
TextPtr levelTitle;
ObjectPtr sky;
};
#endif
