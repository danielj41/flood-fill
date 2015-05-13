#ifndef RENDER_ENGINE_HPP
#define RENDER_ENGINE_HPP

#include <map>
#include <string>

#include "render_element.hpp"
#include "render_grid.hpp"

class RenderEngine{

public:
    static void setup();
    static void render();

    static void addRenderElement(std::string name, RenderElement * renderElement);
    static RenderElement * getRenderElement(std::string name);
    static void removeRenderElement(std::string name);
    static void setRenderGrid(RenderGrid *_renderGrid);
    static RenderGrid *getRenderGrid();

private:
    static bool loaded;

    static std::map< std::string, RenderElement * > renderElements;
    static RenderGrid *renderGrid;

    static void setupOpenGL();
};

#endif
