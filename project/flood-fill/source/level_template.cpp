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
#include "collision_manager.hpp"


VoidVoxel * LevelTemplate::voidVoxel;

const std::string LevelTemplate::templatesFolder = "flood-fill/config-files/";

const char LevelTemplate::COMMENT = '#';

const int LevelTemplate::VOID_SPACE          = -1;
const int LevelTemplate::AIR                 =  0;
const int LevelTemplate::SOLID_CUBE          =  1;
const int LevelTemplate::AVAIABLE_FILL_SPACE =  2;


LevelTemplate::LevelTemplate(std::string levelFileName)
    : Scene(levelFileName) {
    fileName = templatesFolder;
    fileName += levelFileName;
}

void LevelTemplate::createLevel(){
    INFO("Creating level " << fileName << "...");

    std::ifstream levelFile;

    levelFile.open(fileName.c_str());
    ASSERT(levelFile.is_open(), "Could not open file " << fileName << "!");

    std::string line;
    std::vector<std::string> lines;

    INFO(fileName << " Layout: ");
    while(std::getline(levelFile, line)){
        lines.push_back(line);
        INFO(line);
    }

    levelFile.close();

    voidVoxel = new VoidVoxel();
    interpLines(lines);
}

void LevelTemplate::interpLines(std::vector<std::string> lines){
    bool mapDefined = false;

    std::string line;

    int numVoxelsInX, numVoxelsInY, numVoxelsInZ, minx, miny, minz, maxx, maxy, maxz;

    //Counts the line of the grid that is being read. Correspond to the Y component
    int j = 0;
    //Counts the slice of the grid that is being rea. Correspond to the Z componentd
    int k = 0;

    for(unsigned int linesIndex = 0; linesIndex < lines.size(); linesIndex++){
        line = lines[linesIndex];

        if(line.size() == 0)   continue;
        if(line[0] == COMMENT) continue;

        std::stringstream ss(line);

        if(!mapDefined){
            std::string mapDef;

            ss >> mapDef >> numVoxelsInX >> numVoxelsInY >> numVoxelsInZ >>
                            minx >> miny >> minz >> maxx >> maxy >> maxz ;

            ASSERT(mapDef == "map", "Expecting a map definition:"
                     << " map numVoxelsInX numVoxelsInY numVoxelsInZ"
                     << " minx miny minz maxx maxy maxz.\n"
                     << "Got: " << line);

            grid = Uniform3DGrid<GameObject *>(numVoxelsInX, numVoxelsInY, numVoxelsInZ,
                                               minx, miny, minz, maxx, maxy, maxz);
            grid.initialize(voidVoxel);

            typeGrid = Uniform3DGrid<int>(numVoxelsInX, numVoxelsInY, numVoxelsInZ,
                                          minx, miny, minz, maxx, maxy, maxz);
            typeGrid.initialize(VOID_SPACE);

            CollisionManager::initGrid(numVoxelsInX, numVoxelsInY, numVoxelsInZ,
                                          glm::vec3(minx, miny, minz),
                                          glm::vec3(maxx, maxy, maxz));

            mapDefined = true;
        }
        else{
            int voxelId;

            for(int i = 0; i < numVoxelsInX; i++){
                ss >> voxelId;
                typeGrid(i, j, k) = voxelId;
                grid(i, j, k)     = createVoxel(voxelId, i, j, k);
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

GameObject * LevelTemplate::createVoxel(int id, int i, int j, int k){
    switch(id){
    case AIR:
        return voidVoxel;
        break;
    case SOLID_CUBE:
    {
        SolidCube * c = new SolidCube(glm::vec3(i * 2 + 1, j * 2 + 1, -(k * 2 + 1)));
        c->setup();
        CollisionManager::addCollisionObjectToGrid(c);
        return c;
        break;
    }
    case AVAIABLE_FILL_SPACE:
        return voidVoxel;
        break;
    default:
        ASSERT(false, "Invalid type of Voxel. ID: " << id);
    }

    return voidVoxel;
}
