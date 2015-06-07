#include "level_manager.hpp"
#include "director.hpp"
#include "collision_manager.hpp"
#include "render_engine.hpp"
#include "time_manager.hpp"

#include "tutorial_level.hpp"
#include "test_level.hpp"
#include "tunnel_level.hpp"
#include "menu.hpp"
#include "load_manager.hpp"

#define MAX_LEVEL 3

int curLevel = 1;
bool LevelManager::levelFinished = false;
float LevelManager::timeStamp = 0; 
std::string LevelManager::currentSound = "";
//public:
void LevelManager::nextLevel() {
    if(TimeManager::getTimeStamp() -timeStamp < .5)
        return;
    timeStamp = TimeManager::getTimeStamp(); 

    removeLevel(curLevel);

    curLevel++;

    if(curLevel > MAX_LEVEL) {
        curLevel = 1;
    }

    setupLevel(curLevel);  

    levelFinished = false;
}

void LevelManager::resetLevel() {
    removeLevel(curLevel);
    Menu::setNewLevel(true);
    setupLevel(curLevel);
}

void LevelManager::setupLevel(int level) {
    switch (level) {
    case 1:
    {
        TutorialLevelPtr level1(new TutorialLevel());
        Director::addScene(level1);
        Director::setScene("testLevel4.txt");
        currentSound = "background.wav";
        //LoadManager::getSound(currentSound)->playSound();
        //LoadManager::getSound(currentSound)->loopSound();
        break;
    }
    case 2:
    {
        TestLevelPtr level2(new TestLevel());
        Director::addScene(level2);
        Director::setScene("testLevel3.txt");
        currentSound = "background.wav";
        //LoadManager::getSound(currentSound)->playSound();
        //LoadManager::getSound(currentSound)->loopSound();
        break;
    }
    case 3:
    {
        TunnelLevelPtr level3(new TunnelLevel());
        Director::addScene(level3);
        Director::setScene("tunnellevel.txt");
        currentSound = "roygbiv_pece_cover.wav";
        //LoadManager::getSound(currentSound)->playSound();
        //LoadManager::getSound(currentSound)->loopSound();
        break;
    }
    }

    curLevel = level;
}

void LevelManager::removeLevel(int level) {
    switch (level) {
    case 1:
        CollisionManager::removeAllCollisionObjects();
        RenderEngine::resetRenderEngine();
        Director::removeScene("testLevel4.txt");
        break;
    case 2:
        CollisionManager::removeAllCollisionObjects();
        RenderEngine::resetRenderEngine();
        Director::removeScene("testLevel3.txt");
        break;
    case 3:
        CollisionManager::removeAllCollisionObjects();
        RenderEngine::resetRenderEngine();
        Director::removeScene("tunnellevel.txt");
        break;
    }
    if(currentSound != "") {
        //LoadManager::getSound(currentSound)->stopSound();
        currentSound = "";
    }
}

int LevelManager::getCurrentLevel() { return curLevel; }
