#include "render_grid_item.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#include "load_manager.hpp"
#include "time_manager.hpp"
#include "collision_manager.hpp"
#include "director.hpp"
#include "material_manager.hpp"
#include "render_engine.hpp"

bool RenderGridItem::isEqual(RenderGridItem *other) {
  return object != NULL && renderElement == other->renderElement && object->getMaterial() == other->object->getMaterial();
}