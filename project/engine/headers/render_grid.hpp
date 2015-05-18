#ifndef RENDER_GRID_HPP
#define RENDER_GRID_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "object.hpp"
#include "uniform_3d_grid.hpp"
#include "render_grid_item.hpp"

#include "pointer_macros.h"
DEF_PTR(RenderGrid, RenderGridPtr);

class RenderGrid {

public:

  RenderGrid(unsigned int numberOfCubesInX,
             unsigned int numberOfCubesInY,
             unsigned int numberOfCubesInZ,
             float _minX, float _maxX,
             float _minY, float _maxY,
             float _minZ, float _maxZ);

  void initialize();

  void addObject(ObjectPtr object, RenderElementPtr renderElement);
  void removeObject(ObjectPtr object);

  void clean();  

private:
  Uniform3DGrid<RenderGridItem> grid;
  bool dirty;

  void createPlane(int x, int y, int z, int dir);
  void removePlane(PlanePtr plane);
  void dirtyAdjacent(glm::vec4 pos);
  void dirtyCell(glm::vec4 pos);
};

#endif
