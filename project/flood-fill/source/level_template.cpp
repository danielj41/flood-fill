#include "level_template.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "solid_cube.hpp"
#include "winning_block.hpp"
#include "color_change.hpp"
#include "collision_manager.hpp"
#include "director.hpp"
#include "fluid_box.hpp"


VoidVoxelPtr LevelTemplate::voidVoxel;

const std::string LevelTemplate::templatesFolder = "flood-fill/config-files/";

const char LevelTemplate::COMMENT = '#';

const int LevelTemplate::VOID_SPACE            = -1;
const int LevelTemplate::AIR                   =  0;
const int LevelTemplate::SOLID_CUBE            =  1;
const int LevelTemplate::AVAILABLE_FILL_SPACE  =  2;
const int LevelTemplate::CHANGE_COLOR_BLUE     =  3;
const int LevelTemplate::CHANGE_COLOR_GREEN    =  4;
const int LevelTemplate::CHANGE_COLOR_RED      =  5;
//const int LevelTemplate::CHANGE_COLOR_GREY     =  6;
const int LevelTemplate::TOGGLE_FILL           =  6;
const int LevelTemplate::FLUID_GREEN           =  7;
const int LevelTemplate::WINNING_BLOCK         =  8;
const int LevelTemplate::FLUID_RED             =  9;
const int LevelTemplate::FLUID_BLUE             =  10;

LevelTemplate::LevelTemplate(std::string levelFileName)
    : Scene(levelFileName) {
    fileName = templatesFolder;
    fileName += levelFileName;
}

void LevelTemplate::readFile(){
    INFO("Reading level " << fileName << " level...");

    std::ifstream levelFile;

    levelFile.open(fileName.c_str());
    ASSERT(levelFile.is_open(), "Could not open file " << fileName << "!");

    std::string line;

    INFO(fileName << " Layout: ");
    while(std::getline(levelFile, line)){
        lines.push_back(line);
        INFO(line);
    }

    levelFile.close();
}

void LevelTemplate::initalizeGrid(){
    ASSERT(lines.size() != 0, "You didn't read the text level file " << fileName);

    voidVoxel = VoidVoxelPtr(new VoidVoxel());

    std::string line = "";

    for(unsigned int linesIndex = 0; linesIndex < lines.size(); linesIndex++){
        line = lines[linesIndex];

        if(line.size() == 0)   continue;
        if(line[0] == COMMENT) continue;

        std::stringstream ss(line);

        std::string mapDef;

        ss >> mapDef >> numVoxelsInX >> numVoxelsInY >> numVoxelsInZ >>
                        minx >> miny >> minz >> maxx >> maxy >> maxz ;

        ASSERT(mapDef == "map", "Expecting a map definition:"
                 << " map numVoxelsInX numVoxelsInY numVoxelsInZ"
                 << " minx miny minz maxx maxy maxz.\n"
                 << "Got: " << line);

        grid = Uniform3DGridPtr<GameObjectPtr>(
            new Uniform3DGrid<GameObjectPtr>(numVoxelsInX, numVoxelsInY, numVoxelsInZ,
                                             minx, maxx, miny, maxy, minz, maxz));
        grid->initialize(voidVoxel);

        typeGrid = Uniform3DGridPtr<int>(
            new Uniform3DGrid<int>(numVoxelsInX, numVoxelsInY, numVoxelsInZ,
                                   minx, maxx, miny, maxy, minz, maxz));
        typeGrid->initialize(VOID_SPACE);

        CollisionManager::initGrid(numVoxelsInX, numVoxelsInY, numVoxelsInZ,
                                      glm::vec3(minx, miny, minz),
                                      glm::vec3(maxx, maxy, maxz));
        break;
    }

    mapDefined = true;
}

void LevelTemplate::createLevel(){
    INFO("Creating level " << fileName << "...");

    interpLines(lines);
}

void LevelTemplate::interpLines(std::vector<std::string> lines){
    mapDefined = false;

    //Counts the line of the grid that is being read. Correspond to the Y component
    int j = 0;
    //Counts the slice of the grid that is being rea. Correspond to the Z componentd
    int k = 0;

    for(unsigned int linesIndex = 0; linesIndex < lines.size(); linesIndex++){
        std::string line = lines[linesIndex];

        if(line.size() == 0)   continue;
        if(line[0] == COMMENT) continue;

        std::stringstream ss(line);

        if(!mapDefined){
            mapDefined = true;
        }
        else{
            int voxelId;

            for(int i = 0; i < numVoxelsInX; i++){
                ss >> voxelId;
                (*typeGrid)(i, j, k) = voxelId;
                (*grid)(i, j, k)     = createVoxel(voxelId, i, (numVoxelsInY - j - 1), k);
            }

            j++;
            if(j == numVoxelsInY){
                j = 0;
                k++;
                if(k == numVoxelsInZ){
                    k = 0;
                }
            }
        }

    }

    ASSERT(mapDefined, "Could not find a map definition inside file " <<
                        fileName);
}

