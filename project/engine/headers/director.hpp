#ifndef DIRECTOR_HPP
#define DIRECTOR_HPP

/**
* Director Class
*    This class controls all scenes. And also choses which scene is going to be
*    presented in the moment.
*    The scope of this class is Global.
*/

#include <string>
#include <map>

#include "scene.hpp"

class Director{

public:
    static void addScene(Scene * scene);
    static void removeScene(std::string label);

    //Returns a specific scene
    static Scene * getScene(std::string label);

    //Returns the current scene
    static Scene * getScene();

    static void setScene(std::string label);

private:
    static const std::string NO_VALID_SCENE;

    static std::string currentScene;
    static std::map<std::string, Scene *> scenes;

};

#endif
