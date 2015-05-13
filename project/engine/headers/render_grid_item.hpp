#ifndef RENDER_GRID_ITEM_HPP
#define RENDER_GRID_ITEM_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "game_object.hpp"
#include "collision_object.hpp"
#include "object.hpp"
#include "uniform_3d_grid.hpp"

class RenderGridItem {
    public:
        Plane *planes[6];
        Object *object;
        RenderElement *renderElement;
        bool dirty;

        bool isEqual(RenderGridItem *other);
}

#endif
