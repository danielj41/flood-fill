#ifndef RENDER_ENGINE_HPP
#define RENDER_ENGINE_HPP

#include <map>
#include <string>

#include "render_element.hpp"

class RenderEngine{

public:
    static void setup();
    static void render();

    // Add a render element to the render pipeline associated with some priority
    // Elements with priority zero will be the first to run in the pipeline
    // Priorities < 0 are not included in the pipeline
    static void addRenderElement(std::string name, RenderElement * renderElement, int priority);
    static RenderElement * getRenderElement(std::string name);
    static void removeRenderElement(std::string name);

private:
    static bool loaded;

    static std::map< std::string, RenderElement * > renderElements;
    static std::map< std::string, int > renderElementsPriority;

    static void setupOpenGL();
};

#endif
