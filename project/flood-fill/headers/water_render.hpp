#ifndef WATER_POLYGONS_RENDER_HPP
#define WATER_POLYGONS_RENDER_HPP

#include "render_element.hpp"

#include "pointer_macros.h"
DEF_PTR(WaterRender, WaterRenderPtr);

class WaterRender : public RenderElement{

public:
    WaterRender();

    void loadShader();

    void setupEnviroment();
    void tearDownEnviroment();

    void setupShader();
    void setupMesh(Mesh * mesh);
    void renderObject(Object * object);

private:
    static const std::string VERTEX_SHADER_FILE;
    static const std::string FRAGMENT_SHADER_FILE;
};

#endif
