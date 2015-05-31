#include "render_grid.hpp"

#include <cstdlib>
#include <iostream>
#include "debug_macros.h"

#define XP 0
#define YP 1
#define ZP 2
#define XN 3
#define YN 4
#define ZN 5

RenderGrid::RenderGrid(unsigned int numberOfCubesInX,
                       unsigned int numberOfCubesInY,
                       unsigned int numberOfCubesInZ,
                       float _minX, float _maxX,
                       float _minY, float _maxY,
                       float _minZ, float _maxZ) {

  grid = Uniform3DGridPtr<RenderGridItem>(
         new Uniform3DGrid<RenderGridItem>(numberOfCubesInX, numberOfCubesInY, numberOfCubesInZ,
                                           _minX, _maxX, _minY, _maxY, _minZ, _maxZ));
}

void RenderGrid::initialize() {
  dirty = false;
  RenderGridItem item;
  item.object = NULL_PTR;
  item.planes[0] = NULL_PTR;
  item.planes[1] = NULL_PTR;
  item.planes[2] = NULL_PTR;
  item.planes[3] = NULL_PTR;
  item.planes[4] = NULL_PTR;
  item.planes[5] = NULL_PTR;
  item.renderElement = NULL_PTR;
  item.dirty = true;
  grid->initialize(item);
}

