#include "temp_level.hpp"

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
#include "shadow_occluder_render.hpp"
#include "normal_map_border_render.hpp"
#include "time_manager.hpp"

TempLevel::TempLevel() : LevelTemplate("testLevel.txt"), timer(0.0f) {}

void TempLevel::setup(){
    INFO("Generating Test Level...");
    readFile();
    initalizeGrid();
    createRenders();

    createLevel();

    INFO("Setting up the cameras for the Test Level...");
    CameraPtr cam1(new Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, -5),
                             glm::vec3(0, 1, 0)));
    cam1->setProjectionMatrix(
        glm::perspective(glm::radians(90.0f),
                        (float) Global::ScreenWidth/Global::ScreenHeight,
                        0.1f, 100.f));

    addCamera("Camera1", cam1);
    setMainCamera("Camera1");
    setCullingCamera("Camera1");

    CameraPtr cam2(new Camera(glm::vec3(0, 1, 0), glm::vec3(-6, -3, 6),
                             glm::vec3(0, 1, 0)));
    cam2->setProjectionMatrix(
        glm::perspective(glm::radians(90.0f),
                        (float) Global::ScreenWidth/Global::ScreenHeight,
                        0.1f, 100.f));

    l1 = LightPtr(new Light(glm::vec3(1), 30.0f, glm::vec3(0, 30, 0)));
    l1->setPosition(l1->getDirection()*1.0f);

    Uniform3DGridPtr<int> typeGrid = getTypeGrid();
    gridCenter = glm::vec3((typeGrid->getMaxX() - typeGrid->getMinX())/2.0f,
                         (typeGrid->getMaxY() - typeGrid->getMinY())/2.0f,
                         (typeGrid->getMinZ() - typeGrid->getMaxZ())/2.0f);

    l1->setViewMatrix(glm::lookAt(
        l1->getDirection(),
        gridCenter, glm::vec3(0, 1, 0)));
    l1->setProjectionMatrix(glm::ortho<float>(-30,30,-30,30,-100,100));

    addLight("Sun", l1);

    INFO("Setting up the player for the Test Level...");
    player = PlayerPtr(new Player(cam1));
    player->setup();
    addGameObject("player" , player);
    CollisionManager::addCollisionObjectToList(player);

    debugPlayer = DebugPlayerPtr(new DebugPlayer(cam2));
    debugPlayer->setup();
    addGameObject("debugPlayer" , debugPlayer);

    addCamera("DebugCamera", cam2);
    // INFO("Creating Switch for the Test Level...");
    // SwitchPtr s1(new Switch(glm::vec3(0.9f, 0.1f, 0.1f), glm::vec3(29.7, 23, -45), 
    //                          glm::vec3(0,0,1), -20.0f, 1));
    // s1->setup();
    // addGameObject("s1", s1);
    // CollisionManager::addCollisionObjectToGrid(s1);

    // INFO("Creating Active Terrain for the Test Level...");
    // ActiveTerrainPtr a1(new ActiveTerrain(s1, glm::vec3(), glm::vec3(), 50.0f));
    // a1->setup();
    // addGameObject("a1", a1);

    // PTR_CAST(SolidCube, (*grid)(0, 10, 22))->getObject()->applyTexture(LoadManager::getTexture("DrainTexture"));
    // PTR_CAST(SolidCube, (*grid)(0, 10, 23))->getObject()->applyTexture(LoadManager::getTexture("DrainTexture"));
    // PTR_CAST(SolidCube, (*grid)(0, 10, 22))->getObject()->applyNormalMap(LoadManager::getTexture("RegularNormalMap"));
    // PTR_CAST(SolidCube, (*grid)(0, 10, 23))->getObject()->applyNormalMap(LoadManager::getTexture("RegularNormalMap"));
    // shearRegion(1, 4, 11, 11, 22, 23, 1, 0, 0.0f);
    // shearRegion(5, 8, 10, 10, 22, 23, 1, 0, 0.5f);

    // PTR_CAST(SolidCube, (*grid)(0, 10, 4))->getObject()->applyTexture(LoadManager::getTexture("DrainTexture"));
    // PTR_CAST(SolidCube, (*grid)(0, 10, 5))->getObject()->applyTexture(LoadManager::getTexture("DrainTexture"));
    // PTR_CAST(SolidCube, (*grid)(0, 10, 4))->getObject()->applyNormalMap(LoadManager::getTexture("RegularNormalMap"));
    // PTR_CAST(SolidCube, (*grid)(0, 10, 5))->getObject()->applyNormalMap(LoadManager::getTexture("RegularNormalMap"));
    // shearRegion(1, 4, 11, 11, 4, 5, 1, 0, 0.0f);
    // shearRegion(5, 8, 10, 10, 4, 5, 1, 0, 0.5f);
}

