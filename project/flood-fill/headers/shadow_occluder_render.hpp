#ifndef SHADOW_OCCLUDER_RENDER_HPP
#define SHADOW_OCCLUDER_RENDER_HPP

#include "render_element.hpp"
#include "fbo.hpp"
#include "light.hpp"

#include "pointer_macros.h"
DEF_PTR(ShadowOccluderRender, ShadowOccluderRenderPtr);

class ShadowOccluderRender : public RenderElement{

public:
    ShadowOccluderRender();

    void setup();
    void loadShader();

    void setupEnviroment();
    void tearDownEnviroment();

    void setupShader();
    void setupMesh(MeshPtr mesh);
    void renderObject(ObjectPtr object);

    FBOPtr getFBO();

private:
    static const std::string VERTEX_SHADER_FILE;
    static const std::string FRAGMENT_SHADER_FILE;

    FBOPtr fbo;
};

#endif
