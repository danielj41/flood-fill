#include "action_invisible_block.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#include "load_manager.hpp"
#include "collision_manager.hpp"
#include "material_manager.hpp"
#include "render_engine.hpp"
#include "debug_render.hpp"

ActionInvisibleBlock::ActionInvisibleBlock(glm::vec3 _position) : GameObject(), CollisionObject(_position),
    position(_position) {}

void ActionInvisibleBlock::setup() {
    INFO("Creating a Action Block ...");

    setCollisionIDs();
    setCanCollide(true);

    setBoundingBox(BoundingBox(glm::vec3(1.0f,1.0f,1.0f), glm::vec3(-1.0f,-1.0f,-1.0f)));
    getBoundingBox()->setPosition(position);

    PTR_CAST(DebugRender, RenderEngine::getRenderElement("debug"))->addBoundingBox(getBoundingBox());
}

void ActionInvisibleBlock::update(){
}

void ActionInvisibleBlock::collided(CollisionObjectPtr collidedWith){
  switch (collidedWith->getCollisionID()) {
  case 2:
    doAction();
    break;
  }
}
