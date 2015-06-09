
#include "tutorial_level.hpp"
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
#include "text.hpp"
#include "text_render.hpp"
#include "level_manager.hpp"
#include "menu.hpp"

TutorialLevel::TutorialLevel()
    : LevelTemplate("testLevel4.txt"), timer(0.0f), includeCinema (true) {
    resetHeight = -20.0f;
}

TutorialLevel::TutorialLevel(bool _includeCinema) 
    : LevelTemplate("testLevel4.txt"), timer(0.0f) , includeCinema(_includeCinema){
    resetHeight = -20.0f;
}

void TutorialLevel::setup(){
    INFO("Generating Tutorial Level...");
    readFile();
    initalizeGrid();
    createRenders();
    createLevel();
    waterSurfaceManager = WaterSurfaceManagerPtr(new WaterSurfaceManager());
    addGameObject(waterSurfaceManager);
    INFO("Removal String so less of make");
    INFO("Setting up the cameras for the Test Level...");
    CameraPtr cam3(new Camera(glm::vec3(25, 30, 0), glm::vec3(10, 20, 6),
                             glm::vec3(0, 1, 0)));
    cam3->setProjectionMatrix(
        glm::perspective(glm::radians(90.0f),
                        (float) Global::ScreenWidth/Global::ScreenHeight,
                        0.1f, 100.f));

    addCamera("CinematicCamera", cam3);
    setMainCamera("CinematicCamera");
    setCullingCamera("CinematicCamera");

    CameraPtr cam1(new Camera(glm::vec3(4, 10, -5), glm::vec3(4, 4, -10),
                              glm::vec3(0, 1, 0)));
    cam1->setProjectionMatrix(
        glm::perspective(glm::radians(90.0f),
                         (float) Global::ScreenWidth/Global::ScreenHeight,
                         0.1f, 100.f));
    addCamera("Camera1", cam1);
 
    CameraPtr cam2(new Camera(glm::vec3(0, 1, 0), glm::vec3(-6, -3, 6),
                              glm::vec3(0, 1, 0)));
    cam2->setProjectionMatrix(
        glm::perspective(glm::radians(90.0f),
                         (float) Global::ScreenWidth/Global::ScreenHeight,
                         0.1f, 100.f));
    l1 = LightPtr(new Light(glm::vec3(1), 30.0f, glm::vec3(0, 30, 0)));
    l1->setPosition(l1->getDirection());

    Uniform3DGridPtr<int> typeGrid = getTypeGrid();
    gridCenter = glm::vec3((typeGrid->getMaxX() - typeGrid->getMinX())/2.0f,
                           (typeGrid->getMaxY() - typeGrid->getMinY())/2.0f,
                           (typeGrid->getMinZ() - typeGrid->getMaxZ())/2.0f);
    l1->setViewMatrix(glm::lookAt(

        l1->getPosition(),
        gridCenter, glm::vec3(0, 1, 0)));
    l1->setProjectionMatrix(glm::ortho<float>(-30,30,-30,30,-70,70));

    addLight("Sun", l1);
    INFO("Setting up the player for the Test Level...");
    cinematicPlayer = CinematicPlayerPtr(new CinematicPlayer(cam3));
    cinematicPlayer->setup();
    addGameObject("cinematicPlayer", cinematicPlayer);

    player = PlayerPtr(new Player(cam1, 2));
    player->setup();
    addGameObject("player" , player);
    CollisionManager::addCollisionObjectToList(player);
    debugPlayer = DebugPlayerPtr(new DebugPlayer(cam2));
    debugPlayer->setup();
    addGameObject("debugPlayer" , debugPlayer);
    //Text
    addCamera("DebugCamera", cam2);

    sky = ObjectPtr(new Object(
        LoadManager::getMesh("sphere.obj"),
        MaterialManager::getMaterial("None")));

    sky->applyTexture(LoadManager::getTexture("Sky"));
    sky->enableTexture();
    sky->scale(glm::vec3(-90.0f,-90.0f,-90.0f));
    sky->translate(Director::getScene()->getCamera()->getEye());
    RenderEngine::getRenderElement("textured")->addObject(sky);
}

