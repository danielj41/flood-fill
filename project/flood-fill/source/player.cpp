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
#include "winning_block.hpp"
#include "level_template.hpp"
#include "fluid_box.hpp"
#include "solid_cube.hpp"
#include "level_manager.hpp"
#include "menu.hpp"

#define BLUE    1
#define GREEN   2
#define RED     4
#define GREY    8

Player::Player(CameraPtr _camera, int _initialColor)
  : GameObject(), CollisionObject(), camera(_camera),
    jumping(true), velocity(0), gravity(-2), strafeVelocity(0), forwardVelocity(0),
    jumpMultiplier(1), moveMultiplier(1), ceilingFrame(0), initialColor(_initialColor) {
    removeFluidShootRange = 3;
    removeFluidNumberBlocks = 3;
}

void Player::setup() {
    INFO("Player Setup...");

    active = false;

	lastPosition = camera->getEye();
	
    setCollisionID(2);
    setCollideWithID(1 | 16 | 32 | 64 | 128 | 256);

	setCanCollide(true);
	
	INFO("Can Collide: " << canCollide());
    camera->fix(false, true, false);

    shootPressed = false;

    walkAmount = 0.0f;
    eyeOffset = 0.7f;

    setBoundingBox(BoundingBox(glm::vec3(0.8f,0.8f,0.8f), glm::vec3(-0.8f,-0.8f,-0.8f)));
    getBoundingBox()->setPosition(camera->getEye() - glm::vec3(0,eyeOffset,0));

    LoadManager::loadSound("paintfill.wav");

    gun = ObjectPtr(new Object(
        LoadManager::getMesh("gun.obj"),
        MaterialManager::getMaterial("FlatBlue")));
    gun->loadIdentity();
    gun->scale(glm::vec3(0.5f));
    gun->rotate(180.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    gun->translate(glm::vec3(0.27f, -0.26f, -0.4f));
    gun->applyTexture(LoadManager::getTexture("GunTexture"));
    gun->enableTexture();
    RenderEngine::getRenderElement("camera")->addObject(gun);


    hand = PlayerHandPtr(new PlayerHand(getPosition(), gun, initialColor));
    hand->setup();
    Director::getScene()->addGameObject(hand);
    CollisionManager::addCollisionObjectToList(hand);

    shootTimer = 1.0f;
}

void Player::update() {

    if(!active) return;
    
    if (!Menu::isActive()) {
    float dt = TimeManager::getDeltaTime();
    if(dt > 0.04) {
        dt = 0.04; // prevent falling through blocks with low framerate
    }

    lastPosition = camera->getEye();

    if(isKeyPressed(GLFW_KEY_SPACE) && !jumping) {
        velocity = .6 * jumpMultiplier;
        camera->jump(velocity * 25.0 * dt);
    }
    jumping = true;
    
    addMultiplier = false; 
  
    if(isKeyPressed(GLFW_KEY_W)){
        forwardVelocity = fmin(forwardVelocity + 1.5 * dt , 0.25f - 0.07f * fabs(strafeVelocity));
    } else if(forwardVelocity > 0.0f) {
        forwardVelocity = fmax(forwardVelocity - 1.5 * dt , 0.0f);
    }
    if(isKeyPressed(GLFW_KEY_S)){
        forwardVelocity = fmax(forwardVelocity - 1.5 * dt , -0.25f + 0.07f * fabs(strafeVelocity));
    } else if(forwardVelocity < 0.0f) {
        forwardVelocity = fmin(forwardVelocity + 1.5 * dt , 0.0f);
    }
    if(isKeyPressed(GLFW_KEY_D)){
        strafeVelocity = fmin(strafeVelocity + 1.5 * dt , 0.25f - 0.07f * fabs(forwardVelocity));
    } else if(strafeVelocity > 0.0f) {
        strafeVelocity = fmax(strafeVelocity - 1.5 * dt , 0.0f);
    }
    else if(isKeyPressed(GLFW_KEY_A)){
        strafeVelocity = fmax(strafeVelocity - 1.5 * dt , -0.25f + 0.07f * fabs(forwardVelocity));
    } else if(strafeVelocity < 0.0f) {
        strafeVelocity = fmax(strafeVelocity - 1.5 * dt , 0.0f);
    }

    camera->zoom(Camera::FORWARD_DIRECTION, moveMultiplier * forwardVelocity * dt * 15.0f);
    camera->strafe(Camera::RIGHT_DIRECTION, moveMultiplier * strafeVelocity * dt * 15.0f);

    walkAmount += forwardVelocity * forwardVelocity + strafeVelocity * strafeVelocity;
    eyeOffset = 0.7 + 0.05f * sin(walkAmount * 2.0f);

    if(isKeyPressed(GLFW_KEY_Q)){
        if ( TimeManager::getTimeStamp() - hand->getToggleTime() > .2){
            hand->changeColorMask();
        }
    }

    
    if(jumping) {
        velocity += gravity * dt;
        if(velocity < -0.5) {
            velocity = -0.5;
        }
        camera->jump(velocity * 25.0 * dt);
    }

    getBoundingBox()->setPosition(camera->getEye() - glm::vec3(0,eyeOffset,0));
	setPosition(camera->getEye());

    if(glfwGetMouseButton(Global::window, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS && shootTimer < 0.0f){
        FluidProjectilePtr fluidProjectile(new FluidProjectile(
            camera->getEye() - (0.35f * camera->getStrafeVector()) + glm::vec3(0.0f, 0.5f, 0.0f),
            -glm::normalize(camera->getViewVector()),
            hand->getColorMask()));
        fluidProjectile->setup();
        Director::getScene()->addGameObject(fluidProjectile);
        CollisionManager::addCollisionObjectToList(fluidProjectile);
        shootPressed = true;
        shootTimer = 0.15f;

        LoadManager::getSound("paintfill.wav")->playSound();
    }
    if(glfwGetMouseButton(Global::window, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE) {
        shootPressed = false;
    }

    //Picks all fluid boxes that can be removed
    if(glfwGetMouseButton(Global::window, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS) {
        pickFluidBoxesToRemove();
    }
    //Try to remove the fluid boxes when you release the mouse left button
    else if(glfwGetMouseButton(Global::window, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE) {
        removeFluidBoxes();
    }

    if(shootTimer > 0.0f) {
        shootTimer -= TimeManager::getDeltaTime();
    }

    hand->setPosition(getBoundingBox()->getPosition() - 1.2f * camera->getViewVector());

    gun->loadIdentity();
    gun->scale(glm::vec3(0.5f));
    gun->rotate(180.0f + eyeOffset/4.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    gun->translate(glm::vec3(0.27f, -0.4f + eyeOffset/12.0f, -0.4f));

    if(ceilingFrame > 0) {
        ceilingFrame--;
    }
    }

    doCameraWarp();
}

void Player::setActive(bool _active) {
    active = _active;
}

void Player::collided(CollisionObjectPtr collidedWith) {
  glm::vec3 normal;
  float dist;
  switch (collidedWith->getCollisionID()) {
  case 32: 
      INFO("DETECTING COLLISION WITH SWITCH!");
      break;
  case 128:
      PTR_CAST(WinningBlock, collidedWith)->doAction();
      LevelManager::levelFinished = true;
      break;      
  case 1:
  case 64:
	INFO("DETECTING COLLISION WITH BLOCK!");
    normal = getCollisionNormal(collidedWith);
    dist = getCollisionDistance(collidedWith);
    camera->setEye(getPosition() + normal * dist);
    setPosition(camera->getEye());
    getBoundingBox()->setPosition(camera->getEye() - glm::vec3(0,eyeOffset,0));

    //Reseting multipliers from colored blocks
    if(!addMultiplier) {
        if (moveMultiplier > 1.0) {
            moveMultiplier -= .01;
        }
        jumpMultiplier = 1;
    }

    //If on flat ground, jumping is done. 
    if(normal.y > 0.5f) {
        velocity = 0;
        jumping = false;
    } else if (normal.y < -0.5f) {
        velocity = 0;
        // store that we touched a ceiling for 3 frames, that way all collisions
        // will have a second pass knowing that we collided with a ceiling, for
        // removing fluid boxes that will trap us into the ceiling
        ceilingFrame = 3;
    }
	
    break;
  case 16:
      //INFO("DETECTING COLOR CHANGE!");
      hand->setColorMask((PTR_CAST(ColorChange, collidedWith)->getColor()));
    
    break;
  case 256:
    if(PTR_CAST(FluidBox, collidedWith)->getColorMask() & BLUE) { //Blue filled
        jumpMultiplier = 2;
    } 
    else if(PTR_CAST(FluidBox, collidedWith)->getColorMask() & RED) { //Green filled ... idk why
        moveMultiplier = 2;
        jumpMultiplier = 2;
        activateCameraWarp = true;
    }
    else if(PTR_CAST(FluidBox, collidedWith)->getColorMask() & GREEN) {//Red filled ... trying to fix
        moveMultiplier = 2;
        activateCameraWarp = true;
    }
    addMultiplier = true;

    normal = getCollisionNormal(collidedWith);
    dist = getCollisionDistance(collidedWith);
    camera->setEye(getPosition() + normal * dist);
    setPosition(camera->getEye());
    getBoundingBox()->setPosition(camera->getEye() - glm::vec3(0,eyeOffset,0));

    //If on flat ground, jumping is done. 
    if(normal.y > 0.5f) {
        velocity = 0;
        jumping = false;
        if(ceilingFrame > 0) {
            PTR_CAST(FluidBox, collidedWith)->removeNow();
        }
    } else if (normal.y < -0.5f) {
        velocity = 0;
        ceilingFrame = 3;
        // store that we touched a ceiling for 3 frames, that way all collisions
        // will have a second pass knowing that we collided with a ceiling, for
        // removing fluid boxes that will trap us into the ceiling
    }
    break;
  default:
	break;
  }

}


bool Player::isKeyPressed(unsigned int key){
    return glfwGetKey(Global::window, key) == GLFW_PRESS;
}

void Player::pickOneFluidBoxToRemove(glm::vec3 pos){
    int currentColor = hand->getColorMask();

    if(PTR_CAST(LevelTemplate, Director::getScene())->isFilledWithPaint(pos)){
        FluidBoxPtr box = PTR_CAST(FluidBox, PTR_CAST(LevelTemplate, Director::getScene())->getGridValue(pos));

        //Checks if the player is holding the same color that is being removed
        if(box->getColorMask() & currentColor){
            box->highlightForRemotion();
            boxesToRemove.push_back(box);
        }
    }
}

void Player::pickFluidBoxesToRemove(){
    // Ray Casting into the grid to check which fluid boxes are around
    glm::vec3 p0 = camera->getEye();
    glm::vec3 direction = -glm::normalize(camera->getViewVector());

    for(unsigned int i = 0; i < boxesToRemove.size(); i++){
        boxesToRemove[i]->deselect();
    }

    boxesToRemove.clear();
    float gridCellSize = 2.0f;
    int nBlocks = removeFluidNumberBlocks;

    for(float i = 1.0f; i <= removeFluidShootRange; i+=0.2){
        glm::vec3 rayPos = p0 + direction*((float) i);

        INFO("Casting Ray to remove box: (" << rayPos.x << ", " << rayPos.y << ", " << rayPos.z << ")");

        if(!(PTR_CAST(LevelTemplate, Director::getScene()))->isEmpty(rayPos)){
            pickOneFluidBoxToRemove(rayPos);

            if(boxesToRemove.size() != 0){
                for(int bi = -nBlocks/2; bi <= nBlocks/2; bi++){
                    for(int bk = -nBlocks/2; bk <= nBlocks/2; bk++){
                        if(bi == bk && bi == 0) continue;

                        glm::vec3 newRay = rayPos;
                        newRay.x += bi * gridCellSize;
                        newRay.z += bk * gridCellSize;
                        pickOneFluidBoxToRemove(newRay);
                    }
                }
            }

            break;
        }
    }
}

void Player::removeFluidBoxes(){
    if(boxesToRemove.size() == 0) return;

    for(unsigned int i = 0; i < boxesToRemove.size(); i++){
        boxesToRemove[i]->remove();
    }
}

void Player::doCameraWarp(){
    if(forwardVelocity <= 0) activateCameraWarp = false;

    CameraPtr cam = Director::getScene()->getCamera();
    glm::mat4 P = glm::perspective(glm::radians(angleP),
                         (float) Global::ScreenWidth/Global::ScreenHeight,
                         0.1f, 100.f);
    cam->setProjectionMatrix(P);

    float angleVelocity = 100;
    if(activateCameraWarp){
        angleP += angleInc*angleVelocity*TimeManager::getDeltaTime();
        if(angleP >= anglePMax){
            angleP = anglePMax;
        }
    }
    else{
        angleP -= angleInc*angleVelocity*TimeManager::getDeltaTime();
        if(angleP <= anglePMin){
            angleP = anglePMin;
        }
    }

    activateCameraWarp = false;
}
