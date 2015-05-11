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
#include "shadow_occluder_render.hpp"

TestLevel::TestLevel() : LevelTemplate("testLevel3.txt"){}

void TestLevel::setup(){
    INFO("Generating Test Level...");
    readFile();
    initalizeGrid();
    createRenders();

    createLevel();

    INFO("Setting up the cameras for the Test Level...");
    Camera * cam1 = new Camera(glm::vec3(28, 16, -5), glm::vec3(0, 0, -5),
                             glm::vec3(0, 1, 0));
    cam1->setProjectionMatrix(
        glm::perspective(glm::radians(90.0f),
                        (float) Global::ScreenWidth/Global::ScreenHeight,
                        0.1f, 100.f));

    addCamera("Camera1", cam1);
    setMainCamera("Camera1");

    Camera * cam2 = new Camera(glm::vec3(0, 1, 0), glm::vec3(0, 0, -5),
                             glm::vec3(0, 1, 0));
    cam2->setProjectionMatrix(
        glm::perspective(glm::radians(90.0f),
                        (float) Global::ScreenWidth/Global::ScreenHeight,
                        0.1f, 100.f));

    Light * l1 = new Light(glm::vec3(1), 30.0f, glm::vec3(1.0, -1.0, 0.5));
    l1->setPosition(glm::vec3(1.0, -1.0, 0.5)*-15.0f);
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
    Switch * s1 = new Switch(glm::vec3(0.9f, 0.1f, 0.1f), glm::vec3(29.7, 19, -45), 
                             glm::vec3(0,0,1), -20.0f, 1);
    s1->setup();
    addGameObject("s1", s1);
    CollisionManager::addCollisionObjectToGrid(s1);

    INFO("Creating Active Terrain for the Test Level...");
    ActiveTerrain * a1 = new ActiveTerrain(s1, glm::vec3(), glm::vec3(), 50.0f);
    a1->setup();
    addGameObject("a1", a1);

    //Switch * s2 = new Switch(glm::vec3(7, 3, -2.25), glm::vec3(0,0,-1), glm::vec3(1,0,0));
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
}

void TestLevel::createRenders(){
    INFO("Creating Renders...");

    RenderEngine::addRenderElement("camera", new CameraPolygonsRender(), 1);

    RenderEngine::addRenderElement("regular", new RegularPolygonsRender(), 1);
    Uniform3DGrid<int>* typeGrid = getTypeGrid();
    ((RegularPolygonsRender *)RenderEngine::getRenderElement("regular"))->setGrid(
        Uniform3DGrid<Object *>(typeGrid->getSizeX(), typeGrid->getSizeY(), typeGrid->getSizeZ(),
                                typeGrid->getMinX(), typeGrid->getMaxX(),
                                typeGrid->getMinY(), typeGrid->getMaxY(),
                                typeGrid->getMinZ(), typeGrid->getMaxZ()));

    RenderEngine::addRenderElement("debug", new DebugRender(), 5);
    RenderEngine::addRenderElement("normalmap", new NormalMapRender(), 2);
    RenderEngine::addRenderElement("textured", new TexturedPolygonsRender(), 3);
    RenderEngine::addRenderElement("water", new WaterRender(), 4);
    RenderEngine::addRenderElement("water-particle", new WaterParticleRender(), 4);
    RenderEngine::addRenderElement("water-stream", new WaterStreamRender(), 4);
    RenderEngine::addRenderElement("shadow", new ShadowOccluderRender()), 0;
}
