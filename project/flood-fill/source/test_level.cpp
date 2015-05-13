#include "test_level.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "camera.hpp"
#include "switch.hpp"
#include "global_variables.hpp"
#include "collision_manager.hpp"
#include "object.hpp"
#include "render_engine.hpp"
#include "load_manager.hpp"
#include "material_manager.hpp"
#include "regular_polygons_render.hpp"
#include "textured_polygons_render.hpp"
#include "water_stream_render.hpp"
#include "water_render.hpp"
#include "water_particle_render.hpp"
#include "normal_map_render.hpp"
#include "camera_polygons_render.hpp"
#include "debug_render.hpp"
#include "active_terrain.hpp"
#include "render_grid.hpp"

TestLevel::TestLevel() : LevelTemplate("testLevel3.txt"){}

void TestLevel::setup(){
    INFO("Generating Test Level...");
    readFile();
    initalizeGrid();
    createRenders();

    createLevel();

    INFO("Setting up the cameras for the Test Level...");
    Camera * cam1 = new Camera(glm::vec3(25, 30, -5), glm::vec3(0, 0, -5),
                             glm::vec3(0, 1, 0));
    cam1->setProjectionMatrix(
        glm::perspective(glm::radians(90.0f),
                        (float) Global::ScreenWidth/Global::ScreenHeight,
                        0.1f, 100.f));

    addCamera("Camera1", cam1);
    setMainCamera("Camera1");
    setCullingCamera("Camera1");

    Camera * cam2 = new Camera(glm::vec3(0, 1, 0), glm::vec3(0, 0, -5),
                             glm::vec3(0, 1, 0));
    cam2->setProjectionMatrix(
        glm::perspective(glm::radians(90.0f),
                        (float) Global::ScreenWidth/Global::ScreenHeight,
                        0.1f, 100.f));

    Light * l1 = new Light(glm::vec3(1), 30.0f, glm::vec3(1.0, -1.0, 0.5));
    addLight("Sun", l1);

    INFO("Setting up the player for the Test Level...");
    player = new Player(cam1);
    player->setup();
    addGameObject("player" , player);
    CollisionManager::addCollisionObjectToList(player);

    debugPlayer = new DebugPlayer(cam2);
    debugPlayer->setup();
    addGameObject("debugPlayer" , debugPlayer);

    addCamera("DebugCamera", cam2);
    
    INFO("Creating Switch for the Test Level...");
    Switch * s1 = new Switch(glm::vec3(0.9f, 0.1f, 0.1f), glm::vec3(29.7, 23, -45), 
                             glm::vec3(0,0,1), -20.0f, 1);
    s1->setup();
    addGameObject("s1", s1);
    CollisionManager::addCollisionObjectToGrid(s1);

    INFO("Creating Active Terrain for the Test Level...");
    ActiveTerrain * a1 = new ActiveTerrain(s1, glm::vec3(), glm::vec3(), 50.0f);
    a1->setup();
    addGameObject("a1", a1);

    // Adding AVAILABLE_FILL_SPACE to fillTypes
    fillTypes.insert(LevelTemplate::AVAILABLE_FILL_SPACE);
    //fillTypes.insert(LevelTemplate::TOGGLE_FILL);
}

void TestLevel::update(){
    if(debugPlayer->isActive()){
        ASSERT(getCamera("Camera1") != getCamera("DebugCamera"), "Equal camera");
        setMainCamera("DebugCamera");
        getCamera("Camera1")->fix();
    }
    else{
        setMainCamera("Camera1");
        getCamera("Camera1")->fix(false, true, false);
    }
    //INFO("yay");
}

void TestLevel::createRenders(){
    INFO("Creating Renders...");
    RenderEngine::addRenderElement("camera", new CameraPolygonsRender());
    RenderEngine::addRenderElement("regular", new RegularPolygonsRender());
    RenderEngine::addRenderElement("debug", new DebugRender());
    RenderEngine::addRenderElement("normalmap", new NormalMapRender());
    RenderEngine::addRenderElement("textured", new TexturedPolygonsRender());
    RenderEngine::addRenderElement("water", new WaterRender());
    RenderEngine::addRenderElement("water-particle", new WaterParticleRender());
    RenderEngine::addRenderElement("water-stream", new WaterStreamRender());
    RenderEngine::setRenderGrid(new RenderGrid(typeGrid.getSizeX(), typeGrid.getSizeY(), typeGrid.getSizeZ(),
                                               typeGrid.getMinX(), typeGrid.getMaxX(),
                                               typeGrid.getMinY(), typeGrid.getMaxY(),
                                               typeGrid.getMinZ(), typeGrid.getMaxZ()));
}