void RenderGrid::createPlane(int x, int y, int z, int dir) {
  RenderGridItem center = (*grid)(x, y, z);
  int left = x, right = x, top = y, bottom = y, front = z, back = z, i, j, k;
  int xN = 0, yN = 0, zN = 0;
  bool checkN = true, done = false;

  if(center.planes[dir] != NULL_PTR) {
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

  int gridSizeX = grid->getSizeX();
  int gridSizeY = grid->getSizeY();
  int gridSizeZ = grid->getSizeZ();

  checkN = x + xN >= 0 && x + xN < gridSizeX &&
           y + yN >= 0 && y + yN < gridSizeY &&
           z + zN >= 0 && z + zN < gridSizeZ;

  if(checkN && (*grid)(x + xN, y + yN, z + zN).object != NULL_PTR) {
    return;
  }

  if(dir != XN && dir != XP) {
    done = false;
    for(left = x - 1; left >= 0 && !done; left--) {
      for(j = bottom; j <= top; j++) {
        for(k = back; k <= front; k++) {
          if(!(*grid)(left, j, k).isEqual(&center) || (*grid)(left, j, k).object == NULL_PTR|| (*grid)(left, j, k).planes[dir] != NULL_PTR|| (checkN && (*grid)(left + xN, j + yN, k + zN).object != NULL_PTR)) {
            done = true;
            left++;
          }
        }
      }
    }
    left++;

    done = false;
    for(right = x + 1; right < gridSizeX && !done; right++) {
      for(j = bottom; j <= top; j++) {
        for(k = back; k <= front; k++) {
          if(!(*grid)(right, j, k).isEqual(&center) || (*grid)(right, j, k).object == NULL_PTR || (*grid)(right, j, k).planes[dir] != NULL_PTR || (checkN && (*grid)(right + xN, j + yN, k + zN).object != NULL_PTR)) {
            done = true;
            right--;
          }
        }
      }
    }
    right--;
  }
  
  if(dir != YN && dir != YP) {
    done = false;
    for(bottom = y - 1; bottom >= 0 && !done; bottom--) {
      for(i = left; i <= right; i++) {
        for(k = back; k <= front; k++) {
          if(!(*grid)(i, bottom, k).isEqual(&center) || (*grid)(i, bottom, k).object == NULL_PTR || (*grid)(i, bottom, k).planes[dir] != NULL_PTR || (checkN && (*grid)(i + xN, bottom + yN, k + zN).object != NULL_PTR)) {
            done = true;
            bottom++;
          }
        }
      }
    }
    bottom++;

    done = false;
    for(top = y + 1; top < gridSizeY && !done; top++) {
      for(i = left; i <= right; i++) {
        for(k = back; k <= front; k++) {
          if(!(*grid)(i, top, k).isEqual(&center) || (*grid)(i, top, k).object == NULL_PTR || (*grid)(i, top, k).planes[dir] != NULL_PTR || (checkN && (*grid)(i + xN, top + yN, k + zN).object != NULL_PTR)) {
            done = true;
            top--;
          }
        }
      }
    }
    top--;
  }

  if(dir != ZN && dir != ZP) {
    done = false;
    for(back = z - 1; back >= 0 && !done; back--) {
      for(i = left; i <= right; i++) {
        for(j = bottom; j <= top; j++) {
          if(!(*grid)(i, j, back).isEqual(&center) || (*grid)(i, j, back).object == NULL_PTR || (*grid)(i, j, back).planes[dir] != NULL_PTR || (checkN && (*grid)(i + xN, j + yN, back + zN).object != NULL_PTR)) {
            done = true;
            back++;
          }
        }
      }
    }
    back++;

    done = false;
    for(front = z + 1; front < gridSizeZ && !done; front++) {
      for(i = left; i <= right; i++) {
        for(j = bottom; j <= top; j++) {
          if(!(*grid)(i, j, front).isEqual(&center) || (*grid)(i, j, front).object == NULL_PTR || (*grid)(i, j, front).planes[dir] != NULL_PTR || (checkN && (*grid)(i + xN, j + yN, front + zN).object != NULL_PTR)) {
            done = true;
            front--;
          }
        }
      }
    }
    front--;
  }

  PlanePtr plane(new Plane(left, right, bottom, top, back, front, dir, x, y, z,
                           grid->getMinX(), grid->getMaxY(), grid->getMinZ(),
                           grid->getEdgeSizeX(), grid->getEdgeSizeY(), grid->getEdgeSizeZ(),
                           center.object));
  plane->setup();

  if(center.facingFillSpaces.find(dir) != center.facingFillSpaces.end())
    plane->getObject()->setTextureAndNormalMapPack(1);

  for(i = left; i <= right; i++) {
    for(j = bottom; j <= top; j++) {
      for(k = back; k <= front; k++) {
        (*grid)(i, j, k).planes[dir] = plane;
      }
    }
  }

  center.renderElement->addObject(plane->getObject());
}

void RenderGrid::addObject(ObjectPtr object, RenderElementPtr renderElement, std::set<int> facingFillSpaces) {
  glm::vec4 pos = object->getModelMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  RenderGridItem item;
  item.object = object;
  item.facingFillSpaces = facingFillSpaces;
  item.planes[0] = NULL_PTR;
  item.planes[1] = NULL_PTR;
  item.planes[2] = NULL_PTR;
  item.planes[3] = NULL_PTR;
  item.planes[4] = NULL_PTR;
  item.planes[5] = NULL_PTR;
  item.renderElement = renderElement;
  item.dirty = true;
  grid->setValue(pos.x, pos.y, pos.z, item);
  dirtyAdjacent(pos);
}

void RenderGrid::removeObject(ObjectPtr object) {
  glm::vec4 pos = object->getModelMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
  dirtyAdjacent(pos);
  RenderGridItem item = grid->getValue(pos.x, pos.y, pos.z);
  item.object = NULL_PTR;
  item.renderElement = NULL_PTR;
  item.dirty = true;
  grid->setValue(pos.x, pos.y, pos.z, item);
}

void RenderGrid::dirtyAdjacent(glm::vec4 pos) {
  int i, j, k;
  for(i = -1; i <= 1; i++) {
    for(j = -1; j <= 1; j++) {
      for(k = -1; k <= 1; k++) {
        dirtyCell(pos + glm::vec4(i * grid->getEdgeSizeX(), j * grid->getEdgeSizeY(), k * grid->getEdgeSizeZ(), 0.0f));
      }
    }
  }
  dirtyCell(pos + glm::vec4(grid->getEdgeSizeX(), 0.0f, 0.0f, 0.0f));
  dirtyCell(pos + glm::vec4(-grid->getEdgeSizeX(), 0.0f, 0.0f, 0.0f));
  dirtyCell(pos + glm::vec4(0.0f, grid->getEdgeSizeY(), 0.0f, 0.0f));
  dirtyCell(pos + glm::vec4(0.0f, -grid->getEdgeSizeY(), 0.0f, 0.0f));
  dirtyCell(pos + glm::vec4(0.0f, 0.0f, grid->getEdgeSizeZ(), 0.0f));
  dirtyCell(pos + glm::vec4(0.0f, 0.0f, -grid->getEdgeSizeZ(), 0.0f));
}

void RenderGrid::dirtyCell(glm::vec4 pos) {
  if(grid->inGrid(pos.x, pos.y, pos.z)) {
    RenderGridItem item = grid->getValue(pos.x, pos.y, pos.z);
    item.dirty = true;
    grid->setValue(pos.x, pos.y, pos.z, item);
    int i;
    for(i = 0; i < 6; i++) {
      removePlane(item.planes[i]);
    }
    dirty = true;
  }
}

void RenderGrid::clean() {
  if(dirty) {
    unsigned int i, j, k;
    dirty = false;

    for(i = 0; i < grid->getSizeX(); i++) {
      for(j = 0; j < grid->getSizeY(); j++) {
        for(k = 0; k < grid->getSizeZ(); k++) {
          if((*grid)(i, j, k).dirty) {
            if((*grid)(i, j, k).object != NULL_PTR) {
              createPlane(i, j, k, XP);
              createPlane(i, j, k, XN);
              createPlane(i, j, k, YP);
              createPlane(i, j, k, YN);
              createPlane(i, j, k, ZP);
              createPlane(i, j, k, ZN);
            }
            (*grid)(i, j, k).dirty = false;
          }
        }
      }
    }
  }
}

void RenderGrid::removePlane(PlanePtr plane) {
  if(plane == NULL_PTR) {
    return;
  }
  int i, j, k;
  (*grid)(plane->getCenterX(), plane->getCenterY(), plane->getCenterZ()).renderElement->removeObject(plane->getObject());
  for(i = plane->getLeft(); i <= plane->getRight(); i++) {
    for(j = plane->getBottom(); j <= plane->getTop(); j++) {
      for(k = plane->getBack(); k <= plane->getFront(); k++) {
        (*grid)(i, j, k).planes[plane->getDirection()] = NULL_PTR;
        (*grid)(i, j, k).dirty = true;
      }
    }
  }
}