GameObjectPtr LevelTemplate::createVoxel(int id, int i, int j, int k){
    switch(id){
    case AIR:
        return voidVoxel;
        break;
    case SOLID_CUBE:
    {
        SolidCubePtr c(new SolidCube(glm::vec3(minx + i * 2 + 1,
                                                miny + j * 2 + 1,
                                                minz + (k * 2 + 1))));
        c->setup();

        CollisionManager::addCollisionObjectToGrid(c);
        return c;
        break;
    }
    case AVAILABLE_FILL_SPACE:
        return voidVoxel;
        break;
    case WINNING_BLOCK:
    {
        WinningBlockPtr c(new WinningBlock(glm::vec3(minx + i * 2 + 1, miny + j * 2 + 1, minz + (k * 2 + 1))));
        c->setup();
        CollisionManager::addCollisionObjectToGrid(c);
        return c;
        break;
    }
    case CHANGE_COLOR_BLUE:
    {
        ColorChangePtr c(new ColorChange(glm::vec3(minx + i * 2 + 1, miny + j * 2 + 1, minz + (k * 2 + 1)), 1));
        c->setup();
        CollisionManager::addCollisionObjectToGrid(c);
        return c;
        break;
    }
    case CHANGE_COLOR_GREEN:
    {
        ColorChangePtr c(new ColorChange(glm::vec3(minx + i * 2 + 1, miny + j * 2 + 1, minz + (k * 2 + 1)), 2));
        c->setup();
        CollisionManager::addCollisionObjectToGrid(c);
        Director::getScene()->addGameObject(c);
        return c;
        break;
    }
    case FLUID_GREEN:
    {
        FluidBoxPtr c(new FluidBox(glm::vec3(minx + i * 2 + 1, miny + j * 2 + 1, minz + (k * 2 + 1)), 2));
        c->setup();
        CollisionManager::addCollisionObjectToGrid(c);
        Director::getScene()->addGameObject(c);
        return c;
        break;
    }
    case CHANGE_COLOR_RED:
    {
        ColorChangePtr c(new ColorChange(glm::vec3(minx + i * 2 + 1, miny + j * 2 + 1, minz + (k * 2 + 1)), 4));
        c->setup();
        CollisionManager::addCollisionObjectToGrid(c);
        Director::getScene()->addGameObject(c);
        return c;
        break;
    }
    case FLUID_RED:
    {
        FluidBoxPtr c(new FluidBox(glm::vec3(minx + i * 2 + 1, miny + j * 2 + 1, minz + (k * 2 + 1)), 4));
        c->setup();
        CollisionManager::addCollisionObjectToGrid(c);
        Director::getScene()->addGameObject(c);
        return c;
        break;
    }
    /*case CHANGE_COLOR_GREY:
    {
        ColorChange * c = new ColorChange(glm::vec3(minx + i * 2 + 1, miny + j * 2 + 1, minz + (k * 2 + 1)), 8);
        c->setup();
        CollisionManager::addCollisionObjectToGrid(c);
        Director::getScene()->addGameObject(c);
        return c;
        break;
        }*/
    case TOGGLE_FILL:
    {
        break;
    }    
    default:
        ASSERT(false, "Invalid type of Voxel. ID: " << id);
    }

    return voidVoxel;
}

Uniform3DGridPtr<int> LevelTemplate::getTypeGrid() {
    return typeGrid;
}

std::set<int>* LevelTemplate::getFillTypes()
{
    return &fillTypes;
}

bool LevelTemplate::isFilledWithPaint(glm::vec3 pos){
    //Check if the pos is inside the grid range
    if(minx > pos.x || pos.x > maxx ||
       miny > pos.y || pos.y > maxy ||
       minz > pos.z || pos.z > maxz){
       return false;
    }
    int type = typeGrid->getValue(pos.x, pos.y, pos.z);
    return type == FLUID_BLUE || type == FLUID_GREEN || type == FLUID_RED;
}

bool LevelTemplate::isEmpty(glm::vec3 pos){
    //Check if the pos is inside the grid range
    if(minx > pos.x || pos.x > maxx ||
       miny > pos.y || pos.y > maxy ||
       minz > pos.z || pos.z > maxz){
       return false;
    }
    int type = typeGrid->getValue(pos.x, pos.y, pos.z);
    return type == AVAILABLE_FILL_SPACE || type == VOID_SPACE || type == AIR || type == TOGGLE_FILL;
}

GameObjectPtr LevelTemplate::getGridValue(glm::vec3 pos){
    return grid->getValue(pos.x, pos.y, pos.z);
}
void LevelTemplate::setGridValue(glm::vec3 pos, GameObjectPtr obj){
    grid->setValue(pos.x, pos.y, pos.z, obj);
}

void LevelTemplate::setTypeCell(glm::vec3 pos, int type){
    typeGrid->setValue(pos.x, pos.y, pos.z, type);
}

void LevelTemplate::shearRegion(int x1, int x2, int y1, int y2, int z1, int z2, int shearX, int shearZ) {
    for(int x = x1; x <= x2; x++) {
        for(int y = y1; y <= y2; y++) {
            for(int z = z1; z <= z2; z++) {
                PTR_CAST(SolidCube, (*grid)(x, y, z))->getObject()->setShear(shearX, shearZ);
                (*CollisionManager::getGrid())(x, y, z)->setShear(
                    grid->getEdgeSizeY() * (abs(shearX) * 0.5f - shearX/2.0f + shearX * (float)(x - x1) / (x2 + 1 - x1)),
                    grid->getEdgeSizeY() * (abs(shearX) * 0.5f - shearX/2.0f + shearX * (float)(x + 1 - x1) / (x2 + 1 - x1)),
                    grid->getEdgeSizeY() * (abs(shearZ) * 0.5f - shearZ/2.0f + shearZ * (float)(z - z1) / (z2 + 1 - z1)),
                    grid->getEdgeSizeY() * (abs(shearZ) * 0.5f - shearZ/2.0f + shearZ * (float)(z + 1 - z1) / (z2 + 1 - z1))
                );
            }
        }   
    }
}

