#include "plane.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"
#include "load_manager.hpp"

#define XP 0
#define YP 1
#define ZP 2
#define XN 3
#define YN 4
#define ZN 5

Plane::Plane(int _left, int _right,
             int _bottom, int _top,
             int _back, int _front,
             int _direction,
             int _centerX, int _centerY, int _centerZ,
             float _minX, float _maxY, float _minZ,
             float _edgeX, float _edgeY, float _edgeZ,
             ObjectPtr _refObject) :
             left(_left), right(_right),
             bottom(_bottom), top(_top),
             back(_back), front(_front),
             direction(_direction),
             centerX(_centerX), centerY(_centerY), centerZ(_centerZ),
             minX(_minX), maxY(_maxY), minZ(_minZ),
             edgeX(_edgeX), edgeY(_edgeY), edgeZ(_edgeZ), 
             refObject(_refObject) {}

void Plane::setup() {
  object = ObjectPtr(new Object(LoadManager::getMesh("plane.obj"), refObject->getMaterial()));

  if(refObject->hasTexture()) {
    object->applyTexture(refObject->getTexture());
    object->enableTexture();
  }

  if(refObject->hasNormalMap()) {
    object->applyNormalMap(refObject->getNormalMap());
  }

  object->loadIdentity();

  object->translate(glm::vec3(0.0f, 0.0f, 1.0f));

  if(direction == XP) {
    object->rotate(90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    object->setGridScale(glm::vec2(front - back + 1, top - bottom + 1));
  } else if(direction == XN) {
    object->rotate(-90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
    object->setGridScale(glm::vec2(front - back + 1, top - bottom + 1));
  } else if(direction == YP) {
    object->rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    object->setGridScale(glm::vec2(right - left + 1, front - back + 1));
  } else if(direction == YN) {
    object->rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    object->setGridScale(glm::vec2(right - left + 1, front - back + 1));
  } else if(direction == ZP) {
    object->setGridScale(glm::vec2(right - left + 1, top - bottom + 1));
  } else if(direction == ZN) {
    object->rotate(180.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    object->setGridScale(glm::vec2(right - left + 1, top - bottom + 1));
  }
  
  object->scale(glm::vec3((right - left + 1) * edgeX / 2.0f,
                          (top - bottom + 1) * edgeY / 2.0f,
                          (front - back + 1) * edgeZ / 2.0f));
  object->translate(glm::vec3((right + left + 1.0f) * edgeX / 2.0f + minX,
                              -(top + bottom + 1.0f) * edgeY / 2.0f + maxY,
                              (back + front + 1.0f) * edgeZ / 2.0f + minZ));
}

int Plane::getLeft() { return left; }
int Plane::getRight() { return right; }
int Plane::getBottom() { return bottom; }
int Plane::getTop() { return top; }
int Plane::getBack() { return back; }
int Plane::getFront() { return front; }
int Plane::getCenterX() { return centerX; }
int Plane::getCenterY() { return centerY; }
int Plane::getCenterZ() { return centerZ; }
int Plane::getDirection() { return direction; }
ObjectPtr Plane::getObject() { return object; }
