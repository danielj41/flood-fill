#include "director.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

const std::string Director::NO_VALID_SCENE = "NO_VALID_SCENE";

std::string Director::currentScene = NO_VALID_SCENE;
std::map<std::string, ScenePtr> Director::scenes;


void Director::addScene(ScenePtr scene){
    DEBUG("Adding scene " << scene->getName() << "...");

    if(scenes.find(scene->getName()) == scenes.end()){
        scenes[scene->getName()] = scene;
    }

    DEBUG("Scene " << scene->getName() << " added!");
}

void Director::removeScene(std::string label){
    DEBUG("Removing scene " << label << "...");

    scenes.clear();
}

ScenePtr Director::getScene(std::string label){
    ASSERT(scenes.find(label) != scenes.end(),
            "Scene " << label << " does not exist!");
    return scenes[label];
}

ScenePtr Director::getScene(){
    ASSERT(currentScene != NO_VALID_SCENE, "There is not a main scene");
    return Director::getScene(currentScene);
}

void Director::setScene(std::string label){
    // ASSERT(scenes.find(label) != scenes.end(),
            // "Scene " << label << " does not exist!");
    currentScene = label;
    getScene()->setup();
}

void Director::updateScene(){
    getScene()->updateObjects();
    getScene()->update();
}

void Director::renderScene(){
    getScene()->renderObjects();
}
