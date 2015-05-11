#ifndef REGULAR_POLYGONS_RENDER_HPP
#define REGULAR_POLYGONS_RENDER_HPP

#include "render_element.hpp"
#include "uniform_3d_grid.hpp"

class RegularPolygonsRender : public RenderElement{

public:
    RegularPolygonsRender(Uniform3DGrid<Object *> _grid);

    void loadShader();

    void setupEnviroment();
    void tearDownEnviroment();

    void setupShader();
    void setupMesh(Mesh * mesh);
    void renderObject(Object * object);

    void addToGrid(Object * object);
    void removeFromGrid(Object * object);

    void renderPass();

private:
    static const std::string VERTEX_SHADER_FILE;
    static const std::string FRAGMENT_SHADER_FILE;

    Uniform3DGrid<Object *> grid;
};

#endif
