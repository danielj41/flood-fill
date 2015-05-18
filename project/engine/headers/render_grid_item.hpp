#ifndef RENDER_GRID_ITEM_HPP
#define RENDER_GRID_ITEM_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "plane.hpp"
#include "object.hpp"
#include "render_element.hpp"

#include "pointer_macros.h"
DEF_PTR(RenderGridItem, RenderGridItemPtr);

class RenderGridItem {
    public:
        PlanePtr planes[6];
        ObjectPtr object;
        RenderElementPtr renderElement;
        bool dirty;

        bool isEqual(RenderGridItem *other);
};

#endif
