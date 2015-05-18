#ifndef NORMAL_MAP_RENDER_HPP
#define NORMAL_MAP_RENDER_HPP

#include "render_element.hpp"

#include "pointer_macros.h"
DEF_PTR(NormalMapRender, NormalMapRenderPtr);

class NormalMapRender : public RenderElement{

public:
    NormalMapRender();

    void loadShader();

    void setupEnviroment();
    void tearDownEnviroment();

    void setupShader();
    void setupMesh(MeshPtr mesh);
    void renderObject(ObjectPtr object);

    void addObject(ObjectPtr object);
    void removeObject(ObjectPtr object);

private:
    static const std::string VERTEX_SHADER_FILE;
    static const std::string FRAGMENT_SHADER_FILE;
};

#endif
