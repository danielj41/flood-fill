#ifndef LEVEL_TEMPLATE_HPP
#define LEVEL_TEMPLATE_HPP

#include <string>
#include <vector>

#include "scene.hpp"
#include "uniform_3d_grid.hpp"
#include "game_object.hpp"
#include "debug_player.hpp"
#include "player.hpp"
#include "void_voxel.hpp"

class LevelTemplate: public Scene{

public:
    LevelTemplate(std::string levelFileName);

    void createLevel();

    virtual void setup() = 0;
    virtual void update() = 0;

protected:
    std::string fileName;

    // The grid holds every game object in the map
    Uniform3DGrid<GameObject *> grid;
    // The type grid hold the types of every game object in the map
    Uniform3DGrid<int> typeGrid;

    DebugPlayer * debugPlayer;
    Player * player;

private:
    void interpLines(std::vector<std::string> lines);
    GameObject * createVoxel(int id, int i, int j, int k);

    //Special Game Object to initialize the grid
    static VoidVoxel * voidVoxel;
    static const std::string templatesFolder;

    // Level blocks identifiers
    static const char COMMENT;
    static const int VOID_SPACE;
    static const int AIR;
    static const int SOLID_CUBE;
    static const int AVAIABLE_FILL_SPACE;
};

#endif
