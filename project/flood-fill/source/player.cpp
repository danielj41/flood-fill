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
#include "fluid_projectile.hpp"
#include "director.hpp"
#include "collision_manager.hpp"
#include "load_manager.hpp"
#include "material_manager.hpp"
#include "object.hpp"
#include "render_engine.hpp"

Player::Player(Camera * _camera)
  : GameObject(), CollisionObject(), camera(_camera),
    jumping(false), velocity(0), gravity(-2){}

void Player::setup() {
    INFO("Player Setup...");

	lastPosition = camera->getEye();
	
    setCollisionID(2);
    setCollideWithID(1 | 64 | 128);
	setCanCollide(true);
	
	INFO("Can Collide: " << canCollide());
    camera->fix(false, true, false);

    shootPressed = false;

    setBoundingBox(BoundingBox(glm::vec3(0.8f,0.8f,0.8f), glm::vec3(-0.8f,-0.8f,-0.8f)));
    getBoundingBox()->setPosition(camera->getEye() - glm::vec3(0,1.0f,0));

    sky = new Object(
        LoadManager::getMesh("sphere.obj"),
        MaterialManager::getMaterial("None"));

    sky->applyTexture(LoadManager::getTexture("Sky"));
    sky->enableTexture();
    sky->scale(glm::vec3(-50.0f,-50.0f,-50.0f));
    sky->translate(getPosition());
    RenderEngine::getRenderElement("textured")->addObject(sky);

    gun = new Object(
        LoadManager::getMesh("cube.obj"),
        MaterialManager::getMaterial("FlatBlue"));
    gun->loadIdentity();
    gun->scale(glm::vec3(0.07f, 0.07f, 0.3f));
    gun->rotate(15.0f, glm::vec3(1.0f, 1.0f, 0.0f));
    gun->translate(glm::vec3(0.27f, -0.26f, -0.4f));
    RenderEngine::getRenderElement("camera")->addObject(gun);


    hand = new PlayerHand(getPosition());
    hand->setup();
    Director::getScene()->addGameObject(hand);
    CollisionManager::addCollisionObjectToList(hand);



    shootTimer = -1.0f;
}

void Player::update() {
    lastPosition = camera->getEye();

    if(glfwGetKey(Global::window, GLFW_KEY_SPACE) == GLFW_PRESS && !jumping){
        velocity = .6;
        camera->jump(velocity * 25.0 * TimeManager::getDeltaTime());
    }
    jumping = true;

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

    if(glfwGetMouseButton(Global::window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && !shootPressed && shootTimer < 0.0f){
        FluidProjectile *fluidProjectile = new FluidProjectile(
            camera->getEye() - (0.35f * camera->getStrafeVector()) + glm::vec3(0.0f, 0.5f, 0.0f), -glm::normalize(camera->getViewVector()));
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

void Player::collided(CollisionObject * collidedWith){
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
        velocity = 0;
        jumping = false;
    }
	
    break;
  default:
	break;
  }

}

