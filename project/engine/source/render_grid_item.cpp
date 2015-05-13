#include "render_grid_item.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

bool RenderGridItem::isEqual(RenderGridItem *other) {
  return object != NULL && renderElement == other->renderElement && object->getMaterial() == other->object->getMaterial();
}