#include "player.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GLIncludes.h"
#include "global_variables.hpp"
#include "bounding_box.hpp"
#include "time_manager.hpp"
#include "load_manager.hpp"
#include "fluid_projectile.hpp"
#include "director.hpp"
#include "collision_manager.hpp"
#include "load_manager.hpp"
#include "material_manager.hpp"
#include "object.hpp"
#include "render_engine.hpp"    

#define BLUE    1
#define GREEN   2
#define RED     4
#define GREY    8

Player::Player(Camera * _camera)
  : GameObject(), CollisionObject(), camera(_camera),
    jumping(false), velocity(0), gravity(-2){}

void Player::setup() {
    INFO("Player Setup...");

	lastPosition = camera->getEye();
	
    setCollisionID(2);
    setCollideWithID(1 + 16 + 64);
	setCanCollide(true);
	
	INFO("Can Collide: " << canCollide());
    camera->fix(false, true, false);

    shootPressed = false;

    setBoundingBox(BoundingBox(glm::vec3(0.8f,0.8f,0.8f), glm::vec3(-0.8f,-0.8f,-0.8f)));
    getBoundingBox()->setPosition(camera->getEye() - glm::vec3(0,1.0f,0));

    LoadManager::loadSound("jump_land.wav");

    sky = new Object(
        LoadManager::getMesh("sphere.obj"),
        MaterialManager::getMaterial("None"));

    sky->applyTexture(LoadManager::getTexture("Sky"));
    sky->enableTexture();
    sky->scale(glm::vec3(-50.0f,-50.0f,-50.0f));
    sky->translate(getPosition());
    // RenderEngine::addObject(sky);

    colorMask = BLUE;
    RenderEngine::getRenderElement("textured")->addObject(sky);

    hand = new PlayerHand(getPosition());
    hand->setup();
    Director::getScene()->addGameObject(hand);
    CollisionManager::addCollisionObjectToList(hand);
}

void Player::update() {
    lastPosition = camera->getEye();

    if(glfwGetKey(Global::window, GLFW_KEY_SPACE) == GLFW_PRESS && !jumping){
        jumping = true;
        velocity = .6;
        camera->jump(velocity * 25.0 * TimeManager::getDeltaTime());
    }
    // jumping = true;

    float cameraSpeed = 5.0f*TimeManager::getDeltaTime();
    if(glfwGetKey(Global::window, GLFW_KEY_W) == GLFW_PRESS){
        camera->zoom(Camera::FORWARD_DIRECTION, cameraSpeed);
    }
    else if(glfwGetKey(Global::window, GLFW_KEY_S) == GLFW_PRESS){
        camera->zoom(Camera::BACKWARD_DIRECTION, cameraSpeed);
    }
    if(glfwGetKey(Global::window, GLFW_KEY_A) == GLFW_PRESS){
        camera->strafe(Camera::LEFT_DIRECTION, cameraSpeed);
    }
    else if(glfwGetKey(Global::window, GLFW_KEY_D) == GLFW_PRESS){
        camera->strafe(Camera::RIGHT_DIRECTION, cameraSpeed);
    }

    if(glfwGetKey(Global::window, GLFW_KEY_U) == GLFW_PRESS){
        colorMask = BLUE;
    } 
    else if(glfwGetKey(Global::window, GLFW_KEY_I) == GLFW_PRESS){
        colorMask = GREEN;
    }
    else if(glfwGetKey(Global::window, GLFW_KEY_O) == GLFW_PRESS){
        colorMask = RED;
    }
    else if(glfwGetKey(Global::window, GLFW_KEY_P) == GLFW_PRESS){
        colorMask = GREY;
    }
    
    if(jumping) {
        velocity += gravity * TimeManager::getDeltaTime();
        if(velocity < -0.6) {
            velocity = -0.6;
        }
        camera->jump(velocity * 25.0 * TimeManager::getDeltaTime());
        // if(camera->getEye().y <= 1) {
        //     jumping = false;    
        //     velocity = 0;        
        // }
    }

    getBoundingBox()->setPosition(camera->getEye() - glm::vec3(0,1.0f,0));
	setPosition(camera->getEye());
    sky->loadIdentity();
    sky->scale(glm::vec3(-50.0f,-50.0f,-50.0f));
    sky->translate(getPosition());

    if(glfwGetMouseButton(Global::window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && !shootPressed){
        FluidProjectile *fluidProjectile = new FluidProjectile(
            camera->getEye() + glm::vec3(0,0.5f,0), -glm::normalize(camera->getViewVector()), colorMask);
        fluidProjectile->setup();
        Director::getScene()->addGameObject(fluidProjectile);
        CollisionManager::addCollisionObjectToList(fluidProjectile);
        shootPressed = true;
    }
    if(glfwGetMouseButton(Global::window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
        shootPressed = false;
    }

    hand->setPosition(getBoundingBox()->getPosition() - 1.2f * camera->getViewVector());
}

void Player::collided(CollisionObject * collidedWith) {
  glm::vec3 normal;
  float dist;
  switch (collidedWith->getCollisionID()) {
  case 1:
  case 64:
	INFO("DETECTING COLLISION WITH BLOCK!");
    normal = getCollisionNormal(collidedWith);
    dist = getCollisionDistance(collidedWith);
    camera->setEye(getPosition() + normal * dist);
    setPosition(camera->getEye());
    getBoundingBox()->setPosition(camera->getEye() - glm::vec3(0,1.0f,0));

    //If on flat ground, jumping is done. 
    if(fabs(normal.y) > 0.1) {
        if(jumping)
            LoadManager::getSound("jump_land.wav")->playSound();
        velocity = 0;
        jumping = false;
    }
	
    break;
  case 16:
    INFO("DETECTING COLOR CHANGE!");
    colorMask = ((ColorChange *)collidedWith)->getColor();
    break;
  default:
	break;
  }

}

