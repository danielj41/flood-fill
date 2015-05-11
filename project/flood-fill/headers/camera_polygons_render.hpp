#ifndef CAMERA_POLYGONS_RENDER_HPP
#define CAMERA_POLYGONS_RENDER_HPP

#include "render_element.hpp"

class CameraPolygonsRender : public RenderElement{

public:
    CameraPolygonsRender();

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
