#include "Font.h"
#include <iostream>

Font::Font(
    std::string fontName_, int fontSize_)
    : fontName(fontName_), fontSize(fontSize_)
{
    FT_Library ftlib;
    if (FT_Init_FreeType(&ftlib))
        std::cout << "Error initializing FreeType2 library.\n";

    FT_Face face;
    if (FT_New_Face(ftlib, fontName.c_str(), 0, &face))
        std::cout << "Error loading font: " << fontName << std::endl;

    FT_Set_Pixel_Sizes(face, 0, fontSize);

    unsigned int nCols = 16;
    unsigned int nRows = 8;
    unsigned int textureWidth = nCols * fontSize;
    unsigned int textureHeight = nRows * fontSize;
    unsigned int fontSizeSize = fontSize * fontSize;
    float dx = 1.f / nCols;
    float dy = 1.f / nRows;

    for (unsigned int c = 0; c < nChars; c++)
    {

        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            std::cout << "Error loading glyph: " << c << std::endl;

        int xSize = face->glyph->bitmap.width;
        characters[c].size.x = xSize;
        int ySize = face->glyph->bitmap.rows;
        characters[c].size.y = ySize;
        int xBearing = face->glyph->bitmap_left;
        characters[c].bearing.x = xBearing;
        int yBearing = face->glyph->bitmap_top;
        characters[c].bearing.y = yBearing;
        characters[c].advance = face->glyph->advance.x;

        GLubyte *tex2d = new GLubyte[xSize * ySize];

        for (unsigned int ic = 0; ic < ySize; ic++)
        {
            for (unsigned int jc = 0; jc < xSize; jc++)
            {
                tex2d[ic * xSize + jc] = face->glyph->bitmap.buffer[ic * xSize + jc];
            }
        }

        characters[c].tex = tex2d;
    }
}

Font::~Font()
{
    for (int i = 0; i < nChars; i++)
    {
        delete[] characters[i].tex;
    }
}

TextTexture Font::RenderText(std::string text)
{
    int textLength = text.length();
    const char *ctext = text.c_str();

    int minY = INT_MAX, maxY = 0;
    int textWidth = 0;
    int baseHeight = 0;

    // calculate text dimensions
    for (int i = 0; i < textLength; i++)
    {
        unsigned int c = ctext[i];
        Character ch = characters[c];

        textWidth += ch.advance / 64;
        int y = -ch.size.y + ch.bearing.y;
        minY = glm::min(minY, y);
        maxY = glm::max(maxY, y + ch.size.y);
        baseHeight = glm::max(baseHeight, ch.bearing.y);
    }

    // create texture
    int textHeight = maxY - minY + 1;
    textWidth += 1;
    baseHeight += 1;
    GLubyte *textTex = new GLubyte[textHeight * textWidth * 4];

    for (int i = 0; i < textHeight * textWidth * 4; i++)
    {
        textTex[i] = 0;
    }

    int curX = 0;

    // fill texture with letters
    for (int i = 0; i < textLength; i++)
    {
        unsigned int c = ctext[i];
        Character ch = characters[c];

        int ySize = ch.size.y;
        int xSize = ch.size.x;

        for (unsigned int ic = 0; ic < ySize; ic++)
        {
            for (unsigned int jc = 0; jc < xSize; jc++)
            {
                int textX = curX + ch.bearing.x + jc;
                int textY = textHeight - (baseHeight - ch.bearing.y + ic) - 1;
                int index = textY * textWidth + textX;
                textTex[index * 4 + 0] = ch.tex[ic * xSize + jc];
                textTex[index * 4 + 1] = ch.tex[ic * xSize + jc];
                textTex[index * 4 + 2] = ch.tex[ic * xSize + jc];
                textTex[index * 4 + 3] = ch.tex[ic * xSize + jc];
            }
        }

        curX += ch.advance / 64;
    }

    return TextTexture{textWidth, textHeight, textTex};
}
