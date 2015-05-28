#include "scene.hpp"

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "collision_manager.hpp"
#include "render_engine.hpp"

const std::string Scene::NO_CAMERA_AVAILABLE = "NO_CAMERA_AVAILABLE";

Scene::Scene() : labelCount(0) {
    DEBUG("Scene without a name!");
}

Scene::Scene(std::string _name)
    : labelCount(0), name(_name), currentCamera(NO_CAMERA_AVAILABLE){
    INFO("Creating scene " << name << "...");
}

std::string Scene::getName(){
    return name;
}

void Scene::updateObjects(){
    INFO("Updating scene " << name << "...");

    std::map<std::string, GameObjectPtr> tempMap = gameObjects;
    for(std::map<std::string, GameObjectPtr>::iterator it = tempMap.begin();
            it != tempMap.end(); ++it){
        INFO("Updating " << it->first << "...");
        it->second->update();
    }

    CollisionManager::detectCollisions();
}

void Scene::renderObjects(){
    INFO("Rendering scene " << name << "...");

    /*for(std::map<std::string, GameObject *>::iterator it = gameObjects.begin();
        it != gameObjects.end(); ++it){
        INFO("Rendering " << it->first << "...");
        it->second->draw();
		}*/

    RenderEngine::render();
}

void Scene::addCamera(std::string label, CameraPtr camera){
    ASSERT(camera->isReady(), "The camera is empty!");

    cameras[label] = camera;

    INFO("Camera " << label << " added to scene " << name);
}

void Scene::addGameObject(std::string label, GameObjectPtr gameObject){
    gameObjects[label] = gameObject;

    INFO("Object " << label << " added to scene " << name);
}

void Scene::addGameObject(GameObjectPtr gameObject){
    addGameObject(std::to_string(labelCount++), gameObject);
}

void Scene::addLight(std::string label, LightPtr light){
    lights[label] = light;

    INFO("Light " << label << " added to scene " << name);
}

void Scene::removeCamera(std::string label){
    INFO("Removing camera " << label << "...");

    ASSERT(cameras.find(label) != cameras.end(),
            "Camera " << label << " does not exist in this scene!");

    cameras.erase(cameras.find(label));
    INFO("Object " << label << " removed!");
}

void Scene::removeCamera(CameraPtr camera){
    INFO("Removing camera from scene...");

    for(std::map<std::string, CameraPtr>::iterator it = cameras.begin();
        it != cameras.end(); it++){

        if(it->second == camera){
            cameras.erase(it);

            INFO("Camera removed!");
            return;
        }
    }
    DEBUG("Camera does not exist in this scene!");
}

void Scene::removeGameObject(std::string label){
    INFO("Removing object " << label << "...");

    ASSERT(gameObjects.find(label) != gameObjects.end(),
            "Object " << label << " does not exist in this scene!");

    gameObjects.erase(gameObjects.find(label));
    INFO("Object " << label << " removed!");
}

void Scene::removeGameObject(GameObjectPtr gameObject){
    INFO("Removing game object from scene...");

    for(std::map<std::string, GameObjectPtr>::iterator it = gameObjects.begin();
        it != gameObjects.end(); it++){

        if(it->second == gameObject){
            INFO("Game Object " << it->first << " removed!");
            gameObjects.erase(it);

            return;
        }
    }
    DEBUG("Object does not exist in this scene!");
}

void Scene::removeGameObject(GameObject* gameObject){
    INFO("Removing game object from scene...");

    for(std::map<std::string, GameObjectPtr>::iterator it = gameObjects.begin();
        it != gameObjects.end(); it++){

        if(it->second.get() == gameObject){
            INFO("Game Object " << it->first << " removed!");
            gameObjects.erase(it);

            return;
        }
    }
    DEBUG("Object does not exist in this scene!");
}

void Scene::removeLight(std::string label){
    INFO("Removing light " << label << "...");

    ASSERT(lights.find(label) != lights.end(),
            "Light " << label << " does not exist in this scene!");

    lights.erase(lights.find(label));
    INFO("Light " << label << " removed!");
}

void Scene::removeLight(LightPtr light){
    INFO("Removing light from scene...");

    for(std::map<std::string, LightPtr>::iterator it = lights.begin();
        it != lights.end(); it++){

        if(it->second == light){
            INFO("Light " << it->first << " removed!");

            lights.erase(it);
            return;
        }
    }
    DEBUG("Light does not exist in this scene!");
}

CameraPtr Scene::getCamera(std::string label){
    ASSERT(cameras.find(label) != cameras.end(),
           "Camera " << label << " does not exist in this scene!");
    return cameras[label];
}

CameraPtr Scene::getCamera(){
    ASSERT(currentCamera != NO_CAMERA_AVAILABLE,
           "There is not a main camera setted for this scene");
    return getCamera(currentCamera);
}

CameraPtr Scene::getCullingCamera() {
    return getCamera(cullingCamera);
}

GameObjectPtr Scene::getGameObject(std::string label){
    ASSERT(gameObjects.find(label) != gameObjects.end(),
           "Object " << label << " does not exist in this scene!");
    return gameObjects[label];
}

LightPtr Scene::getLight(std::string label){
    ASSERT(lights.find(label) != lights.end(),
           "Light " << label << " does not exist in this scene!");
    return lights[label];
}

std::map<std::string, LightPtr> Scene::getLights(){
    return lights;
}

void Scene::setCullingCamera(std::string label) {
    cullingCamera = label;
}

void Scene::setMainCamera(std::string label){
    ASSERT(cameras.find(label) != cameras.end(),
           "Camera " << label << " does not exist in this scene!");
    currentCamera = label;
}

bool Scene::hasMainCamera(){
    return currentCamera != NO_CAMERA_AVAILABLE;
}

void Scene::resetMaps() {
    cameras.clear();
    lights.clear();
    gameObjects.clear();
}