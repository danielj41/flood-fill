#ifndef NORMAL_MAP_BORDER_RENDER_HPP
#define NORMAL_MAP_BORDER_RENDER_HPP

#include "render_element.hpp"

#include "pointer_macros.h"
DEF_PTR(NormalMapBorderRender, NormalMapBorderRenderPtr);

class NormalMapBorderRender : public RenderElement{

public:
    NormalMapBorderRender();

    void loadShader();

    void setupEnviroment();
    void tearDownEnviroment();

    void setupShader();
    void setupMesh(Mesh* mesh);
    void renderObject(ObjectPtr object);

    void addObject(ObjectPtr object);
    void removeObject(ObjectPtr object);

private:
    static const std::string VERTEX_SHADER_FILE;
    static const std::string FRAGMENT_SHADER_FILE;
};

#endif
