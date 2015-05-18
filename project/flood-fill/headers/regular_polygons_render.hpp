#ifndef REGULAR_POLYGONS_RENDER_HPP
#define REGULAR_POLYGONS_RENDER_HPP

#include "render_element.hpp"
#include "uniform_3d_grid.hpp"

#include "pointer_macros.h"
DEF_PTR(RegularPolygonsRender, RegularPolygonsRenderPtr);

class RegularPolygonsRender : public RenderElement{

public:
    RegularPolygonsRender();

    void loadShader();

    void setupEnviroment();
    void tearDownEnviroment();

    void setupShader();
    void setupMesh(MeshPtr mesh);
    void renderObject(ObjectPtr object);

private:
    static const std::string VERTEX_SHADER_FILE;
    static const std::string FRAGMENT_SHADER_FILE;
};

#endif
