#ifndef LEVEL_TEMPLATE_HPP
#define LEVEL_TEMPLATE_HPP

#include <string>
#include <vector>
#include <set>

#include "scene.hpp"
#include "uniform_3d_grid.hpp"
#include "game_object.hpp"
#include "debug_player.hpp"
#include "player.hpp"
#include "void_voxel.hpp"

#include "pointer_macros.h"
DEF_PTR(LevelTemplate, LevelTemplatePtr);

class LevelTemplate: public Scene{

public:
    LevelTemplate(std::string levelFileName);

    void readFile();
    void initalizeGrid();
    void createLevel();


    virtual void setup() = 0;
    virtual void update() = 0;

    Uniform3DGridPtr<int> getTypeGrid();
    std::set<int>* getFillTypes();
    GameObjectPtr getGridValue(glm::vec3 pos);
    void setGridValue(glm::vec3 pos, GameObjectPtr obj);
    void setTypeCell(glm::vec3 pos, int type);

    bool isFilledWithPaint(glm::vec3 pos);
    bool isEmpty(glm::vec3 pos);

    void shearRegion(int x1, int x2,  int y1, int y2, int z1, int z2, int shearX, int shearY, float endEarly);

    // Level blocks identifiers
    static const char COMMENT;
    static const int TOGGLE_FILL;
    static const int VOID_SPACE;
    static const int AIR;
    static const int SOLID_CUBE;
    static const int AVAILABLE_FILL_SPACE;
    static const int CHANGE_COLOR_BLUE;
    static const int CHANGE_COLOR_GREEN;
    static const int CHANGE_COLOR_RED;
    static const int CHANGE_COLOR_GREY;
    static const int WINNING_BLOCK;
    static const int FLUID_BLUE;
    static const int FLUID_RED;
    static const int FLUID_GREEN;
    static const int FLUID_DRAIN;

protected:
    std::string fileName;
    float minx, miny, minz, maxx, maxy, maxz;
    int numVoxelsInX, numVoxelsInY, numVoxelsInZ;

    // The grid holds every game object in the map
    Uniform3DGridPtr<GameObjectPtr> grid;
    // The type grid hold the types of every game object in the map
    Uniform3DGridPtr<int> typeGrid;
    std::set<int> fillTypes;

    DebugPlayerPtr debugPlayer;
    PlayerPtr player;

private:
    void interpLines(std::vector<std::string> lines);
    GameObjectPtr createVoxel(int id, int i, int j, int k);

    //Special Game Object to initialize the grid
    static VoidVoxelPtr voidVoxel;
    static const std::string templatesFolder;
    std::vector<std::string> lines;
    bool mapDefined;
};

#endif
