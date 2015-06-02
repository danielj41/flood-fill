#include "text_render.hpp"

#include <sstream>

#include <iostream>
#include <cstdlib>
#include "debug_macros.h"

#include "global_variables.hpp"

TextRender::TextRender() : RenderElement(false) {
    fontEngine = FontEngine();
}

void TextRender::addText(TextPtr txt) {
    INFO("Adding text [" << txt->getText() << "] to the Text Render...");

    messages.push_front(txt);

    INFO("Adding " << txt->getFont() << " Font...");

    std::stringstream fontPath;
    fontPath << "flood-fill/fonts/" << txt->getFont() << ".ttf";

    if (!fontEngine.addFont(txt->getFont(), fontPath.str())) {
        ASSERT(false, "Could not add " << fontPath.str() << " to the font engine!");
    };
}

void TextRender::removeText(TextPtr txt) {
    INFO("Removing text [" << txt->getText() << "] from the Text Render...");

    for(auto it = messages.begin() ; it != messages.end(); it++){
        if(*it == txt){
            messages.remove(*it);
            INFO("Text Removed!");
            return;
        }
    }

    DEBUG("Could not find text!");
}

void TextRender::setup() {
    INFO("Text Render setup...");

    if (!fontEngine.init()) {
        ASSERT(false, "Could not initalize the font engine!");
    }

    fontEngine.updateWindowSize(Global::FbWidth, Global::FbHeight);
}

void TextRender::loadShader() {}

void TextRender::setupEnviroment() {
    glClear(GL_DEPTH_BUFFER_BIT);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDepthMask(GL_TRUE);

    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
}

void TextRender::tearDownEnviroment() {
}

void TextRender::setupShader() {}

void TextRender::setupMesh(Mesh*) {}

void TextRender::renderObject(ObjectPtr ) {}

void TextRender::renderPass() {
    INFO("Rendering the texts...");

    for(auto it = messages.begin() ; it != messages.end(); it++){
        TextPtr txt = *it;

        if (fontEngine.useFont(txt->getFont(), txt->getFontSize())) {
            glm::vec4 color = txt->getColor();
            glm::vec2 pos = txt->getPosition();

            fontEngine.setColor(color.x, color.y, color.z, color.w);
            fontEngine.renderText(txt->getText(), pos.x, pos.y);

            INFO("Rendering text " << txt->getText());
        }
    }
}
