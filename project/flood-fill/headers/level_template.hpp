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

    Uniform3DGrid<int>* getTypeGrid();

    // Level blocks identifiers
    static const char COMMENT;
    static const int VOID_SPACE;
    static const int AIR;
    static const int SOLID_CUBE;
    static const int AVAILABLE_FILL_SPACE;
    static const int CHANGE_COLOR_BLUE;
    static const int CHANGE_COLOR_GREEN;
    static const int CHANGE_COLOR_RED;
    static const int CHANGE_COLOR_GREY;
    static const int WINNING_BLOCK;

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
    int minx, miny, minz, maxx, maxy, maxz;
    GameObject * createVoxel(int id, int i, int j, int k);

    //Special Game Object to initialize the grid
    static VoidVoxel * voidVoxel;
    static const std::string templatesFolder;
};

#endif
