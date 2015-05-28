#include "FontAtlas.h"

FontAtlas::FontAtlas(FT_Face face, int pixelSize) {
    FT_Set_Pixel_Sizes(face, 0, pixelSize);
    FT_GlyphSlot g = face->glyph;

    maxCharHeight = 0;
    int roww = 0, rowh = 0;
    atlasWidth = 0, atlasHeight = 0;
    memset(charInfo, 0, sizeof(charInfo));

    // find the min size for a texture holding the visible ASCII characters
    for (int i = ASCII_START; i < ASCII_END; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            fprintf(stderr, "Loading character %c failed!\n", i);
            continue;   // just skip this character
        }
        if (roww + g->bitmap.width + 1 >= MAX_TEX_WIDTH) {
            // go to the next row
            atlasWidth = std::max(atlasWidth, roww);
            atlasHeight += rowh;
            roww = 0;
            rowh = 0;
        }
        roww += g->bitmap.width + 1;
        rowh = std::max((int) rowh, (int) g->bitmap.rows);
    }

    atlasWidth = std::max(atlasWidth, roww);
    atlasHeight += rowh;

    // create the texture to hold the glyphs
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &atlasTexture);
    glBindTexture(GL_TEXTURE_2D, atlasTexture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth,
                 atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

    // need 1 byte alignment when uploading texture data
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glGenerateMipmap(GL_TEXTURE_2D);

    // clamp to edges to prevent scaling artifacts
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // add glyphs to the texture and store the offsets
    rowh = 0;
    int xOffset = 0, yOffset = 0;
    for (int i = ASCII_START; i < ASCII_END; i++) {
        if (FT_Load_Char(face, i, FT_LOAD_RENDER)) {
            fprintf(stderr, "Loading character %c failed!\n", i);
            continue;
        }

        if (xOffset + g->bitmap.width + 1 >= MAX_TEX_WIDTH) {
            yOffset += rowh;
            rowh = 0;
            xOffset = 0;
        }

        glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, g->bitmap.width,
                        g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

        charInfo[i].advanceX = g->advance.x >> 6;
        charInfo[i].advanceY = g->advance.y >> 6;

        charInfo[i].bitWidth = g->bitmap.width;
        charInfo[i].bitHeight = g->bitmap.rows;

        charInfo[i].bitLeft = g->bitmap_left;
        charInfo[i].bitTop = g->bitmap_top;

        charInfo[i].tex_offX = xOffset / (float)atlasWidth;
        charInfo[i].tex_offY = yOffset / (float)atlasHeight;

        maxCharHeight = std::max((int) maxCharHeight, (int) g->bitmap.rows);

        rowh = std::max((int) rowh, (int) g->bitmap.rows);
        xOffset += g->bitmap.width + 1;
    }
    maxCharHeight = rowh;
    lineHeight = maxCharHeight;
    printf("Generated a %d x %d (%d kb) texture atlas\n", 
           atlasWidth, atlasHeight, atlasWidth * atlasHeight / 1024);
}

FontAtlas::~FontAtlas() {
    glDeleteTextures(1, &atlasTexture);
}

int FontAtlas::getAtlasWidth() {
    return atlasWidth;
}

int FontAtlas::getAtlasHeight() {
    return atlasHeight;
}

GLuint FontAtlas::getAtlasTexture() {
    return atlasTexture;
}

FontAtlas::CharInfo FontAtlas::getCharInfo(int ch) {
    return charInfo[ch];
}

unsigned int FontAtlas::getMaxCharHeight() {
    return maxCharHeight;
}

unsigned int FontAtlas::getLineHeight() {
    return lineHeight;
}

void FontAtlas::addLineHeight(float val) {
    lineHeight += val;
}
