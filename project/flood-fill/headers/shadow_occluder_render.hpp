#ifndef SHADOW_OCCLUDER_RENDER_HPP
#define SHADOW_OCCLUDER_RENDER_HPP

#include "render_element.hpp"
#include "fbo.hpp"
#include "light.hpp"

class ShadowOccluderRender : public RenderElement{

public:
    ShadowOccluderRender();

    void setup();
    void loadShader();

    void setupEnviroment();
    void tearDownEnviroment();

    void setupShader();
    void setupMesh(Mesh * mesh);
    void renderObject(Object * object);

    FBO * getFBO();

private:
    static const std::string VERTEX_SHADER_FILE;
    static const std::string FRAGMENT_SHADER_FILE;

    FBO * fbo;
};

#endif
