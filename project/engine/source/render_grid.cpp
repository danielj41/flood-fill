#include "render_grid.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#include "load_manager.hpp"
#include "time_manager.hpp"
#include "collision_manager.hpp"
#include "director.hpp"
#include "material_manager.hpp"
#include "render_engine.hpp"

#define XP 0
#define YP 1
#define ZP 2
#define XN 3
#define YN 4
#define ZN 5

RenderGrid::RenderGrid() {}

void RenderGrid::initialize() {
  dirty = false;
  RenderGridItem item;
  item.object = NULL;
  item.planes[0] = NULL;
  item.planes[1] = NULL;
  item.planes[2] = NULL;
  item.planes[3] = NULL;
  item.planes[4] = NULL;
  item.planes[5] = NULL;
  item.renderElement = NULL;
  item.dirty = true;
  grid.initialize(item);
}

void RenderGrid::createPlane(int x, int y, int z, int dir) {
  RenderGridItem center = grid(x, y, z);
  int left = x, right = x, top = y, bottom = y, front = z, back = z, i, j, k;
  int xN, yN, zN;
  bool checkN = true;

  if(center.planes[dir] != NULL) {
    return;
  }

  if(dir == XP) {
    xN++;
  }
  if(dir == XN) {
    xN--;
  }
  if(dir == YP) {
    yN++;
  }
  if(dir == YN) {
    yN--;
  }
  if(dir == ZP) {
    zN++;
  }
  if(dir == ZN) {
    zN--;
  }
  if(!grid.inGrid(xN, yN, zN)) {
    checkN = false;
  }

  if(checkN && grid(x + xN, y + yN, z + zN).object != NULL) {
    return;
  }

  if(dir != XDIR) {
    for(left = x; left >= 0; left--) {
      for(j = bottom; j <= top; j++) {
        for(k = back; k <= front; k++) {
          if(!grid(left, j, k).isEqual(center) || grid(left, j, k).planes[dir] != NULL || checkN && grid(left + xN, j + yN, k + zN) != NULL) {
            break;
          }
        }
      }
    }
    left++;

    for(right = x; right < grid.getSizeX(); right++) {
      for(j = bottom; j <= top; j++) {
        for(k = back; k <= front; k++) {
          if(!grid(right, j, k).isEqual(center) || grid(left, j, k).planes[dir] != NULL || checkN && grid(right + xN, j + yN, k + zN) != NULL) {
            break;
          }
        }
      }
    }
    right--;
  }
  
  if(dir != YDIR) {
    for(bottom = y; bottom >= 0; bottom--) {
      for(i = left; i <= right; i++) {
        for(k = back; k <= front; k++) {
          if(!grid(i, bottom, k).isEqual(center) || grid(left, j, k).planes[dir] != NULL || checkN && grid(i + xN, bottom + yN, k + zN) != NULL) {
            break;
          }
        }
      }
    }
    bottom++;

    for(top = y; top < grid.getSizeY(); top++) {
      for(i = left; i <= right; i++) {
        for(k = back; k <= front; k++) {
          if(!grid(i, top, k).isEqual(center) || grid(left, j, k).planes[dir] != NULL || checkN && grid(i + xN, top + yN, k + zN) != NULL) {
            break;
          }
        }
      }
    }
    top--;
  }

  if(dir != ZDIR) {
    for(back = z; back >= 0; back--) {
      for(i = left; i <= right; i++) {
        for(j = bottom; j <= top; j++) {
          if(!grid(i, j, back).isEqual(center) || grid(left, j, k).planes[dir] != NULL || checkN && grid(i + xN, j + yN, back + zN) != NULL) {
            break;
          }
        }
      }
    }
    back++;

    for(front = z; front < grid.getSizeZ(); front++) {
      for(i = left; i <= right; i++) {
        for(j = bottom; j <= top; j++) {
          if(!grid(i, j, front).isEqual(center) || grid(left, j, k).planes[dir] != NULL || checkN && grid(left + xN, j + yN, front + zN) != NULL) {
            break;
          }
        }
      }
    }
    front--;
  }

  Plane *plane = new Plane(left, right, bottom, top, back, front, direction, x, y, z, &grid, center.object);
  plane->setup();

  for(i = left; i <= front; i++) {
    for(j = bottom; j <= top; j++) {
      for(k = back; k <= front; k++) {
        grid(i, j, k).planes[dir] = plane;
      }
    }
  }

  center.renderElement.add(plane->getObject());
}

