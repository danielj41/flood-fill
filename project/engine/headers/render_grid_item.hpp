#ifndef RENDER_GRID_ITEM_HPP
#define RENDER_GRID_ITEM_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "plane.hpp"
#include "object.hpp"
#include "render_element.hpp"

class RenderGridItem {
    public:
        Plane *planes[6];
        Object *object;
        RenderElement *renderElement;
        bool dirty;

        bool isEqual(RenderGridItem *other);
};

#endif
