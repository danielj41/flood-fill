#ifndef RENDER_ELEMENT_HPP
#define RENDER_ELEMENT_HPP

#include <map>
#include <list>

#include "shader.hpp"
#include "object.hpp"
#include "mesh.hpp"

#include "pointer_macros.h"
DEF_PTR(RenderElement, RenderElementPtr);

class RenderElement {

public:
    RenderElement(bool viewFrustumCullingEnable = true);

    virtual void addObject(ObjectPtr object);
    virtual void removeObject(ObjectPtr object);

    // Functions Called when you add the Render Element to the Render Engine
    // These function are called in this order:
    virtual void setup();
    virtual void loadShader() = 0;

    // Setups the OpenGL state for render your geometry
    // It is called once per render pass
    virtual void setupEnviroment();

    // Unset states of OpenGL to make sure that they do not affect other Renders
    // It is called once per render pass
    virtual void tearDownEnviroment();

    // Methods that will be executed every frame

    // First to be executed in the Rendering Engine
    // Sets the Enviorment for this shader. Data that is common to all meshes
    virtual void setupShader() = 0;

    // Second to be executed in the Rendering Engine
    // Sets the Enviorment for each Mesh, sends to the GPU the common information
    // of all objects that uses this mesh
    virtual void setupMesh(Mesh* mesh) = 0;

    // Third to be excuted
    // Draw each object with their specific data
    virtual void renderObject(ObjectPtr object) = 0;

    // Organize the render order for all objects and renders them
    virtual void renderPass();

protected:
    bool cull;

    ShaderPtr shader;
    std::map<Mesh*, std::list<ObjectPtr> > objects;
};

#endif
