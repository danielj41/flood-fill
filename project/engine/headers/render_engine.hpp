#ifndef RENDER_ENGINE_HPP
#define RENDER_ENGINE_HPP

#include <map>
#include <list>

#include "mesh.hpp"
#include "shader.hpp"
#include "object.hpp"
#include "camera.hpp"

class RenderEngine{

public:
    static void setup();
    static void render();

    static void addMesh(Mesh * mesh);
    static void addObject(Object * object);

    static void removeMesh(Mesh * mesh);
    static void removeObject(Object * object);

private:
    static bool loaded;

    static Shader * geometryShader;
    static Shader * geometryWaterShader;
    static Shader * geometryTextureShader;
    static Shader * debugShader;
    static std::map< Mesh *, std::list<Object *> > objects;
    static std::map< Mesh *, std::list<Object *> > waterObjects;
    static std::map< Mesh *, std::list<Object *> > texturedObjects;

    static int objectsCount;
    static int texturedObjectsCount;
    static int waterObjectsCount;
    static int totalObjects;

    static void loadShaders();
    static void setupOpenGL();

    static int renderObjects();
    static int renderWaterObjects();
    static int renderTexturedObjects();
};

#endif
