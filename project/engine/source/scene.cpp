#include "scene.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

const std::string Scene::NO_CAMERA_AVAILABLE = "NO_CAMERA_AVAILABLE";

Scene::Scene(){
    DEBUG("Scene without a name!");
}

Scene::Scene(std::string _name)
    : name(_name), currentCamera(NO_CAMERA_AVAILABLE){
    INFO("Creating scene " << name << "...");
}

std::string Scene::getName(){
    return name;
}

void Scene::update(){
    INFO("Updating scene " << name << "...");

    for(std::map<std::string, GameObject *>::iterator it = gameObjects.begin();
            it != gameObjects.end(); ++it){
        INFO("Updating " << it->first << "...");
        it->second->update();
    }
}

void Scene::render(){
    INFO("Rendering scene " << name << "...");

    for(std::map<std::string, GameObject *>::iterator it = gameObjects.begin();
        it != gameObjects.end(); ++it){
        INFO("Rendering " << it->first << "...");
        it->second->draw();
    }
}

void Scene::addCamera(std::string label, Camera * camera){
    ASSERT(camera->isReady(), "The camera is empty!");

    cameras[label] = camera;

    INFO("Camera " << label << " added to scene " << name);
}

void Scene::addGameObject(std::string label, GameObject * gameObject){
    gameObjects[label] = gameObject;

    INFO("Object " << label << " added to scene " << name);
}

void Scene::removeCamera(std::string label){
    INFO("Removing camera " << label << "...");

    std::map<std::string, Camera * >::iterator it = cameras.find(label);
    if(it == cameras.end()){
        DEBUG("Camera " << label << " does not exist in this scene!");
    }
    else{
        cameras.erase(it);
        INFO("Camera " << label << " removed!");
    }
}

void Scene::removeGameObject(std::string label){
    INFO("Removing object " << label << "...");

    std::map<std::string, GameObject *>::iterator it = gameObjects.find(label);
    if(it == gameObjects.end()){
        DEBUG("Object " << label << " does not exist in this scene!");
    }
    else{
        gameObjects.erase(it);
        INFO("Object " << label << " removed!");
    }
}

Camera * Scene::getCamera(std::string label){
    ASSERT(cameras.find(label) != cameras.end(),
           "Camera " << label << " does not exist in this scene!");
    return cameras[label];
}

Camera * Scene::getCamera(){
    ASSERT(currentCamera != NO_CAMERA_AVAILABLE,
           "There is not a main camera setted for this scene");
    return getCamera(currentCamera);
}

GameObject * Scene::getGameObject(std::string label){
    ASSERT(gameObjects.find(label) != gameObjects.end(),
           "Object " << label << " does not exist in this scene!");
    return gameObjects[label];
}

void Scene::setMainCamera(std::string label){
    ASSERT(cameras.find(label) != cameras.end(),
           "Camera " << label << " does not exist in this scene!");
    currentCamera = label;
}

bool Scene::hasMainCamera(){
    return currentCamera != NO_CAMERA_AVAILABLE;
}