void TempLevel::update(){
    if(debugPlayer->isActive()){
        ASSERT(getCamera("Camera1") != getCamera("DebugCamera"), "Equal camera");
        setMainCamera("DebugCamera");
        getCamera("Camera1")->fix();
    }
    else{
        setMainCamera("Camera1");
        getCamera("Camera1")->fix(false, true, false);
    }
    timer += TimeManager::getDeltaTime() / 15.0;
    l1->setDirection(glm::vec3(20.0 * sin(timer * 3.1), 5.0 * sin(timer * 3.4 + 5.0) + 30.0, 20.0 * sin(timer * 3.8 + 2.0)));
    l1->setPosition(gridCenter + l1->getDirection()*1.0f);
    l1->setViewMatrix(glm::lookAt(
        gridCenter + l1->getDirection(),
        gridCenter, glm::vec3(0, 1, 0)));
    //INFO("yay");
}

void TempLevel::reset() {
    resetMaps();
    readFile();
    initalizeGrid();
    createRenders();

    createLevel();
    CameraPtr cam1(new Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, -5),
                             glm::vec3(0, 1, 0)));
    cam1->setProjectionMatrix(
        glm::perspective(glm::radians(90.0f),
                        (float) Global::ScreenWidth/Global::ScreenHeight,
                        0.1f, 100.f));

    addCamera("Camera1", cam1);
    setMainCamera("Camera1");
    setCullingCamera("Camera1");

    CameraPtr cam2(new Camera(glm::vec3(0, 1, 0), glm::vec3(-6, -3, 6),
                             glm::vec3(0, 1, 0)));
    cam2->setProjectionMatrix(
        glm::perspective(glm::radians(90.0f),
                        (float) Global::ScreenWidth/Global::ScreenHeight,
                        0.1f, 100.f));

    l1 = LightPtr(new Light(glm::vec3(1), 30.0f, glm::vec3(0, 30, 0)));
    l1->setPosition(l1->getDirection()*1.0f);

    Uniform3DGridPtr<int> typeGrid = getTypeGrid();
    gridCenter = glm::vec3((typeGrid->getMaxX() - typeGrid->getMinX())/2.0f,
                         (typeGrid->getMaxY() - typeGrid->getMinY())/2.0f,
                         (typeGrid->getMinZ() - typeGrid->getMaxZ())/2.0f);

    l1->setViewMatrix(glm::lookAt(
        l1->getDirection(),
        gridCenter, glm::vec3(0, 1, 0)));
    l1->setProjectionMatrix(glm::ortho<float>(-30,30,-30,30,-100,100));

    addLight("Sun", l1);

    INFO("Setting up the player for the Test Level...");
    player = PlayerPtr(new Player(cam1));
    player->setup();
    addGameObject("player" , player);
    CollisionManager::addCollisionObjectToList(player);

    debugPlayer = DebugPlayerPtr(new DebugPlayer(cam2));
    debugPlayer->setup();
    addGameObject("debugPlayer" , debugPlayer);

    addCamera("DebugCamera", cam2);
}

void TempLevel::createRenders(){
    INFO("Creating Renders...");

    RenderEngine::addRenderElement("camera", RenderElementPtr(new CameraPolygonsRender()), 1);

    RenderEngine::addRenderElement("regular", RenderElementPtr(new RegularPolygonsRender()), 1);
    RenderEngine::addRenderElement("debug", RenderElementPtr(new DebugRender()), -5);
    RenderEngine::addRenderElement("normalmap", RenderElementPtr(new NormalMapRender()), 1);
    RenderEngine::addRenderElement("normalmap-border", RenderElementPtr(new NormalMapBorderRender()), 1);
    RenderEngine::addRenderElement("textured", RenderElementPtr(new TexturedPolygonsRender()), 1);
    RenderEngine::addRenderElement("water", RenderElementPtr(new WaterRender()), 4);
    RenderEngine::addRenderElement("water-particle", RenderElementPtr(new WaterParticleRender()), 4);
    RenderEngine::addRenderElement("water-stream", RenderElementPtr(new WaterStreamRender()), 4);
    RenderEngine::addRenderElement("shadow", RenderElementPtr(new ShadowOccluderRender()), 0);

    RenderEngine::setRenderGrid(RenderGridPtr(new RenderGrid(typeGrid->getSizeX(), typeGrid->getSizeY(), typeGrid->getSizeZ(),
                                               typeGrid->getMinX(), typeGrid->getMaxX(),
                                               typeGrid->getMinY(), typeGrid->getMaxY(),
                                               typeGrid->getMinZ(), typeGrid->getMaxZ())));
}
