#ifndef WATER_PARTICLE_RENDER_HPP
#define WATER_PARTICLE_RENDER_HPP

#include "render_element.hpp"

#include "pointer_macros.h"
DEF_PTR(WaterParticleRender, WaterParticleRenderPtr);

class WaterParticleRender : public RenderElement{

public:
    WaterParticleRender();

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
