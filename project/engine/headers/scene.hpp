#ifndef SCENE_H
#define SCENE_H

#include <string>
#include <map>

#include "game_object.hpp"
#include "camera.hpp"

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
    void addGameObject(std::string label, GameObject * gameObject);

    void removeCamera(std::string label);
    void removeCamera(Camera * camera);
    void removeGameObject(std::string label);
    void removeGameObject(GameObject * gameObject);

    //Get a specific camera
    Camera * getCamera(std::string label);
    //Get the main camera
    Camera * getCamera();

    GameObject * getGameObject(std::string label);

    //Sets the camera that will look to scene
    void setMainCamera(std::string label);

    bool hasMainCamera();

protected:
    static const std::string NO_CAMERA_AVAILABLE;

    std::map<std::string, Camera *> cameras;
    std::map<std::string, GameObject *> gameObjects;

    std::string currentCamera;
    std::string name;
};

#endif
