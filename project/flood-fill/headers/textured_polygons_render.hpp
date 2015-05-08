#ifndef TEXTURED_POLYGONS_RENDER_HPP
#define TEXTURED_POLYGONS_RENDER_HPP

#include "render_element.hpp"

class TexturedPolygonsRender : public RenderElement{

public:
    TexturedPolygonsRender();

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