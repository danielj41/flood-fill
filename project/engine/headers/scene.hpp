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

    void addCamera(std::string label, Camera * camera);
    void addLight(std::string label, Light * light);
    void addGameObject(std::string label, GameObject * gameObject);
    void addGameObject(GameObject * gameObject);

    void removeCamera(std::string label);
    void removeCamera(Camera * camera);
    void removeGameObject(std::string label);
    void removeGameObject(GameObject * gameObject);
    void removeLight(std::string label);
    void removeLight(Light * light);

    //Get a specific camera
    Camera * getCamera(std::string label);
    //Get the main camera
    Camera * getCamera();
    Camera * getCullingCamera();

    GameObject * getGameObject(std::string label);
    Light * getLight(std::string label);
    std::map<std::string, Light *> getLights();

    //Sets the camera that will look to scene
    void setMainCamera(std::string label);
    void setCullingCamera(std::string label);

    bool hasMainCamera();

protected:
    static const std::string NO_CAMERA_AVAILABLE;

    int labelCount;
    std::string name, currentCamera, cullingCamera;

    std::map<std::string, Camera *> cameras;
    std::map<std::string, Light *> lights;
    std::map<std::string, GameObject *> gameObjects;
};

#endif