void RenderGrid::addObject(Object *object, RenderElement *renderElement) {
  glm::vec4 pos = object->getModelMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  RenderGridItem item;
  item.object = object;
  item.planes[0] = NULL;
  item.planes[1] = NULL;
  item.planes[2] = NULL;
  item.planes[3] = NULL;
  item.planes[4] = NULL;
  item.planes[5] = NULL;
  item.renderElement = renderElement;
  item.dirty = true;
  grid.setValue(pos.x, pos.y, pos.z, item);
  dirtyAdjacent(pos);
}

void RenderGrid::removeObject(Object *object) {
  glm::vec4 pos = object->getModelMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  RenderGridItem item;
  item.object = NULL;
  item.renderElement = NULL;
  item.dirty = true;
  grid.setValue(pos.x, pos.y, pos.z, item);
  dirtyAdjacent(pos);
}

void RenderGrid::dirtyAdjacent(glm::vec4 pos) {
  dirtyCell(pos);
  dirtyCell(pos + glm::vec4(grid.getEdgeSizeX(), 0.0f, 0.0f, 0.0f));
  dirtyCell(pos + glm::vec4(-grid.getEdgeSizeX(), 0.0f, 0.0f, 0.0f));
  dirtyCell(pos + glm::vec4(0.0f, grid.getEdgeSizeY(), 0.0f, 0.0f));
  dirtyCell(pos + glm::vec4(0.0f, -grid.getEdgeSizeY(), 0.0f, 0.0f));
  dirtyCell(pos + glm::vec4(0.0f, 0.0f, grid.getEdgeSizeZ(), 0.0f));
  dirtyCell(pos + glm::vec4(0.0f, 0.0f, -grid.getEdgeSizeZ(), 0.0f));
}

void RenderGrid::dirtyCell(glm::vec4 pos) {
  RenderGridItem item = grid.getValue(pos.x, pos.y, pos.z);
  int i;
  for(i = 0; i < 6; i++) {
    removePlane(item.planes[i]);
  }
  dirty = true;
}

void RenderGrid::clean() {
  if(dirty) {
    int i, j, k;
    dirty = false;

    for(i = 0; i < grid.getSizeX(); i++) {
      for(j = 0; j < grid.getSizeY(); j++) {
        for(k = 0; k < grid.getSizeZ(); k++) {
          if(grid(i, j, k).dirty) {
            if(grid(i, j, k).object != NULL) {
              createPlane(i, j, k, XP);
              createPlane(i, j, k, XN);
              createPlane(i, j, k, YP);
              createPlane(i, j, k, YN);
              createPlane(i, j, k, ZP);
              createPlane(i, j, k, ZN);
            }
            grid(i, j, k).dirty = false;
          }
        }
      }
    }
  }
}

void RenderGrid::removePlane(Plane *plane) {
  int i, j, k;
  grid(plane->getCenterX(), plane->getCenterY(), plane->getCenterZ()).renderElement->removeObject(plane);
  for(i = plane->getLeft(); i <= plane->getRight(); i++) {
    for(j = plane->getBottom(); j <= plane->getTop(); j++) {
      for(k = plane->getBack(); k <= plane->getFront(); k++) {
        grid(i, j, k).plane[plane->getDirection()] = NULL;
        grid(i, j, k).dirty = true;
      }
    }
  }
}

