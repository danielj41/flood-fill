#ifndef RENDER_ENGINE_HPP
#define RENDER_ENGINE_HPP

#include <list>

#include "render_element.hpp"

class RenderEngine{

public:
    static void setup();
    static void render();

    static void addRenderElement(RenderElement * renderElement);
    static void removeRenderElement(RenderElement * renderElement);

private:
    static bool loaded;

    static std::list< RenderElement * > renderElements;

    static void setupOpenGL();
};

#endif
