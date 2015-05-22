#include "render_grid_item.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

bool RenderGridItem::isEqual(RenderGridItem *other) {
  return object != NULL_PTR &&
         renderElement == other->renderElement &&
         object->getMaterial() == other->object->getMaterial() &&
         object->getShearX() == other->object->getShearX() && object->getShearZ() == other->object->getShearZ() &&
         object->getTexture() == other->object->getTexture();
}