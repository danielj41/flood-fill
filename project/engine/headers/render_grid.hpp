#ifndef RENDER_GRID_HPP
#define RENDER_GRID_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"
#include "uniform_3d_grid.hpp"
#include "render_grid_item.hpp"

class RenderGrid {

public:

  Box(glm::vec3 _position, glm::vec3 _movementDirection, float _speed);

  void addObject(Object *object, RenderElement *renderElement);
  void removeObject(Object *object);

private:

  Uniform3DGrid<RenderGridItem> grid;
  bool dirty;

};

#endif
