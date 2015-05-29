#ifndef FONTATLAS_H
#define FONTATLAS_H

#include <algorithm>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "text_shader_utils.h"

// this currently only supports the visible US-ASCII 
// characters (i.e. 32-127 in ascii tables)
class FontAtlas {
    public:
        FontAtlas(FT_Face face, int pixelSize);
        ~FontAtlas();

        typedef struct CharInfo {
            // how much to advance after character
            float advanceX;
            float advanceY;

            // x/y offset of glyph in tex coords
            float tex_offX;
            float tex_offY;

            // bitmap width/height, left/top
            float bitWidth, bitHeight;
            float bitLeft, bitTop;
        } CharInfo;

        int getAtlasWidth();
        int getAtlasHeight();

        GLuint getAtlasTexture();
        CharInfo getCharInfo(int ch);
        unsigned int getMaxCharHeight();
        unsigned int getLineHeight();
        void addLineHeight(float val);
    private:
        // start and end of visible ascii range
        static const int ASCII_START = 32;
        static const int ASCII_END = 127;

        // max width to make our atlas (before starting next row)
        static const int MAX_TEX_WIDTH = 1024;

        int atlasWidth, atlasHeight;
	    GLuint atlasTexture;

        CharInfo charInfo[ASCII_END + 1];

        // the max height of the loaded characters
        // (to determine line height for this font)
        unsigned int maxCharHeight;

        // current line height
        // initalizes to maxCharHeight and can be modified by user
        unsigned int lineHeight;
};

#endif
