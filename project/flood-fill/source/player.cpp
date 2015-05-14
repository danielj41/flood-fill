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
    jumping(true), velocity(0), gravity(-2), strafeVelocity(0), forwardVelocity(0) {}

void Player::setup() {
    INFO("Player Setup...");

	lastPosition = camera->getEye();
	
    setCollisionID(2);
    setCollideWithID(1 | 16 | 32 | 64 | 128);

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

    gun = new Object(
        LoadManager::getMesh("gun.obj"),
        MaterialManager::getMaterial("FlatBlue"));
    gun->loadIdentity();
    //gun->scale(glm::vec3(0.07f, 0.07f, 0.3f));
    gun->rotate(15.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    gun->translate(glm::vec3(0.27f, -0.26f, -0.4f));
    gun->applyTexture(LoadManager::getTexture("GunTexture"));
    gun->enableTexture();
    RenderEngine::getRenderElement("camera")->addObject(gun);


    hand = new PlayerHand(getPosition());
    hand->setup();
    Director::getScene()->addGameObject(hand);
    CollisionManager::addCollisionObjectToList(hand);

    shootTimer = -1.0f;
}

void Player::update() {
    float dt = TimeManager::getDeltaTime();
    if(dt > 0.04) {
        dt = 0.04; // prevent falling through blocks with low framerate
    }

    lastPosition = camera->getEye();

    if(isKeyPressed(GLFW_KEY_SPACE) && !jumping) {
        velocity = .6;
        camera->jump(velocity * 25.0 * dt);
    }
    
    jumping = true;
  
    if(isKeyPressed(GLFW_KEY_W)){
        forwardVelocity = fmin(forwardVelocity + 1.5 * dt, 0.25f - 0.07f * fabs(strafeVelocity));
    } else if(forwardVelocity > 0.0f) {
        forwardVelocity = fmax(forwardVelocity - 1.5 * dt, 0.0f);
    }
    if(isKeyPressed(GLFW_KEY_S)){
        forwardVelocity = fmax(forwardVelocity - 1.5 * dt, -0.25f + 0.07f * fabs(strafeVelocity));
    } else if(forwardVelocity < 0.0f) {
        forwardVelocity = fmin(forwardVelocity + 1.5 * dt, 0.0f);
    }
    if(isKeyPressed(GLFW_KEY_D)){
        strafeVelocity = fmin(strafeVelocity + 1.5 * dt, 0.25f - 0.07f * fabs(forwardVelocity));
    } else if(strafeVelocity > 0.0f) {
        strafeVelocity = fmax(strafeVelocity - 1.5 * dt, 0.0f);
    }
    else if(isKeyPressed(GLFW_KEY_A)){
        strafeVelocity = fmax(strafeVelocity - 1.5 * dt, -0.25f + 0.07f * fabs(forwardVelocity));
    } else if(strafeVelocity < 0.0f) {
        strafeVelocity = fmax(strafeVelocity - 1.5 * dt, 0.0f);
    }

    camera->zoom(Camera::FORWARD_DIRECTION, forwardVelocity * dt * 15.0f);
    camera->strafe(Camera::RIGHT_DIRECTION, strafeVelocity * dt * 15.0f);

    if(glfwGetKey(Global::window, GLFW_KEY_U) == GLFW_PRESS){
        colorMask = BLUE;
        gun->setMaterial(MaterialManager::getMaterial("FlatBlue"));
    } 
    else if(glfwGetKey(Global::window, GLFW_KEY_I) == GLFW_PRESS){
        colorMask = GREEN;
        gun->setMaterial(MaterialManager::getMaterial("FlatGreen"));
    }
    else if(glfwGetKey(Global::window, GLFW_KEY_O) == GLFW_PRESS){
        colorMask = RED;
        gun->setMaterial(MaterialManager::getMaterial("FlatRed"));
    }
    else if(glfwGetKey(Global::window, GLFW_KEY_P) == GLFW_PRESS){
        colorMask = GREY;
        gun->setMaterial(MaterialManager::getMaterial("FlatGrey"));
    }
    hand->setColorMask(colorMask);
    
    if(jumping) {
        velocity += gravity * dt;
        if(velocity < -0.5) {
            velocity = -0.5;
        }
        camera->jump(velocity * 25.0 * dt);
    }

    getBoundingBox()->setPosition(camera->getEye() - glm::vec3(0,1.0f,0));
	setPosition(camera->getEye());
    sky->loadIdentity();
    sky->scale(glm::vec3(-50.0f,-50.0f,-50.0f));
    sky->translate(getPosition());

    if(glfwGetMouseButton(Global::window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && !shootPressed && shootTimer < 0.0f){
        FluidProjectile *fluidProjectile = new FluidProjectile(
            camera->getEye() - (0.35f * camera->getStrafeVector()) + glm::vec3(0.0f, 0.5f, 0.0f),
            -glm::normalize(camera->getViewVector()),
            colorMask);
        fluidProjectile->setup();
        Director::getScene()->addGameObject(fluidProjectile);
        CollisionManager::addCollisionObjectToList(fluidProjectile);
        shootPressed = true;
        shootTimer = 2.0f;
    }
    if(glfwGetMouseButton(Global::window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
        shootPressed = false;
    }

    if(shootTimer > 0.0f) {
        shootTimer -= TimeManager::getDeltaTime();
    }

    hand->setPosition(getBoundingBox()->getPosition() - 1.2f * camera->getViewVector());
}

void Player::collided(CollisionObject * collidedWith) {
  glm::vec3 normal;
  float dist;
  switch (collidedWith->getCollisionID()) {
  case 32: 
      INFO("DETECTING COLLISION WITH SWITCH!");
      break;
  case 1:
  case 64:
	INFO("DETECTING COLLISION WITH BLOCK!");
    normal = getCollisionNormal(collidedWith);
    dist = getCollisionDistance(collidedWith);
    camera->setEye(getPosition() + normal * dist);
    setPosition(camera->getEye());
    getBoundingBox()->setPosition(camera->getEye() - glm::vec3(0,1.0f,0));

    //If on flat ground, jumping is done. 
    if(normal.y > 0.5) {
        if(jumping)
            LoadManager::getSound("jump_land.wav")->playSound();
        velocity = 0;
        jumping = false;
    }
	
    break;
  case 16:
    INFO("DETECTING COLOR CHANGE!");
    colorMask = ((ColorChange *)collidedWith)->getColor();
    if(colorMask & BLUE)
      gun->setMaterial(MaterialManager::getMaterial("FlatBlue"));
    else if(colorMask & GREEN)
      gun->setMaterial(MaterialManager::getMaterial("FlatGreen"));
    else if(colorMask & RED)
      gun->setMaterial(MaterialManager::getMaterial("FlatRed"));
    else if(colorMask & GREY)
      gun->setMaterial(MaterialManager::getMaterial("FlatGrey"));
    break;
  default:
	break;
  }

}


bool Player::isKeyPressed(unsigned int key){
    return glfwGetKey(Global::window, key) == GLFW_PRESS;
}

