#include "director.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

const std::string Director::NO_VALID_SCENE = "NO_VALID_SCENE";

std::string Director::currentScene = NO_VALID_SCENE;
std::map<std::string, Scene *> Director::scenes;


void Director::addScene(Scene * scene){
    INFO("Adding scene " << scene->getName() << "...");

    if(scenes.find(scene->getName()) == scenes.end()){
        scenes[scene->getName()] = scene;
    }

    INFO("Scene " << scene->getName() << " added!");
}

void Director::removeScene(std::string label){
    INFO("Removing scene " << label << "...");

    if(scenes.find(label) != scenes.end()){
        scenes.erase(label);
        INFO("Scene " << label << " removed!");
    }
    else{
        DEBUG("Scene " << label << " does not exist!");
    }

}

Scene * Director::getScene(std::string label){
    ASSERT(scenes.find(label) != scenes.end(),
            "Scene " << label << " does not exist!");
    return scenes[label];
}

Scene * Director::getScene(){
    ASSERT(currentScene != NO_VALID_SCENE, "There is not a main scene");
    return Director::getScene(currentScene);
}

void Director::setScene(std::string label){
    ASSERT(scenes.find(label) != scenes.end(),
            "Scene " << label << " does not exist!");
    currentScene = label;
    getScene()->setup();
}

void Director::updateScene(){
    getScene()->update();
    getScene()->updateObjects();

}

void Director::renderScene(){
    getScene()->renderObjects();
}
