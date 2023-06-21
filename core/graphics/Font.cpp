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
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    unsigned int nCols = 16;
    unsigned int nRows = 8;
    unsigned int textureWidth = nCols * fontSize;
    unsigned int textureHeight = nRows * fontSize;
    unsigned int fontSizeSize = fontSize * fontSize;
    float dx = 1.f / nCols;
    float dy = 1.f / nRows;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(
        GL_TEXTURE_2D, 0,
        GL_RED,
        textureWidth, textureHeight,
        0,
        GL_RED, GL_UNSIGNED_BYTE,
        nullptr);

    GLubyte *tex2d = new GLubyte[fontSizeSize];

    for (unsigned int i = 0; i < nCols; i++)
    {
        for (unsigned int j = 0; j < nRows; j++)
        {
            unsigned int c = i * nRows + j;

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
            characters[c].texCoord.x = i * dx;
            characters[c].texCoord.y = j * dy;
            characters[c].texSize.x = (float)xSize / (float)textureWidth;
            characters[c].texSize.y = (float)ySize / (float)textureHeight;

            for (unsigned int k = 0; k < fontSizeSize; k++)
            {
                tex2d[k] = 0;
            }

            for (int ic = 0; ic < ySize; ic++)
            {
                for (int jc = 0; jc < xSize; jc++)
                {
                    tex2d[ic * fontSize + jc] =
                        face->glyph->bitmap.buffer[ic * xSize + jc];
                }
            }

            glTexSubImage2D(
                GL_TEXTURE_2D, 0,
                i * fontSize, j * fontSize,
                fontSize, fontSize,
                GL_RED, GL_UNSIGNED_BYTE,
                tex2d);
        }
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);

    delete[] tex2d;

    FT_Done_Face(face);
    FT_Done_FreeType(ftlib);

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL,
                 GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float),
                          0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Font::~Font()
{
    glDeleteTextures(1, &textureID);
}

void Font::RenderText(
    Shader *shader_,
    std::string text_,
    float posX_, float posY_,
    float scale_, glm::vec3 color_)
{
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    shader_->SetVector3("textColor", glm::value_ptr(color_));
    shader_->SetScalar1("tex", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(VAO);

    int textLength = text_.length();
    const char *ctext = text_.c_str();
    for (int i = 0; i < textLength; i++)
    {
        unsigned int c = ctext[i];
        Character ch = characters[c];

        float x = posX_ + ch.bearing.x * scale_;
        float y = posY_ - (ch.size.y - ch.bearing.y) * scale_;
        float w = ch.size.x * scale_;
        float h = ch.size.y * scale_;
        float xt = ch.texCoord.x;
        float yt = ch.texCoord.y;
        float wt = ch.texSize.x;
        float ht = ch.texSize.y;

        // update VBO for each character
        float vertices[6][4] = {
            {x, y + h, xt, yt},           // 0
            {x, y, xt, yt + ht},          // 1
            {x + w, y, xt + wt, yt + ht}, // 2

            {x, y + h, xt, yt},           // 3
            {x + w, y, xt + wt, yt + ht}, // 4
            {x + w, y + h, xt + wt, yt}   // 5
        };
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        posX_ += (ch.advance / 64) * scale_;
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
}
