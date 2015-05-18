#ifndef DEBUG_RENDER_HPP
#define DEBUG_RENDER_HPP

#include "render_element.hpp"
#include "bounding_box.hpp"

#include "pointer_macros.h"
DEF_PTR(DebugRender, DebugRenderPtr);

class DebugRender : public RenderElement{

public:
    DebugRender();

    void loadShader();

    void setupEnviroment();
    void tearDownEnviroment();

    void setupShader();
    void setupMesh(MeshPtr mesh);
    void renderObject(ObjectPtr object);

    void renderPass();

    void addBoundingBox(BoundingBoxPtr box);
    void removeBoundingBox(BoundingBoxPtr box);

    void renderBoundingBox(BoundingBoxPtr box);

private:
    static const std::string VERTEX_SHADER_FILE;
    static const std::string FRAGMENT_SHADER_FILE;

    std::list<BoundingBoxPtr> boxes;
};

#endif
