#ifndef TEXT_RENDER_HPP
#define TEXT_RENDER_HPP

#include <list>

#include "render_element.hpp"
#include "FontEngine.h"
#include "text.hpp"

#include "pointer_macros.h"
DEF_PTR(TextRender, TextRenderPtr);

class TextRender : public RenderElement{

public:
    TextRender();

    void addText(TextPtr txt);
    void removeText(TextPtr txt);

    // Setup the font engine
    void setup();

    // Does nothing, the shader is loaded in the font engine
    void loadShader();

    void setupEnviroment();

    void tearDownEnviroment();

    void setupShader();

    void setupMesh(Mesh* mesh);

    void renderObject(ObjectPtr object);

    void renderPass();

private:
    FontEngine fontEngine;
    std::list<TextPtr> messages;
};


#endif
