#ifndef PLANE_HPP
#define PLANE_HPP

#define GLM_FORCE_RADIANS
#define GLM_FORCE_PURE
#include "glm/glm.hpp"

#include "object.hpp"
#include "uniform_3d_grid.hpp"

class Plane {
    public:

        Plane(int _left, int _right,
              int _bottom, int _top,
              int _back, int _front,
              int _direction,
              int _centerX, int _centerY, int _centerZ,
              Uniform3DGrid<RenderGridItem>* _grid,
              Object *_refObject);

        void setup();

        int getLeft();
        int getRight();
        int getBottom();
        int getTop();
        int getBack();
        int getFront();
        int getCenterX();
        int getCenterY();
        int getCenterZ();
        int getDirection();
        Object *getObject();

    private:
        Object *object, *refObject;
        int left, right, bottom, top, back, front, direction, centerX, centerY, centerZ;
        Uniform3DGrid<RenderGridItem>* grid;
}

#endif
