#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <map>

#include "game_object.hpp"
#include "camera.hpp"
#include "light.hpp"

/**
* Scene class
*    This class holds all elements that composes a game scene: Camera,
*    Game Objects, and Lights.
*    On each game loop the scene render method is called, rendering all
*    game objects with the correct camera and lights.
*
*/

#include "pointer_macros.h"
DEF_PTR(Scene, ScenePtr);

class Scene{

public:
    Scene();
    Scene(std::string _name);

    virtual void setup() = 0;
    virtual void update() = 0;

    //Updates all objects in the scene
    void updateObjects();
    //Render all objects in the scene
    void renderObjects();

    std::string getName();

    void addCamera(std::string label, CameraPtr camera);
    void addLight(std::string label, LightPtr light);
    void addGameObject(std::string label, GameObjectPtr gameObject);
    void addGameObject(GameObjectPtr gameObject);

    void removeCamera(std::string label);
    void removeCamera(CameraPtr camera);
    void removeGameObject(std::string label);
    void removeGameObject(GameObjectPtr gameObject);
    void removeGameObject(GameObject* gameObject);
    void removeLight(std::string label);
    void removeLight(LightPtr light);

    //Get a specific camera
    CameraPtr getCamera(std::string label);
    //Get the main camera
    CameraPtr getCamera();
    CameraPtr getCullingCamera();

    GameObjectPtr getGameObject(std::string label);
    LightPtr getLight(std::string label);
    std::map<std::string, LightPtr> getLights();

    //Sets the camera that will look to scene
    void setMainCamera(std::string label);
    void setCullingCamera(std::string label);

    void resetMaps();

    bool hasMainCamera();

protected:
    static const std::string NO_CAMERA_AVAILABLE;

    int labelCount;
    std::string name, currentCamera, cullingCamera;

    std::map<std::string, CameraPtr> cameras;
    std::map<std::string, LightPtr> lights;
    std::map<std::string, GameObjectPtr> gameObjects;
};

#endif