void TutorialLevel::update(){
    if (Menu::isNewLevel()) {
        float pixelDensityX = (float)Global::FbWidth / Global::ScreenWidth;
        levelTitle = TextPtr(new Text("Level1", glm::vec4(0, 0, 0, 1), glm::vec2(-0.5, 0), "FourPixel", 75));
        
        levelTitle->setPosition(glm::vec2(0-levelTitle->getTextWidth()/2.0/pixelDensityX + .05, 0)); 
        PTR_CAST(TextRender, RenderEngine::getRenderElement("text"))->addText(levelTitle);
        Menu::setNewLevel(false);
    }

    if(debugPlayer->isActive()){
        ASSERT(getCamera("Camera1") != getCamera("DebugCamera"), "Equal camera");
        setMainCamera("DebugCamera");
        getCamera("Camera1")->fix();
    }
    else if(cinematicPlayer->isActive() && includeCinema){
        ASSERT(getCamera("Camera1") != getCamera("CinematicCamera"), "Equal camera");
        setMainCamera("CinematicCamera");
        setCullingCamera("CinematicCamera");
        player->setActive(false);
    }
    else{
        player->setActive(true);
        setMainCamera("Camera1");
        setCullingCamera("Camera1");
        getCamera("Camera1")->fix(false, true, false);
    }
    timer += TimeManager::getDeltaTime() / 15.0;
    //l1->setDirection(glm::vec3(20.0 * sin(timer * 3.1), 5.0 * sin(timer * 3.4 + 5.0) + 30.0, 20.0 * sin(timer * 3.8 + 2.0)));
    //l1->setPosition(gridCenter + l1->getDirection()*1.0f);
    //l1->setViewMatrix(glm::lookAt(
    //    gridCenter + l1->getDirection(),
    //    gridCenter, glm::vec3(0, 1, 0)));

    // gridCenter + l1->getDirection(),
    // gridCenter, glm::vec3(0, 1, 0)));
    if (!Menu::isActive()) {
        glm::vec4 titleColor = levelTitle->getColor();
        if(titleColor.w > 0){
            titleColor.w -= TimeManager::getDeltaTime()*0.3;
            levelTitle->setColor(titleColor);
        }
    }

    if(player->getPosition().y <= resetHeight){
            LevelManager::resetLevel();
    }

    sky->loadIdentity();
    sky->scale(glm::vec3(-90.0f,-90.0f,-90.0f));
    sky->translate(Director::getScene()->getCamera()->getEye());
}

void TutorialLevel::createRenders(){
    INFO("Creating Renders...");
    RenderEngine::addRenderElement("camera", RenderElementPtr(new CameraPolygonsRender()), 1);
    RenderEngine::addRenderElement("regular", RenderElementPtr(new RegularPolygonsRender()), 1);
    RenderEngine::addRenderElement("debug", RenderElementPtr(new DebugRender()), 5);
    RenderEngine::addRenderElement("normalmap", RenderElementPtr(new NormalMapRender()), 1);
    RenderEngine::addRenderElement("normalmap-border", RenderElementPtr(new NormalMapBorderRender()), 1);
    RenderEngine::addRenderElement("textured", RenderElementPtr(new TexturedPolygonsRender()), 1);
    RenderEngine::addRenderElement("water", RenderElementPtr(new WaterRender()), 4);
    RenderEngine::addRenderElement("water-particle", RenderElementPtr(new WaterParticleRender()), 4);
    RenderEngine::addRenderElement("water-stream", RenderElementPtr(new WaterStreamRender()), 4);
    RenderEngine::addRenderElement("shadow", RenderElementPtr(new ShadowOccluderRender()), 1);
    RenderEngine::addRenderElement("text", RenderElementPtr(new TextRender()), 10);
    RenderEngine::setRenderGrid(RenderGridPtr(new RenderGrid(typeGrid->getSizeX(), typeGrid->getSizeY(), typeGrid->getSizeZ(),
                                                             typeGrid->getMinX(), typeGrid->getMaxX(),
                                                             typeGrid->getMinY(), typeGrid->getMaxY(),
                                                             typeGrid->getMinZ(), typeGrid->getMaxZ())));
}

